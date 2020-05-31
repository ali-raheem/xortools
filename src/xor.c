#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

size_t parse(char *outBuffer, const char *inBuffer, const size_t len) {
  int i, j;
  i = 0;
  j = 0;
  while(i < len) {
    switch(inBuffer[i]) {
    case '\\':
      assert(i + 1 < len);
      switch(inBuffer[++i]) {
      case '\\':
	outBuffer[j++] = inBuffer[i];
	break;
      case 'x':
	assert(i + 2 < len);
	assert(1 == sscanf(&inBuffer[i + 1], "%2x", (uint *) &outBuffer[j++]));
	i += 2;
	break;
      default:
	printf("Error: Don't know what '\\%c' means.\n", inBuffer[i]);
	exit(EXIT_FAILURE);
      }
      break;
    default:
      outBuffer[j++] = inBuffer[i];
    }
    i++;
  }
  return j;
}

int main(int argc, char **argv) {
  int keyfile = 0;
  char *key;
  char c;

  if (2 > argc) {
    printf("xortools - v0.2.8 - Ali Raheem");
    puts("https://github.com/ali-raheem/xortools");
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
  
  size_t key_len;
  
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
    key_len = strlen(key);
    char *keydata = (char *) malloc(key_len * sizeof(char));
    assert(NULL != keydata);
    key_len = parse(keydata, key, key_len);
    key = keydata;
  }

  size_t i = 0;

  do{
    fread(&c, 1, 1, stdin);
    printf("%c", c ^ key[i % key_len]);
    i++;
  }while(0 == feof(stdin));
    
  exit(EXIT_SUCCESS);
}
