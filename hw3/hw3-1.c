/* ID: 2016113566		>>> REPLACE WITH YOUR ID
 * NAME: KIMNAMYOUNG	>>> REPLACE WITH YOUR NAME
 * OS: linux, Ubuntu 16.04 
 * Compiler version: gcc 5.4.0 20160609
 */

// no memcpy, memccpy, memmove, wmemmove, or other direct memory copy/move functions
//  these functions performs 'BLOCKED' operations so that large chunk of memory 
//  operations are efficiently implemented, so they break UNIT TIME assumption
//  in algorithm design
// string functions: strcpy, strncpy, strlen, strcmp are allowed, but not strdup

// >>> (10/100) pts
// >>> IN THE TOP 4-LINES COMMENTS 
// >>> LINE 1: REPLACE WITH YOUR ID (IF YOU HAVE NON-NUMERIC, IGNORE IT)
// >>> Line 2: REPLACE WITH YOUR NAME (NO HANGUL)
// >>> DO NOT CHANGE OS AND Compiler
// >>> COMPILE AND RUN YOUR CODE ON THE LINUX MACHINE

// HOMEWORK PROGRAMMING ASSIGNMENT 3
// IMPLEMENTATION OF SORTING ALGORITHMS FOR CHAR ARRAY STRINGS

#include<stdio.h>
#include<stdlib.h>	// atoi, malloc, free
#include<string.h>	// string library
#include<stdint.h>	// fixed-width integers, since C99 --- 1999 ISO standards

#define MEASURE_TIME	// to measure execution time

#define MEASURE_MEMORY	// to measure required memory
// from GJANG: free is not considered
// just counts the amount of memory required by malloc_c

/////////////////////////////////////////////////////////////////////
// to compute execution time in seconds
/////////////////////////////////////////////////////////////////////
#ifdef MEASURE_TIME
#include<time.h>
#endif

/////////////////////////////////////////////////////////////////////
// to compute used memory
// use malloc_c defined below, instead of malloc, calloc, realloc, etc.
// malloc_c accumulates the size of the dynamically allocated memory to
// global/static variable used_memory, so that we can measure the 
// used amount of memory exactly.
/////////////////////////////////////////////////////////////////////
#ifdef MEASURE_MEMORY
static size_t used_memory = 0;
void *malloc_c(size_t size)
{
  if ( size > 0 ) {
    // increase the required memory count
    used_memory += size;
    return malloc(size);
  }
  else return NULL;
}
#else
void *malloc_c(size_t size) { return malloc(size); }
#endif


/////////////////////////////////////////////////////////////
// read whole file into memory
/////////////////////////////////////////////////////////////
char* read_bytes( const char infile[], long *pNbytes )
  // returns an char array with its size stored in
  // the memory indicated by integer pointer variable pNbytes
  // the retured memory should freed by the caller
{
  char *B;
  long n;
  FILE *fp;

  // check for input file name
  if ( infile == NULL ) {
    fprintf(stderr, "NULL input file name\n");
    return NULL;
  }

  // check for file existence
  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s for read\n",infile);
    return NULL;
  }
  else {
    // find the file size
    fseek(fp,0,SEEK_END);	// move to the end of file
    *pNbytes = ftell(fp);	// file size = end loc from the start
    if ( *pNbytes <= 0 ) return NULL;	// no element

    // to the beginning again
    rewind(fp);

    // allocate memory and read the file
    B = (char*)malloc_c(*pNbytes+1);
    n = fread(B,1,*pNbytes,fp);
    if ( n != *pNbytes ) {
      fprintf(stderr, "file %s, %ld/%ld bytes read\n",infile,n,*pNbytes);
    }
    B[*pNbytes] = '\0';
    //fprintf(stderr, "file %s, %ld/%ld bytes read\n",infile,n,*pNbytes);

    // close file 
    fclose(fp);
    return B;
  }
}

/////////////////////////////////////////////////////////////
// write memory to a text file 
/////////////////////////////////////////////////////////////
void write_bytes( const char outfile[], const char B[], long Nbytes )
  // write the char array of size Nbytes
  // to file whose name given by outfile[]
{
  FILE *fp;
  long n;

  // check for output filename
  if ( outfile == NULL ) {
    fprintf(stderr, "NULL output file name\n");
    return;
  }

  // check for file existence
  fp = fopen(outfile,"w");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s for write\n",outfile);
  }
  else {
    n = fwrite(B,1,Nbytes,fp);
    if ( n != Nbytes ) {
      fprintf(stderr, "file %s, %ld/%ld bytes written\n",outfile,n,Nbytes);
    }
    //fprintf(stderr, "file %s, %ld/%ld bytes written\n",outfile,n,Nbytes);

    // close file 
    fclose(fp);
  }
}

/////////////////////////////////////////////////////////////
// FILL: YOUR AREA
// DEFINE YOUR OWN FUNCTIONS
/////////////////////////////////////////////////////////////
void merge(char* B, int left, int mid, int right){
	int l, r;
	l = left;
	r = mid + 1;
	int i=0;
	char *sortedArr;
	sortedArr = (char*)malloc_c(right+1);
	int s = left;
	
	while(l<=mid && r<=right){
		if(B[l]<=B[r]){
			while(l==mid+1){
				sortedArr[s]=B[l];
				s++;
				l++;
			}
			while(r==right+1){
					sortedArr[s]=B[r];
					s++;
					r++;
			}
		}
		else{
			while(r==right+1){
				sortedArr[s]=B[r];
				s++;
				r++;
			}
			while(l==mid+1){
				sortedArr[s]=B[l];
				s++;
				l++;
			}
		}
	}
	
	if(l>mid){
		for(i=r; i<=right; i++){
			sortedArr[s]=B[i];
			s++;
		}
	}
	else{
		for(i=l; i<=mid; i++){
			sortedArr[s]=B[i];
			s++;
		}
	}
	for(i = left; i<= right; i++) B[i]=sortedArr[i];
	free(sortedArr);
}//reference: ppt 31page merge sort C code

void merge_sort(char* B, int left, int right){
	int mid;
	int i;
	int index=0;
	
	if(left<right){
		mid = (left+right)/2;
		if(B[mid]=='.'){
			merge_sort(B, left, mid);
			merge_sort(B, mid+1, right);
		}
		else{ 
			i=0;
			while(B[mid+i] != '.'){ 
				index++;
			}
			if((mid+index)==right){ 
				merge(B, left, mid, right);
			}
			mid = mid+index; 
			
			merge_sort(B, left, mid);
			merge_sort(B, mid+1, right);
		}
		merge(B, left, mid, right);
	}
}//reference: ppt 31page merge sort C code

/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  long numBytes;
  char *B;	// to store data to be sorted
  int method;	// which sorting method
  int fixdotwordsize;
#ifdef MEASURE_TIME
  clock_t start, end;
  double cpu_time_used;
#endif

  if ( argc != 5 ) {
    fprintf(stderr, "usage: %s method fixedwordsize infile outfile\n",argv[0]);
    fprintf(stderr, " method 1: mergesort, 2: quicksort\n");
    exit(0);
  }

  method = atoi(argv[1]);
  fixdotwordsize = atoi(argv[2])+1;	// including last '.'
  B = read_bytes(argv[3], &numBytes);

#ifdef MEASURE_TIME
  start = clock();
#endif
  if ( B != NULL && numBytes > 0 ) {
    switch ( method ) {
      case 1:
		merge_sort(B, 0, numBytes);
		printf("\n\n%s\n\n",B);
	break;
      default:
	break;
    }

#ifdef MEASURE_TIME
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    fprintf(stdout,"%.5f seconds\n",cpu_time_used);
#endif
#ifdef MEASURE_MEMORY
    fprintf(stdout,"%lu bytes ( %.3f x %ld )\n",
	used_memory,(double)used_memory/(double)numBytes,numBytes);
	//used_memory,(double)used_memory/(double)(numBytes-1),numBytes-1);
#endif

    // save results
    write_bytes(argv[4], B, numBytes);

    // once allocated, should be freed after use
    free(B);
  }
}
