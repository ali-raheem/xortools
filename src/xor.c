#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  int keyfile = 0;
  char *key;
  char c;

  if (2 > argc) {
    printf("xortools - v0.2.6 - Ali Raheem");
    printf("\t%s [-f] key\n", argv[0]);
    puts("\t\t-f\t\tOptional, key parameter is a file");
    exit(EXIT_FAILURE);
  }
  
  while ((c = getopt(argc, argv, "-f:")) != -1) {
    switch (c) {
    case 'f':
      keyfile = 1;
      key = optarg;
      break;
    case 1:
      key = optarg;
      break;
    default:
      printf("xortools\n\t%s [-f] key", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  
  size_t key_len = strlen(key);
  
  if (keyfile) {
    FILE *fp;
    struct stat fpstat;
    fp = fopen(key, "rb");
    stat(key, &fpstat);
    key_len = fpstat.st_size;
    key = (char *) malloc(sizeof(char) * key_len);
    assert(NULL != key);
    fread(key, key_len, sizeof(char), fp);
  } else {
      //    key = argv[1];
    key_len = strlen(key);
  }

  size_t i = 0;

  do{
    fread(&c, 1, 1, stdin);
    printf("%c", c ^ key[i % key_len]);
    i++;
  }while(0 == feof(stdin));
    
  exit(EXIT_SUCCESS);
}