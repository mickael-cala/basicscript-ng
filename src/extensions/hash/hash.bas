'
' Module hash
'
' This module lets you create memory stored hashes.
'
' Values are returned by copy. Modifying a returned value does not modify
' the value stored in the hash; use SetValue again for that.
'
module hash


const ErrorInvalidHandle = &H80001
const ErrorNoThisKey     = &H80002

'
' Create a new hash and return the handle to the hash
'
' h = hash::New()
declare sub ::New alias "newh" lib "hash"

'
' Set a value in the hash
'
' hash::SetValue h,key,value
declare sub ::SetValue alias "sethv" lib "hash"

'
' Get a copy of the value for a given key
'
' q = hash::Value(h,key)
declare sub ::Value alias "gethv" lib "hash"

'
' Check that a key exists in the hash (may have undef value)
'
' if hash::Exists(h,key) then ...
declare sub ::Exists alias "ivhv" lib "hash"

'
' Delete a key/value pair from the hash
'
' hash::Delete h,key
declare sub ::Delete alias "delhk" lib "hash"

'
' Start iteration setting the pointer to the first element
'
' hash::Start h
declare sub ::Start alias "starth" lib "hash"

'
' Start iteration backward setting the pointer to the last element
'
' hash::End h
declare sub ::End alias "endh" lib "hash"

'
' Go forward one element
'
' hash::Next h
declare sub ::Next alias "nexthk" lib "hash"

'
' Go backward one element
'
' hash::Prev h
declare sub ::Prev alias "pervhk" lib "hash"

'
' Get the key of the actual key/value pair
'
' k = hash::ThisKey(h)
declare sub ::ThisKey alias "thishk" lib "hash"

'
' Get a copy of the value of the actual key/value pair
'
' v = hash::ThisValue(h)
declare sub ::ThisValue alias "thishv" lib "hash"

'
' Release a hash and free allocated memory
'
' hash::Release h
declare sub ::Release alias "relh" lib "hash"

end module
