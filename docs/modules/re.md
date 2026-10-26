<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `re`

Source : `src/extensions/re/interface.c`

## The module RE

This module implements some simple regular expression handling functions.
They can be used to match some strings against regular expressions and to
use matched sub strings to format replace strings. This module provides
functions that are similar to the Perl language operators `m//` and `s///`.

## Match a string against a regular expression

```
re::match(string,regexp [, replace])
re::m(string,regexp [, replace])
```

This function is the main regular expression match function. The first argument
is the string that is to be matched against the regular expression. The second
argument is the regular expression. If the string matches the regular expression the
return value of the function is true, otherwise false. The regular expressions
may contain parentheses inside it. If it does the substrings matching the
regular expression part between the parentheses will be stored in a
regular expression dollar array.

The substrings can be accessed via the function `re::dollar()` `dollar`.

If there is a replace string defined the return value is either false
if the string is not matched by the regular expression; or the replace string itself.
The replace string may contain `$0`, `$1`, ... `$n` literal that will be replaced by
the actual substrings between the parentheses. This is the same way as Perl does.

`$0` is the substring matched by the whole regular expression.

For example:
```
import re.bas

print "match result=",re::match("ahlma","h(.*)","haho $0 $1 $0 q")
print
n = re::n()
print "number of ()s =",n
print
for i=0 to n
  print i,". "
  print re::dollar(i)
  print
next i
```
will print
```
match result=haho hlma lma hlma q
number of ()s =1
0. hlma
1. lma
```
Note that the short for `re::m` exists in case you are a Perl
knowing programmer, because this function is similar to the
Perl operator `=~ m/`.

## number of matches

```
re::n()
```
return the number of usable matches substrings

## Return the i-th sub string

```
re::dollar(i)
re::$(i)
```

return the i-th matched substring

## Reset matches

```
re::reset()
```

Delete the `dollar` string array and release all memory that was allocated during the
last match. This function is autoamtically invoked between matches.

## Format a string with replacement sub-strings

```
re::format(string)
```

Use this function to format a string using the `$1`, `$2`, ... `$n` placeholders to be replaced by the actual strings after a successful pattern matching.

## Search and replace
```
re::replace(string,regexp,replace)
re::s(string,regexp,replace)
```
This function searches the string for matches of the regular expression and replaces the
actual matches with the replace string. This is the same functionality as the `=~s/` operator in Perl.

The function fills the `dollar` array the same way as the function `match`.

For example

```
import re.bas

print re::s("almakbat*","a(.)","$1s")
print
print re::$(0)," ",re::$(1)
print
```
will print

```
lsmksbts*
at t
```

As you can see in this case the value of `re::$(0)` is the string that was
matched by the last replace inside the string.
