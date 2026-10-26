# Créer build.py
$buildPy = @'
#!/usr/bin/env python3
"""build.py - Build system for scriptbasic2-ng using Zig and C23"""

import os, sys, json, shutil, subprocess, argparse
from pathlib import Path
from typing import List, Dict, Optional

TARGETS = {
    "windows-x64": {"triple": "x86_64-windows-gnu", "exe_ext": ".exe", "lib_ext": ".dll", "flags": ["-DWIN32"], "libs": ["kernel32", "user32"]},
    "linux-x64": {"triple": "x86_64-linux-gnu", "exe_ext": "", "lib_ext": ".so", "flags": ["-DLINUX"], "libs": ["m", "dl", "pthread"]},
}

class Builder:
    def __init__(self, target: str):
        self.target = target
        self.config = TARGETS[target]
        self.root = Path(__file__).parent.parent
        self.build_dir = self.root / "build" / target
        self.dist_dir = self.root / "dist" / target
        
    def run_cmd(self, cmd: List[str]) -> None:
        print(f"  → {' '.join(cmd)}")
        result = subprocess.run(cmd, cwd=self.root, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"❌ Erreur: {result.stderr}")
            sys.exit(1)
    
    def build_all(self) -> None:
        print(f"🏗️  Build pour {self.target}")
        self.build_dir.mkdir(parents=True, exist_ok=True)
        self.dist_dir.mkdir(parents=True, exist_ok=True)
        
        # Compilation du noyau
        src_dir = self.root / "src" / "main"
        for src in src_dir.glob("*.c"):
            obj = self.build_dir / (src.stem + ".o")
            self.run_cmd(["zig", "cc", "-target", self.config["triple"], "-std=c23", "-c", str(src), "-o", str(obj)])
        
        print("✅ Build terminé !")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--target", required=True, choices=TARGETS.keys())
    args = parser.parse_args()
    Builder(args.target).build_all()
'@

$buildPy | Out-File -FilePath "C:\Users\micka\4QWEN\scriptbasic2-ng\scripts\build.py" -Encoding UTF8
Write-Host "✅ Script build.py créé" -ForegroundColor Green

# Créer test_runner.py
$testRunner = @'
#!/usr/bin/env python3
"""test_runner.py - Lance les tests de contrat"""

import sys, subprocess
from pathlib import Path

def run_tests(target: str) -> int:
    root = Path(__file__).parent.parent
    scriba = root / "dist" / target / ("scriba.exe" if "windows" in target else "scriba")
    test_dir = root / "test" / "contrat"
    
    if not scriba.exists():
        print(f"❌ {scriba} non trouvé")
        return 1
    
    passed = failed = 0
    for bas_file in sorted(test_dir.glob("*.bas")):
        out_file = bas_file.with_suffix(".out")
        if not out_file.exists():
            continue
        
        result = subprocess.run([str(scriba), str(bas_file)], capture_output=True, text=True, timeout=10)
        expected = out_file.read_text().strip()
        actual = result.stdout.strip()
        
        if expected == actual:
            print(f"✅ {bas_file.name}")
            passed += 1
        else:
            print(f"❌ {bas_file.name}")
            failed += 1
    
    print(f"\nRésultats: {passed} passés, {failed} échoués")
    return 0 if failed == 0 else 1

if __name__ == "__main__":
    sys.exit(run_tests(sys.argv[1]))
'@

$testRunner | Out-File -FilePath "C:\Users\micka\4QWEN\scriptbasic2-ng\scripts\test_runner.py" -Encoding UTF8
Write-Host "✅ Script test_runner.py créé" -ForegroundColor Green#!/usr/bin/env python3
"""
build.py - Build system for scriptbasic2-ng using Zig and C23
Supports: multi-target, --standalone, --libscriba
"""

import os
import sys
import json
import shutil
import subprocess
import argparse
from pathlib import Path
from typing import List, Dict, Optional

# Configuration des cibles
TARGETS = {
    "windows-x64": {
        "triple": "x86_64-windows-gnu",
        "exe_ext": ".exe",
        "lib_ext": ".dll",
        "flags": ["-DWIN32", "-D_WIN32"],
        "libs": ["kernel32", "user32", "advapi32", "shell32"]
    },
    "windows-x86": {
        "triple": "i386-windows-gnu",
        "exe_ext": ".exe",
        "lib_ext": ".dll",
        "flags": ["-DWIN32", "-D_WIN32", "-m32"],
        "libs": ["kernel32", "user32", "advapi32", "shell32"]
    },
    "linux-x64": {
        "triple": "x86_64-linux-gnu",
        "exe_ext": "",
        "lib_ext": ".so",
        "flags": ["-DLINUX", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    },
    "linux-arm64": {
        "triple": "aarch64-linux-gnu",
        "exe_ext": "",
        "lib_ext": ".so",
        "flags": ["-DLINUX", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    },
    "macos-x64": {
        "triple": "x86_64-macos",
        "exe_ext": "",
        "lib_ext": ".dylib",
        "flags": ["-DMACOS", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    },
    "macos-arm64": {
        "triple": "aarch64-macos",
        "exe_ext": "",
        "lib_ext": ".dylib",
        "flags": ["-DMACOS", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    }
}

class Builder:
    def __init__(self, target: str, release: bool = False):
        if target not in TARGETS:
            raise ValueError(f"Cible inconnue: {target}. Disponibles: {', '.join(TARGETS.keys())}")
        
        self.target = target
        self.config = TARGETS[target]
        self.release = release
        self.root = Path(__file__).parent.parent
        self.build_dir = self.root / "build" / target
        self.dist_dir = self.root / "dist" / target
        
        # Flags C23 standards
        self.cflags = [
            "-std=c23",
            "-Wall", "-Wextra", "-Werror",
            "-fvisibility=hidden",
            "-I" + str(self.root / "src" / "main"),
            "-I" + str(self.root / "src" / "vendor" / "zlib"),
            "-I" + str(self.root / "src" / "vendor" / "curl" / "include"),
        ]
        
        if release:
            self.cflags.extend(["-O3", "-DNDEBUG", "-flto"])
        else:
            self.cflags.extend(["-O0", "-g", "-DDEBUG"])
        
        self.cflags.extend(self.config["flags"])
        
    def run_cmd(self, cmd: List[str], cwd: Optional[Path] = None) -> subprocess.CompletedProcess:
        """Exécute une commande et retourne le résultat."""
        print(f"  → {' '.join(cmd)}")
        result = subprocess.run(
            cmd,
            cwd=cwd or self.root,
            capture_output=True,
            text=True
        )
        if result.returncode != 0:
            print(f"❌ Erreur: {result.stderr}")
            sys.exit(1)
        return result
    
    def compile_file(self, src: Path, obj: Path) -> None:
        """Compile un fichier C en objet."""
        obj.parent.mkdir(parents=True, exist_ok=True)
        
        cmd = [
            "zig", "cc",
            "-target", self.config["triple"],
            *self.cflags,
            "-c", str(src),
            "-o", str(obj)
        ]
        self.run_cmd(cmd)
    
    def build_main(self) -> List[Path]:
        """Compile le noyau de l'interpréteur."""
        print("\n🔨 Compilation du noyau...")
        src_dir = self.root / "src" / "main"
        obj_files = []
        
        if not src_dir.exists():
            print("⚠️  Dossier src/main non trouvé. Lancez finalize.ps1 d'abord.")
            return []
            
        for src in src_dir.glob("*.c"):
            obj = self.build_dir / "main" / (src.stem + ".o")
            self.compile_file(src, obj)
            obj_files.append(obj)
        
        return obj_files
    
    def build_modules(self) -> List[Path]:
        """Compile les modules dynamiques."""
        print("\n📦 Compilation des modules...")
        modules_dir = self.root / "src" / "modules"
        lib_files = []
        
        if not modules_dir.exists():
            print("⚠️  Dossier src/modules non trouvé.")
            return []
            
        for module_dir in modules_dir.iterdir():
            if not module_dir.is_dir():
                continue
            
            module_name = module_dir.name
            print(f"  → Module: {module_name}")
            
            obj_files = []
            for src in module_dir.glob("*.c"):
                obj = self.build_dir / "modules" / module_name / (src.stem + ".o")
                self.compile_file(src, obj)
                obj_files.append(obj)
            
            if not obj_files:
                continue
                
            # Lier en bibliothèque dynamique
            lib = self.dist_dir / (module_name + self.config["lib_ext"])
            lib.parent.mkdir(parents=True, exist_ok=True)
            
            cmd = [
                "zig", "cc",
                "-target", self.config["triple"],
                "-shared",
                *self.cflags,
                *[str(o) for o in obj_files],
                "-o", str(lib)
            ]
            
            # Ajouter les libs spécifiques au module
            if module_name == "zlib":
                pass # Déjà vendorisé
            elif module_name == "curl":
                cmd.extend(["-lcurl"])
            
            self.run_cmd(cmd)
            lib_files.append(lib)
        
        return lib_files
    
    def build_scriba(self, obj_files: List[Path]) -> Path:
        """Lie l'exécutable principal."""
        print("\n🔗 Liaison de scriba...")
        exe = self.dist_dir / ("scriba" + self.config["exe_ext"])
        exe.parent.mkdir(parents=True, exist_ok=True)
        
        cmd = [
            "zig", "cc",
            "-target", self.config["triple"],
            *self.cflags,
            *[str(o) for o in obj_files],
            "-o", str(exe)
        ]
        
        # Ajouter les libs système
        for lib in self.config["libs"]:
            cmd.append(f"-l{lib}")
        
        self.run_cmd(cmd)
        return exe
    
    def build_standalone(self, bas_file: Path) -> Path:
        """
        Crée un exécutable autonome contenant le bytecode du script BASIC.
        """
        print(f"\n🚀 Build standalone pour {bas_file.name}...")
        
        scriba = self.dist_dir / ("scriba" + self.config["exe_ext"])
        if not scriba.exists():
            print("❌ scriba non trouvé. Lancez d'abord un build normal.")
            sys.exit(1)
        
        bytecode_file = self.build_dir / (bas_file.stem + ".bc")
        self.run_cmd([str(scriba), "-C", str(bas_file), "-o", str(bytecode_file)])
        
        stub_c = self.build_dir / "standalone_stub.c"
        bytecode_data = bytecode_file.read_bytes()
        
        bytecode_array = ", ".join(f"0x{b:02x}" for b in bytecode_data)
        
        stub_code = f"""#include <stdint.h>
#include <stddef.h>

__attribute__((section(".rodata")))
const uint8_t embedded_bytecode[] = {{
    {bytecode_array}
}};

const size_t embedded_bytecode_size = sizeof(embedded_bytecode);

extern int scriba_execute_bytecode(const uint8_t* bc, size_t size);

int main(int argc, char** argv) {{
    return scriba_execute_bytecode(embedded_bytecode, embedded_bytecode_size);
}}
"""
        stub_c.write_text(stub_code)
        
        standalone_exe = self.dist_dir / (bas_file.stem + self.config["exe_ext"])
        
        cmd = [
            "zig", "cc",
            "-target", self.config["triple"],
            *self.cflags,
            str(stub_c),
            "-o", str(standalone_exe)
        ]
        
        for lib in self.config["libs"]:
            cmd.append(f"-l{lib}")
        
        self.run_cmd(cmd)
        print(f"✅ Exécutable autonome créé: {standalone_exe}")
        return standalone_exe
    
    def build_libscriba(self, obj_files: List[Path]) -> Path:
        """Crée une bibliothèque statique libscriba.a pour intégration."""
        print("\n📚 Création de libscriba...")
        lib = self.dist_dir / "libscriba.a"
        lib.parent.mkdir(parents=True, exist_ok=True)
        
        cmd = [
            "zig", "ar",
            "rcs", str(lib),
            *[str(o) for o in obj_files]
        ]
        self.run_cmd(cmd)
        return lib
    
    def build_all(self) -> None:
        """Build complet : noyau + modules + exécutable."""
        print(f"🏗️  Build pour {self.target} ({'release' if self.release else 'debug'})")
        
        if self.build_dir.exists():
            shutil.rmtree(self.build_dir)
        self.build_dir.mkdir(parents=True)
        self.dist_dir.mkdir(parents=True)
        
        obj_files = self.build_main()
        if not obj_files:
            print("⚠️  Aucun fichier objet compilé. Vérifiez src/main/")
            return
            
        self.build_modules()
        self.build_scriba(obj_files)
        self.build_libscriba(obj_files)
        
        print(f"\n✅ Build terminé ! Exécutables dans {self.dist_dir}")


def main():
    parser = argparse.ArgumentParser(description="Build system for scriptbasic2-ng")
    parser.add_argument("--target", required=True, choices=TARGETS.keys(), help="Cible de compilation")
    parser.add_argument("--release", action="store_true", help="Build en mode release (optimisé)")
    parser.add_argument("--standalone", type=Path, help="Crée un exécutable autonome pour le fichier .bas donné")
    parser.add_argument("--libscriba", action="store_true", help="Crée uniquement libscriba.a")
    
    args = parser.parse_args()
    
    builder = Builder(args.target, args.release)
    
    if args.standalone:
        builder.build_all()
        builder.build_standalone(args.standalone)
    elif args.libscriba:
        obj_files = builder.build_main()
        if obj_files:
            builder.build_libscriba(obj_files)
    else:
        builder.build_all()


if __name__ == "__main__":
    main()#!/usr/bin/env python3
"""
build.py - Système de build pour scriptbasic2-ng utilisant Zig et C23
Supporte : multi-cibles, --standalone, --libscriba
"""

import os
import sys
import shutil
import subprocess
import argparse
from pathlib import Path
from typing import List, Optional

# Configuration des cibles supportées
TARGETS = {
    "windows-x64": {
        "triple": "x86_64-windows-gnu",
        "exe_ext": ".exe",
        "lib_ext": ".dll",
        "obj_ext": ".o",
        "flags": ["-DWIN32", "-D_WIN32"],
        "libs": ["kernel32", "user32", "advapi32", "shell32", "ws2_32"]
    },
    "windows-x86": {
        "triple": "i386-windows-gnu",
        "exe_ext": ".exe",
        "lib_ext": ".dll",
        "obj_ext": ".o",
        "flags": ["-DWIN32", "-D_WIN32", "-m32"],
        "libs": ["kernel32", "user32", "advapi32", "shell32", "ws2_32"]
    },
    "linux-x64": {
        "triple": "x86_64-linux-gnu",
        "exe_ext": "",
        "lib_ext": ".so",
        "obj_ext": ".o",
        "flags": ["-DLINUX", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    },
    "linux-arm64": {
        "triple": "aarch64-linux-gnu",
        "exe_ext": "",
        "lib_ext": ".so",
        "obj_ext": ".o",
        "flags": ["-DLINUX", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    },
    "macos-x64": {
        "triple": "x86_64-macos",
        "exe_ext": "",
        "lib_ext": ".dylib",
        "obj_ext": ".o",
        "flags": ["-DMACOS", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    },
    "macos-arm64": {
        "triple": "aarch64-macos",
        "exe_ext": "",
        "lib_ext": ".dylib",
        "obj_ext": ".o",
        "flags": ["-DMACOS", "-fPIC"],
        "libs": ["m", "dl", "pthread"]
    }
}

class Builder:
    def __init__(self, target: str, release: bool = False):
        if target not in TARGETS:
            raise ValueError(f"Cible inconnue: {target}. Disponibles: {', '.join(TARGETS.keys())}")
        
        self.target = target
        self.config = TARGETS[target]
        self.root = Path(__file__).parent.parent
        self.build_dir = self.root / "build" / target
        self.dist_dir = self.root / "dist" / target
        
        # Flags C23 standards + optimisations de sécurité
        self.cflags = [
            "-std=c23",
            "-Wall", "-Wextra", "-Werror",
            "-fvisibility=hidden",
            "-I" + str(self.root / "src" / "main"),
            "-I" + str(self.root / "src" / "vendor" / "zlib"),
            "-I" + str(self.root / "src" / "vendor" / "curl" / "include"),
        ]
        
        if release:
            self.cflags.extend(["-O3", "-DNDEBUG", "-flto"])
        else:
            self.cflags.extend(["-O0", "-g", "-DDEBUG"])
        
        self.cflags.extend(self.config["flags"])
        
    def run_cmd(self, cmd: List[str], cwd: Optional[Path] = None) -> subprocess.CompletedProcess:
        """Exécute une commande et retourne le résultat."""
        print(f"  → {' '.join(cmd)}")
        result = subprocess.run(
            cmd,
            cwd=cwd or self.root,
            capture_output=True,
            text=True
        )
        if result.returncode != 0:
            print(f"❌ Erreur: {result.stderr}")
            sys.exit(1)
        return result
    
    def compile_file(self, src: Path, obj: Path) -> None:
        """Compile un fichier C en objet."""
        obj.parent.mkdir(parents=True, exist_ok=True)
        cmd = [
            "zig", "cc",
            "-target", self.config["triple"],
            *self.cflags,
            "-c", str(src),
            "-o", str(obj)
        ]
        self.run_cmd(cmd)
    
    def build_main(self) -> List[Path]:
        """Compile le noyau de l'interpréteur."""
        print("\n🔨 Compilation du noyau...")
        src_dir = self.root / "src" / "main"
        obj_files = []
        
        for src in src_dir.glob("*.c"):
            obj = self.build_dir / "main" / (src.stem + self.config["obj_ext"])
            self.compile_file(src, obj)
            obj_files.append(obj)
        
        # Compiler les sous-dossiers de commandes
        for cmd_dir in src_dir.glob("commands"):
            if cmd_dir.is_dir():
                for src in cmd_dir.glob("*.c"):
                    obj = self.build_dir / "main" / "commands" / (src.stem + self.config["obj_ext"])
                    self.compile_file(src, obj)
                    obj_files.append(obj)
        
        return obj_files
    
    def build_modules(self) -> List[Path]:
        """Compile les modules dynamiques."""
        print("\n📦 Compilation des modules...")
        modules_dir = self.root / "src" / "modules"
        lib_files = []
        
        for module_dir in modules_dir.iterdir():
            if not module_dir.is_dir():
                continue
            
            module_name = module_dir.name
            print(f"  → Module: {module_name}")
            
            obj_files = []
            for src in module_dir.glob("*.c"):
                obj = self.build_dir / "modules" / module_name / (src.stem + self.config["obj_ext"])
                self.compile_file(src, obj)
                obj_files.append(obj)
            
            # Lier en bibliothèque dynamique
            lib = self.dist_dir / (module_name + self.config["lib_ext"])
            lib.parent.mkdir(parents=True, exist_ok=True)
            
            cmd = [
                "zig", "cc",
                "-target", self.config["triple"],
                "-shared",
                *self.cflags,
                *[str(o) for o in obj_files],
                "-o", str(lib)
            ]
            
            # Ajouter les libs spécifiques au module
            if module_name == "zlib":
                # Utiliser la version vendorisée
                pass
            elif module_name == "curl":
                cmd.extend(["-lcurl"])
            elif module_name == "odbc" and "windows" in self.target:
                cmd.extend(["-lodbc32"])
            
            self.run_cmd(cmd)
            lib_files.append(lib)
        
        return lib_files
    
    def build_scriba(self, obj_files: List[Path]) -> Path:
        """Lie l'exécutable principal."""
        print("\n🔗 Liaison de scriba...")
        exe = self.dist_dir / ("scriba" + self.config["exe_ext"])
        exe.parent.mkdir(parents=True, exist_ok=True)
        
        cmd = [
            "zig", "cc",
            "-target", self.config["triple"],
            *self.cflags,
            *[str(o) for o in obj_files],
            "-o", str(exe)
        ]
        
        for lib in self.config["libs"]:
            cmd.append(f"-l{lib}")
        
        self.run_cmd(cmd)
        return exe
    
    def build_standalone(self, bas_file: Path) -> Path:
        """
        Crée un exécutable autonome contenant le bytecode du script BASIC.
        Le bytecode est injecté dans une section .rodata.
        """
        print(f"\n🚀 Build standalone pour {bas_file.name}...")
        
        scriba = self.dist_dir / ("scriba" + self.config["exe_ext"])
        if not scriba.exists():
            print("❌ scriba non trouvé. Lancez d'abord un build normal.")
            sys.exit(1)
        
        bytecode_file = self.build_dir / (bas_file.stem + ".bc")
        self.run_cmd([str(scriba), "-C", str(bas_file), "-o", str(bytecode_file)])
        
        stub_c = self.build_dir / "standalone_stub.c"
        bytecode_data = bytecode_file.read_bytes()
        bytecode_array = ", ".join(f"0x{b:02x}" for b in bytecode_data)
        
        stub_code = f"""
#include <stdint.h>
#include <stddef.h>

__attribute__((section(".rodata")))
const uint8_t embedded_bytecode[] = {{
    {bytecode_array}
}};

const size_t embedded_bytecode_size = sizeof(embedded_bytecode);

extern int scriba_execute_bytecode(const uint8_t* bc, size_t size);

int main(int argc, char** argv) {{
    return scriba_execute_bytecode(embedded_bytecode, embedded_bytecode_size);
}}
"""
        stub_c.write_text(stub_code)
        
        standalone_exe = self.dist_dir / (bas_file.stem + self.config["exe_ext"])
        cmd = [
            "zig", "cc",
            "-target", self.config["triple"],
            *self.cflags,
            str(stub_c),
            "-o", str(standalone_exe)
        ]
        
        for lib in self.config["libs"]:
            cmd.append(f"-l{lib}")
        
        self.run_cmd(cmd)
        print(f"✅ Exécutable autonome créé: {standalone_exe}")
        return standalone_exe
    
    def build_libscriba(self, obj_files: List[Path]) -> Path:
        """Crée une bibliothèque statique libscriba.a pour intégration."""
        print("\n📚 Création de libscriba...")
        lib = self.dist_dir / "libscriba.a"
        lib.parent.mkdir(parents=True, exist_ok=True)
        
        cmd = [
            "zig", "ar",
            "rcs", str(lib),
            *[str(o) for o in obj_files]
        ]
        self.run_cmd(cmd)
        return lib
    
    def build_all(self) -> None:
        """Build complet : noyau + modules + exécutable."""
        print(f"🏗️  Build pour {self.target} ({'release' if self.release else 'debug'})")
        
        if self.build_dir.exists():
            shutil.rmtree(self.build_dir)
        self.build_dir.mkdir(parents=True)
        self.dist_dir.mkdir(parents=True)
        
        obj_files = self.build_main()
        self.build_modules()
        self.build_scriba(obj_files)
        self.build_libscriba(obj_files)
        
        print(f"\n✅ Build terminé ! Exécutables dans {self.dist_dir}")


def main():
    parser = argparse.ArgumentParser(description="Build system for scriptbasic2-ng")
    parser.add_argument("--target", required=True, choices=TARGETS.keys(), help="Cible de compilation")
    parser.add_argument("--release", action="store_true", help="Build en mode release (optimisé)")
    parser.add_argument("--standalone", type=Path, help="Crée un exécutable autonome pour le fichier .bas donné")
    parser.add_argument("--libscriba", action="store_true", help="Crée uniquement libscriba.a")
    
    args = parser.parse_args()
    builder = Builder(args.target, args.release)
    
    if args.standalone:
        builder.build_all()
        builder.build_standalone(args.standalone)
    elif args.libscriba:
        obj_files = builder.build_main()
        builder.build_libscriba(obj_files)
    else:
        builder.build_all()

if __name__ == "__main__":
    main()