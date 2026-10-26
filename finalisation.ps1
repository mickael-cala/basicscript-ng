# ==============================================================================
# FINALISATION AUTOMATIQUE DE SCRIPTBASIC2-NG
# ==============================================================================
$TargetDir = "C:\Users\micka\4QWEN\scriptbasic2-ng"
$SourceZig = "C:\Users\micka\4QWEN\ScriptBasic-master\scriba-zig"
$SourceNG  = "C:\Users\micka\4QWEN\ScriptBasic-master-ng"

Write-Host "🚀 Démarrage de la finalisation..." -ForegroundColor Green

# 1. Copie du noyau C
Write-Host "`n📁 Copie du noyau C..." -ForegroundColor Cyan
Copy-Item -Path "$SourceZig\src\*" -Destination "$TargetDir\src\main" -Recurse -Force
Write-Host "  ✓ Noyau C copié" -ForegroundColor Green

# 2. Copie des modules
Write-Host "`n📦 Copie des modules..." -ForegroundColor Cyan
Copy-Item -Path "$SourceZig\src\extensions\*" -Destination "$TargetDir\src\modules" -Recurse -Force
Copy-Item -Path "$SourceNG\src\modules\zlib" -Destination "$TargetDir\src\modules" -Recurse -Force
Copy-Item -Path "$SourceNG\src\modules\odbc" -Destination "$TargetDir\src\modules" -Recurse -Force
Write-Host "  ✓ Modules copiés" -ForegroundColor Green

# 3. Vendorisation
Write-Host "`n🔒 Vendorisation..." -ForegroundColor Cyan
Copy-Item -Path "$SourceNG\src\modules\zlib\*" -Destination "$TargetDir\src\vendor\zlib" -Recurse -Force
Copy-Item -Path "$SourceNG\resources\curl\*" -Destination "$TargetDir\src\vendor\curl\include" -Recurse -Force
Write-Host "  ✓ Dépendances vendorisées" -ForegroundColor Green

# 4. Application C23
Write-Host "`n🔧 Application des types C23..." -ForegroundColor Cyan
$filesModified = 0
Get-ChildItem -Path "$TargetDir\src" -Recurse -Include "*.c","*.h" | ForEach-Object {
    $content = Get-Content $_.FullName -Raw
    $modified = $false
    $content = $content -replace "\bunsigned long\b", "uint32_t"
    $content = $content -replace "\bunsigned int\b", "uint32_t"
    $content = $content -replace "\bunsigned short\b", "uint16_t"
    $content = $content -replace "\bunsigned char\b", "uint8_t"
    $content = $content -replace "\blong\b", "int32_t"
    $content = $content -replace "\bint\b", "int32_t"
    
    if ($content -notmatch "#include\s*<stdint\.h>") {
        $content = "#include <stdint.h>`n#include <stdbool.h>`n" + $content
        $modified = $true
    }
    
    if ($modified) {
        Set-Content -Path $_.FullName -Value $content -NoNewline -Encoding UTF8
        $filesModified++
    }
}
Write-Host "  → $filesModified fichiers mis à jour en C23" -ForegroundColor Green

# 5. Tests de contrat
Write-Host "`n🧪 Copie des tests..." -ForegroundColor Cyan
Copy-Item -Path "$SourceNG\test\contrat\*" -Destination "$TargetDir\test\contrat" -Recurse -Force
Write-Host "  ✓ Tests copiés" -ForegroundColor Green

# 6. Git
Write-Host "`n🔧 Initialisation Git..." -ForegroundColor Cyan
Set-Location $TargetDir
if (!(Test-Path ".git")) {
    git init
    git add .
    git commit -m "Initial commit: scriptbasic2-ng - fusion scriba-zig + ScriptBasic-master-ng (C23, Zig, standalone)"
    Write-Host "  ✓ Dépôt Git initialisé" -ForegroundColor Green
}

Write-Host "`n✅ FINALISATION TERMINÉE !" -ForegroundColor Green