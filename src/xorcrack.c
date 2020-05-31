#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>


void gen_key(uint8_t *buffer, uint8_t *plaintext, uint8_t *key, int keylen) {
  int i = 0;
  while (i < keylen) {
    key[i] = buffer[i] ^ plaintext[i];
    i++;
  }
}

void decrypt(uint8_t *buffer, uint8_t *key, int keylen) {
  int i = 0;
  while (i < keylen) {
    buffer[i] = buffer[i] ^ key[i];
    i++;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    puts("xortools - v0.2.8 - Ali Raheem");
    puts("https://github.com/ali-raheem/xortools");
    printf("\t%s CRYPTFILE PLAINTEXT KEYLEN\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  uint8_t *crypt_filename = argv[1];
  uint8_t *plaintext = argv[2];
  int keylen = atoi(argv[3]);
  int found = 0;
  
  uint8_t *key = (uint8_t *) malloc(keylen);
  assert(NULL != key);
  
  size_t buffer_len = strlen(plaintext);

  if (buffer_len < 2*keylen) {
    puts("Plaintext must be at least twice as long as key length.");
    exit(EXIT_FAILURE);
  }
      
  uint8_t *buffer = (uint8_t *) malloc(buffer_len);
  assert(NULL != buffer);

  FILE *in = fopen(crypt_filename, "rb");
  assert(NULL != in);
  struct stat fpstat;
  stat(crypt_filename, &fpstat);
  size_t filesize = fpstat.st_size;

  long i = 0;
  do {
    size_t bytesread = fread(buffer, buffer_len, 1, in);
    gen_key(buffer, plaintext, key, keylen);
    decrypt(buffer + keylen, key, keylen);
    if (0 == memcmp(buffer + keylen, plaintext + keylen, keylen)) {
      found = 1;
      break;
    }
    i++;
    fseek(in, i, SEEK_SET);
  }while(i + buffer_len <= filesize);
  
  if (found) {
    i %= keylen;
    buffer[keylen] = 0;
    memcpy(buffer, key + keylen - i, i);
    memcpy(buffer + i, key, keylen - i);
    fwrite(buffer, keylen, 1, stdout);
  } else {
    fprintf(stderr, "No beans");
  }
}

