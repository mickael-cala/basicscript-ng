<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `xml`

Source : `src/extensions/xml/interface.c`

## Parse an XML file and read into memory

You should use this function to read the content of an XML file into memory.
The argument of the function is the name of the file to be read. The function
will return a handle to the structure created during the parsing of the file
content.

```
 DOC = xml::ParseFile("my_file.xml")
```

## Create a new empty document

```
 DOC = xml::NewDoc("1.0")
```

## Release a document from memory

When an XML document is not needed by the BASIC program any more the
program should free the memory used up by the document calling the
function `FreeDoc`.

```
xml::FreeDoc DOC
```

## Get the children on a document node

This function should be used to get the handle to the

## Set the children on a document node

This function should be used to get the handle to the

## Get the next node of a document node

This function should be used to get the handle to the

## Set the next on a document node

This function should be used to get the handle to the

## Get the prev node of a document node

This function should be used to get the handle to the

## Set the next on a document node

This function should be used to get the handle to the

## Get the Parent node of a document node

This function should be used to get the handle to the

## Set the next on a document node

This function should be used to get the handle to the

## set the property of a node

## get the property of a node

## Create a new name space defintion

Use this function to create a new name space definition.

```
  ns = xml::NewNs(node, href,prefix)
```

## Release a name space definition

Use this function to release a name space definition created by the function
`xml::NewNs`, which is not needed anymore.

```
  xml::FreeNs ns
```

## Create a new child under a node

Use this function to create a new child under an already existing node.

```
  newNode = xml::NewChild(parent,nameSpace,name,content)
```

Arguments:
- `parent`     is the parent node. The function will appens a new child node after
                    the last child node of the `parent` node.
- `nameSpace`  is optional handler to the name space. May be `undef`.
- `name`       is the name of the new node, should be a string.
- `content`    optional string content.

## Create a new text child under a node

Use this function to create a new text child under an already existing node.

```
  newNode = xml::NewTextChild(parent,nameSpace,name,content)
```

Arguments:
- `parent`     is the parent node. The function will appens a new child node after
                    the last child node of the `parent` node.
- `nameSpace`  is optional handler to the name space. May be `undef`.
- `name`       is the name of the new node, should be a string.
- `content`    optional string content.

## Create a new document node

Use this function to create a new document node.

```
  newNode = xml::NewDocNode(parent,nameSpace,name,content)
```

Arguments:
- `parent`     is the parent node. The function will appens a new child node after
                    the last child node of the `parent` node.
- `nameSpace`  is optional handler to the name space. May be `undef`.
- `name`       is the name of the new node, should be a string.
- `content`    optional string content.

## Convert a document to XML text
