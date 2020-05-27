#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("xortools - v0.2.4 - May 2020 - Ali Raheem\n\t%s INPUT_FILE KEY_FILE\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  char *keyfilename = argv[2];
  
  FILE *keyFile;
  keyFile = fopen(keyfilename, "rb");
  assert(NULL != keyFile);
  struct stat fpstat;
  stat(keyfilename, &fpstat);
  size_t key_len = fpstat.st_size;

  char *key = (char *) malloc(key_len);
  assert(NULL != key);
  fread(key, key_len, 1, keyFile);
    
  FILE *inFile;
  inFile = fopen(filename, "rb");
  assert(NULL != inFile);
  
  char c;
  size_t i = 0;

  do{
    c = fgetc(inFile);
    printf("%c", c ^ key[i % key_len]);
    i++;
    //    if(feof(inFile)) break;
  }while(0 == feof(inFile));
    
  exit(EXIT_SUCCESS);
}
