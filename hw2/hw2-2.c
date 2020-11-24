/* ID: 2016113566
 * NAME: KIMNAMYOUNG
 * OS: linux, Ubuntu 16.04 
 * Compiler version: gcc 5.4.0 20160609
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MEASURE_TIME	
#ifdef MEASURE_TIME
#include<time.h>
#endif

#define MAX_STR_LEN	256

void print_chararr_dynamic( FILE *fp, char *A[], int N )
{
  int i;
  fprintf(fp,"%d\n",N);
  for (i=0; i<N; i++) fprintf(fp,"%s ",A[i]);
  fprintf(fp,"\n");
}

char** read_chararr_dynamic_textfile( const char infile[], int *pN )
{
  char **A;
  char tmp[MAX_STR_LEN];
  int i;
  FILE *fp;
  
  if ( infile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return NULL;
  }

  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",infile);
    return NULL;
  }
  else {
    if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
      fprintf(stderr, "cannot read number of elements %s\n",infile);
      return NULL;
    }
    else {
      A = (char**)malloc(sizeof(char*)*(*pN));
      for (i=0; i<(*pN); i++) {
	if ( fscanf(fp, "%s", tmp) != 1 ) {
	  fprintf(stderr, "cannot read value at %d/%d\n",i+1,(*pN));
	  *pN = i;
	  return A;
	}
	else {
	  A[i] = (char*) malloc(sizeof(char)*(strlen(tmp)+1));
	  strcpy(A[i],tmp);
	}
      }
    }
    fclose(fp);
    return A;
  }
}

void write_chararr_dynamic_textfile( const char outfile[], char *A[], int N )
{
  FILE *fp;

  if ( outfile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return;
  }

  fp = fopen(outfile,"w");
  if ( !fp ) {
    fprintf(stderr, "cannot open file for write %s\n",outfile);
  }
  else {
    print_chararr_dynamic(fp,A,N);
    fclose(fp);
  }
}

void insertion_sort_chararr_dynamic ( char* arr[], int n ) {
  int i, j;
  char *key;
  
  for(i=1; i<n; i++){
  	key = (char*)malloc(sizeof(char)*(strlen(arr[i])+1));
  	strcpy(key, arr[i]);
  	j = i - 1;
  	while( (j>=0) && (strcmp(arr[j],key) >0 )){
  		strcpy(arr[j+1], arr[j]);
  		j = j-1;
  	}
  	strcpy(arr[j+1], key);
  	free(key);
  }
} // >>> Source: alg20b-lec04, 61page, pseudo-code

void selection_sort_chararr_dynamic(char* arr[], const int n) {
  int i, j, indexmin;
  char *key;
  
  for(i=0; i<n-1; i++){
  	indexmin = i;
  	for(j=i+1; j<n; j++){
  		if(strcmp(arr[j], arr[indexmin]) < 0){
  			indexmin = j;
  		}
  	}
  	key = (char*)malloc(sizeof(char)*(strlen(arr[indexmin])+1));
  	strcpy(key, arr[indexmin]);
  	strcpy(arr[indexmin], arr[i]);
  	strcpy(arr[i], key);
  	free(key);
  }
} // >>> Source: alg20b-lec04, 75page, C code

void bubble_sort_chararr_dynamic(char* arr[], int n) {
	int i, j;
	char key[MAX_STR_LEN];
	
	for(i=n-1; i>0; i--){
		for(j=0; j<i; j++){
			if(strcmp(arr[j], arr[j+1])>0){
				strcpy(key, arr[j]);
				strcpy(arr[j], arr[j+1]);
				strcpy(arr[j+1], key);
			}
		}
	}
} // >>> Source: alg20b-lec04, 87page, C code

int main(int argc, char *argv[])
{
  int i, numElements;
  char **A;
#ifdef MEASURE_TIME
  clock_t start, end;
  double cpu_time_used;
#endif

  int method;	

  if ( argc != 4 ) {
    fprintf(stderr, "%d\n",argc);
    fprintf(stderr, "usage: %s method infile outfile\n",argv[0]);
    fprintf(stderr, " method 1: insertion, 2: selection, 3: bubble\n");
    exit(0);
  }

  method = atoi(argv[1]);

  A = read_chararr_dynamic_textfile(argv[2], &numElements);

#ifdef MEASURE_TIME
  start = clock();
#endif
  if ( A != NULL && numElements > 0 ) {
    switch ( method ) {
      case 1:	
	insertion_sort_chararr_dynamic(A, numElements);
	break;
      case 2:
	selection_sort_chararr_dynamic(A, numElements);
	break;
      case 3:	
	bubble_sort_chararr_dynamic(A, numElements);
	break;
      default:
	break;
    }

#ifdef MEASURE_TIME
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  fprintf(stderr,">> Execution time: %.5f seconds\n",cpu_time_used);
#endif

    write_chararr_dynamic_textfile(argv[3], A, numElements);

    for (i=0; i<numElements; i++) free(A[i]);
    free(A);
  }
}
