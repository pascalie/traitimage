/**
 * @file print-ppm.c
 * @brief print to standard output a color ppm file
 */

#include <stdlib.h>
#include <stdio.h>

static void process(int r, int g, int b, int rows, int cols){
  /* void the parameters to discard compilation unused warnings */
  (void)r;
  (void)g;
  (void)b;
  (void)rows;
  (void)cols;
}

void usage (char *s){
  fprintf(stderr,
	  "Usage: %s <r={0,255}> <g={0,255}> <b={0,255}> <rows> <cols>\n"
	  ,s);
  exit(EXIT_FAILURE);
}

#define PARAMS 5
int main(int argc, char *argv[]){
  if (argc != PARAMS+1){
    usage(argv[0]);
  }
  /* code to modify */
  int r=0, g=0, b=0, rows=0, cols=0;
  r = atoi(argv[1]);
  g = atoi(argv[2]);
  b = atoi(argv[3]);

  rows = atoi(argv[4]);
  cols = atoi(argv[5]);

  printf("P3\n");
  printf("%d %d\n", cols, rows);
  printf("255\n");
  
  for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			printf("%d %d %d ", r,g,b);
		}
		printf("\n");
  }

  process(r, g, b, cols, rows);
  return EXIT_SUCCESS;
}


