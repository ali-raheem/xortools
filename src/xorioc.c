#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (3 != argc) {
    printf("xortools - v0.2.2 - May 2020 - Ali Raheem\n\t%s FILE MAX_LEN\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int maxlen = atoi(argv[2]);

  FILE *fp = fopen(filename, "rb");
  assert(NULL != fp);
  struct stat fpstat;
  stat(filename, &fpstat);
  size_t filesize = fpstat.st_size; 
/*
  fseek(fp, 0L, SEEK_END);
  size_t filesize = ftell(fp);
  rewind(fp);
*/

  char *buffer = (char *) malloc(filesize);
  assert(NULL != buffer);
  fread(buffer, filesize, 1, fp);

  if (filesize < maxlen) {
    maxlen = filesize - 1;
  }


  float *ioc = (float *)malloc(sizeof (float) * maxlen);
  assert(NULL != ioc);
  float ioc_total;
  uint total, matchs;
  int i, j, k;
# pragma omp parallel for private(total, matchs, j, k) reduction(+:ioc_total)
  for(i = maxlen; i > 0; i--) {
    total = 0;
    matchs = 0;
    for(j = 0; j < filesize; j++) {
      for(k = i + j; k < filesize; k += i) {
	total++;
	if(buffer[j] == buffer[k]) matchs++;
      }
    }
    ioc[i] = (float) matchs / (float) total;
    ioc_total += ioc[i];
  }

  for(i = 1; i < maxlen + 1; i++) {
    printf("%04d %0.3f \n", i, maxlen*ioc[i]/ioc_total);
  }
}
