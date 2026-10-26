' standalone_modules - la preuve que --standalone --modules colle les
' modules dans l'exécutable : ce script importe zlib, l'autonome tourne
' sans modules/ ni scriba.conf a cote de lui.
import zlib.bas
original = "standalone " & string$(200, "x")
c = zlib::Compress(original)
d = zlib::UnCompress(c)
if d = original and len(c) < len(original) then
  print "standalone modules : OK\n"
else
  print "standalone modules : ECHEC\n"
end if
