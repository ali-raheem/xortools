#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    puts("xortools - v0.2.7 - Ali Raheem");
    puts("https://github.com/ali-raheem/xortools");
    printf("\t%s INPUT_FILE KEY\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  char *key = argv[2];
  size_t key_len = strlen(key);

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
