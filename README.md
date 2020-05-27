# xortools

Small suit of programs for xor functions on files.

## Tools

### xorioc

Estimate xor key length using index of coincidence

#### Building

`gcc -O2 -fopenmp -o bin/xorioc src/xorioc.c`

OpenMP provides ~5% speed up, but means output will be scrambled and so has to wait until all processing is finished.

#### Usage

`xorioc file max_key_length` higher ioc suggests a particular key length is more likely to be correct. However, multiples and factors will confound the result.

`xorioc file max_key_length | sort -n -r -k2` will sort the output by decending likelihood. 

### xorcrack

Try and find a key given the key length and some plaintext

#### Building

`gcc -O2 -o bin/xorcrack src/xorcrack.c`

#### Usage

`xorcrack file plaintext key_length` The plaintext must be at least twice as long as keylength. And must occur in the (decrypted) file verbatim.

### xorkey

Encrypt/Decrypt files using a key

#### Building

`gcc -O2 -o bin/xorkey src/xorkey.c`

#### Usage

`xorkey file key`

Arbitary key's can be provided with printf.

`xorkey file "$(printf '\xDE\xAD\xBE\xEF')"`

### xorfile

Encrypt/Decrypt files using a keyfile.

#### Building

`gcc -O2 -o bin/xorfile src/xorfile.c`

#### Usage

`xorfile file keyfile`
