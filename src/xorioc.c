#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (3 != argc) {
    puts("xortools - v0.2.6 - Ali Raheem");
    printf("\t%s FILE MAX_LEN\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int maxlen = atoi(argv[2]);

  FILE *fp = fopen(filename, "rb");
  assert(NULL != fp);
  struct stat fpstat;
  stat(filename, &fpstat);
  size_t filesize = fpstat.st_size; 


  char *buffer = (char *) malloc(filesize);
  assert(NULL != buffer);
  fread(buffer, filesize, 1, fp);

  if (filesize < maxlen) {
    maxlen = filesize - 1;
  }


  float *ioc = (float *) malloc(sizeof (float) * maxlen);
  assert(NULL != ioc);
  
  float ioc_total = 0;
  uint total, matchs;
  int i, j, k;
# pragma omp parallel for private(total, matchs, j, k) reduction(+:ioc_total)
  for(i = maxlen; i > 0; i--) {
    total = 0;
    matchs = 0;
    for(j = 0; j < filesize; j++) {
      for(k = i + j; k < filesize; k += i) {
	if(buffer[j] == buffer[k]) matchs++;
      }
      total += (filesize - j) / i;
    }
    ioc[i - 1] = (float) matchs / (float) total;
    ioc_total += ioc[i - 1];
  }

  for(i = 0; i < maxlen; i++) {
    printf("%04d %0.3f \n", i + 1, maxlen*ioc[i]/ioc_total);
  }
}
