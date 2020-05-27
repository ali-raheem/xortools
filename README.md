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

### xorcrack

Try and find a key given the key length and some plaintext

#### Building

`gcc -O2 -o bin/xorcrack src/xorcrack.c`

#### Usage

`xorcrack file plaintext key_length` The plaintext must be at least twice as long as keylength. And must occur in the (decrypted) file verbatim.

### xorkey

Encrypt/Decrypt files

#### Building

`gcc -O2 -o bin/xorkey src/xorkey.c`

#### Usage

`xorkey file key`

