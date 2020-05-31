#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

size_t parse(uint8_t *outBuffer, const uint8_t *inBuffer, const size_t len) {
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
	assert(1 == sscanf(&inBuffer[i + 1], "%2x", (int *) &outBuffer[j++]));
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

void usage(char *progname) {
    printf("xortools - v0.2.8 - Ali Raheem");
    puts("https://github.com/ali-raheem/xortools");
    printf("\t%s [-f] key\n", progname);
    puts("\t\t-f\t\tOptional, key parameter is a file");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int keyfile = 0;
  uint8_t *key;
  uint8_t c;
  size_t key_len;

  if (2 > argc) {
    usage(argv[0]);
  }

  key = argv[1];
  if(0 == strcmp("-f", key)) {
    if (3 != argc) {
      usage(argv[0]);
    }
    keyfile = 1;
    key = argv[2];
  }
  
  
  if (keyfile) {
    FILE *fp;
    struct stat fpstat;
    fp = fopen(key, "rb");
    stat(key, &fpstat);
    key_len = fpstat.st_size;
    key = (uint8_t *) malloc(sizeof(uint8_t) * key_len);
    assert(NULL != key);
    fread(key, key_len, sizeof(uint8_t), fp);
  } else {
    key_len = strlen(key);
    uint8_t *keydata = (uint8_t *) malloc(key_len * sizeof(uint8_t));
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
