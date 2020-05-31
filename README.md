# xortools

Small suit of programs for xor functions on files.

## Tools

### xorioc

Estimate xor key length using index of coincidence

#### Building

`gcc -O2 -fopenmp -o bin/xorioc src/xorioc.c`

OpenMP provides ~5% speed up, but means output will be scrambled and so has to wait until all processing is finished.

##### xorioc benchmarks

Working with a 70KB random file `dd if=/dev/uranomd of=test bs=1K count=70` here are the results on my laptop (i5-2430M CPU @ 2.40GHz).

###### -march=native -mtune=native -O3 -fopenmp
real    0m8.467s
user    0m13.522s
sys     0m0.016s

###### -march=native -mtune=native -O3
real    0m10.639s
user    0m10.638s
sys     0m0.000s

###### -O2  -fopenmp 
real    0m8.413s
user    0m13.682s
sys     0m0.017s

###### -O3 -fopenmp
real    0m8.708s
user    0m13.637s
sys     0m0.028s


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

### xor

Encrypt and decrypt with key or keyfile from STDIN.

#### Building

`gcc -O2 -o bin/xor src/xor.c`

#### Usage

`cat xor.c | xor "hunter2"`

or from a keyfile

`echo -n "hunter2" > keyfile`  -n will supress echo's trailing new line.

`cat xor.c | xor -f keyfile`

## Workflow

Using encrypted firmware as an example.

1. You can use `xorioc` to see if you have reason to believe xor encryption is used and how long the key might be. **This is the slowest step in my experience, and equally you might just skip to step 3**. I used `xorioc firmware 32`.
2. In my case it looks like 8, 16, 32 are equally likely (2.45~2.48). Lets start with 8 (shortest).
3. We need to guess what the firmware might contain, my experience with this particular family "JUAN IPCC" is that it's likely to have loads of symbolic links to `../../bin/busybox`. That's likely very common in all firmware so long as it's not obstrufcated by the packaging.
4. Using `xorcrack firmware '../../bin/busybox' 8` gives us a hit with the password "12345678".
5. We can decrypt with `xorkey firmware "12345678" > decrypted`.
