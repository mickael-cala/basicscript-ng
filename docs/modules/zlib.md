<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `zlib`

Source : `src/extensions/zlib/interface.c`

## Zlib functions

To use these function the program has to import the file `zlib.sbm`

```
import zlib.sbm
```
--------------------------------------

The functions in this module are:

## Compress a string
```
CompressedString = zlib::Compress(UncompressedString)
```
This function implements a simple compression interface.

## Uncompress a string
```
UncompressedString = zlib::UnCompress(CompressedString)
```
This function implements a simple uncompression interface.

## Compress a file

```
zlib::gzip input_file [,output_file]
```
This command compresses a file.

## Uncompress a file

```
zlib::gunzip input_file [,output_file]
```
This command uncompresses a file.
