<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `unix`

Source : `src/extensions/unix/interface.c`

## Unix specific functions
This module implements some UNIX system calls that are not implemented
in the core of ScriptBasic but can be helpful for those who want to write
system maintenance scripts using ScriptBasic.

The reason that these functions are not implemented inside ScriptBasic
is that ScriptBasic itself is portable, and whenever a programmer writes a
program in pure ScriptBasic it should execute the same way under UNIX as well
as under Windows NT/Win98/W2K.

Programs using the module unix however are UNIX specific and will
not run unaltered under Windows NT.

## Fork the process

```
pid = unix::fork()
if pid then
  print "The sub process pid is ",pid
else
  print "Oh I am the child process"
end if
```

## Change file access mode
```
unix::chmod("file_name",mode)
```

This function implements the `chmod` UNIX system call.
