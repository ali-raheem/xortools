#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void gen_key(char *buffer, char *plaintext, char *key, int keylen) {
  int i = 0;
  while (i < keylen) {
    key[i] = buffer[i] ^ plaintext[i];
    i++;
  }
}

void decrypt(char *buffer, char *key, int keylen) {
  int i = 0;
  while (i < keylen) {
    buffer[i] = buffer[i] ^ key[i];
    i++;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("xorcrack\n\t%s CRYPTFILE PLAINTEXT KEYLEN\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *crypt_filename = argv[1];
  char *plaintext = argv[2];
  int keylen = atoi(argv[3]);
  int found = 0;
  
  char *key = (char *) malloc(keylen);
  assert(NULL != key);
  
  size_t buffer_len = strlen(plaintext);

  if (buffer_len < 2*keylen) {
    puts("Plaintext must be at least twice as long as key length.");
    exit(EXIT_FAILURE);
  }
      
  char *buffer = (char *) malloc(buffer_len);
  assert(NULL != buffer);

  FILE *in = fopen(crypt_filename, "rb");
  assert(NULL != in);
  fseek(in, 0L, SEEK_END);
  size_t filesize = ftell(in);
  rewind(in);

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
    int r = i % keylen;
    buffer[keylen] = 0;
    memcpy(buffer, key + keylen - r, r);
    memcpy(buffer + r, key, keylen - r);
    printf("Found match with key '%s'.\n", buffer);
  } else {
    puts("No beans");
  }
}

