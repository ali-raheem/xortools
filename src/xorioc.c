#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (3 != argc) {
    printf("xorioc\n\t%s FILE MAX_LEN\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int maxlen = atoi(argv[2]);
  
  FILE *fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END);
  size_t filesize = ftell(fp);
  rewind(fp);

  char *buffer = (char *) malloc(filesize);
  fread(buffer, filesize, 1, fp);

  if (filesize < maxlen) {
    maxlen = filesize - 1;
  }

  int i, j, k;
  for(i = 1; i < maxlen + 1; i++) {
    int total = 0;
    int matchs = 0;
    for(j = 0; j < filesize; j++) {
      for(k = i + j; k < filesize; k += i) {
	total++;
	if(buffer[j] == buffer[k]) matchs++;
      }
    }
    float ioc = (float) matchs / (float) total;
    printf("%04d %0.3f\%\n", i, ioc*100);
  }
  
}
