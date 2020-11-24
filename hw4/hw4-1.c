/* ID: 2016113566
 * NAME: KIMNAMYOUNG
 * OS: linux, Ubuntu 16.04 
 * Compiler version: gcc 5.4.0 20160609
 */

// NOTE: IN GENERAL, COMMENTED STATEMENTS FOLLOWED BY 
// ">>>" WRITTEN IN CAPITAL LETTERS ARE DIRECTIONS

// >>> (10/100) pts
// >>> IN THE TOP 4-LINES COMMENTS 
// >>> LINE 1: WRITE YOUR ID (NON-NUMERIC LETTERS ARE FINE)
// >>> Line 2: WRITE YOUR NAME (NO HANGUL)
// >>> DO NOT CHANGE OS OR Compiler
// >>> COMPILE AND RUN YOUR CODE ON THE LINUX MACHINE

// HOMEWORK PROGRAMMING ASSIGNMENT 4-1
// HASH FUNCTION DESIGN FOR WORD COUNTING

#include<stdio.h>
#include<stdlib.h>	// atoi
#include<string.h>	// string library

#define MEASURE_TIME	// to measure execution time

#define MEASURE_MEMORY	// to measure required memory

#ifdef MEASURE_TIME
/////////////////////////////////////////////////////////////////////
// to compute execution time in seconds
/////////////////////////////////////////////////////////////////////
#include<time.h>
#endif

#ifdef MEASURE_MEMORY
/////////////////////////////////////////////////////////////////////
// to compute used memory
/////////////////////////////////////////////////////////////////////
static size_t used_memory = 0;

// allocate memory with counting bytes
void *malloc_c(size_t size)
{
  if ( size > 0 ) {
    // increase the required memory count
    used_memory += size;
    return malloc(size);
  }
  else return NULL;
}

// create a duplicate word with counting bytes
char *strdup_c(const char *s) { 
  int size;
  size = strlen(s)+1;	// including last null character
  used_memory += size;
  return strdup(s);
}

#else
void *malloc_c(size_t size) { return malloc(size); }
char *strdup_c(const char *s) { return strdup(s); }
#endif

/////////////////////////////////////////////////////////////
// linked list for word counting
/////////////////////////////////////////////////////////////
struct WORDNODE {
  char *word;	// content
  int count;	// number of appearances
  struct WORDNODE *next;
};

struct WORDNODE *create_wnode( const char w[] ) {
  struct WORDNODE *newnode;

  // create a new word node
  newnode = (struct WORDNODE*)malloc_c(sizeof(struct WORDNODE));
  newnode->word = strdup_c(w);
  newnode->count = 1; //중복 갯수 세려고
  newnode->next = NULL;
  return newnode;
}

void free_wnodelist_recursive(struct WORDNODE *node) { 
  if ( node ) {
    free_wnodelist_recursive(node->next);
    free(node->word);
    free(node);
  }
}


/* ========= FILL ======== */
// functions for handing struct WORDNODE
// changing the struct or 
// defining any additional functions and structs are allowed
/* ========= END OF FILL ======== */


/////////////////////////////////////////////////////////////
// hash table for word counting
/////////////////////////////////////////////////////////////
struct WORDHASHTABLE {
  struct WORDNODE **wnode;	// hash table, array of word node lists
  int size;	// size of the hash table
  int num_words;	// count unique number of words, same as the length of the list
};

struct WORDHASHTABLE *create_word_hashtable( int hashtabsize ) 
{
  int i;
  struct WORDHASHTABLE *htab;

  htab = (struct WORDHASHTABLE*)malloc_c(sizeof(struct WORDHASHTABLE));
  htab->wnode = (struct WORDNODE**)malloc_c(sizeof(struct WORDNODE*)*hashtabsize);
  for (i=0; i<hashtabsize; i++) htab->wnode[i] = NULL;
  htab->size = hashtabsize;
  htab->num_words = 0;
  return htab;
}

void free_hashtable(struct WORDHASHTABLE *htab){
  int i;
  if ( htab ) {
    for (i=0; i<htab->size; i++) 
      free_wnodelist_recursive(htab->wnode[i]);
    free(htab);
  }
}

void insert(struct WORDHASHTABLE *htab, int key, char w[]){
	struct WORDNODE *newnode = create_wnode(w);
	struct WORDNODE *cur = NULL;
	
	if(htab->wnode[key] == NULL){ 
		htab->wnode[key] = newnode;
	}
	else{
		cur = htab->wnode[key];
		
		if(strcmp(cur->word,w)>0){ 
			newnode->next = htab->wnode[key];
			htab->wnode[key] = newnode;
			return;
		}
		else{
			while(1){
				if(strcmp(cur->word,w)==0){
					cur->count++;
					return;
				}
			
				if(cur->next == NULL){
					cur->next = newnode;
					return;
				}
				if(strcmp(w,cur->next->word)<0){
					newnode->next = cur->next;
					cur->next = newnode;
					return;
				}
				cur = cur->next;
			}
		}
		
	free_wnodelist_recursive(cur);
	}
} // reference: http://blog.naver.com/PostView.nhn?blogId=ifkiller&logNo=70080058562


void print_hashtable(struct WORDHASHTABLE *htab){
	int i=0;
	struct WORDNODE *cur = NULL;
	
	for(i=0; i<htab->size; i++){
		cur = htab->wnode[i];
		printf("bucket %d ",i);
		while(cur != NULL){
			printf("(");
			printf("%s ",cur->word);
			printf("%d", cur->count);
			printf(") ");
			cur=cur->next;
		}
		printf("\n");
	}
	free_wnodelist_recursive(cur);
}

/* ========= FILL ======== */
// functions for handing struct WORDHASHTABLE
// changing the struct or 
// defining any additional functions and structs are allowed
/* ========= END OF FILL ======== */


/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  struct WORDHASHTABLE *hashtable;
  int htabsize;
  char buffer[256];	// input string
  char *infile, *outfile;
  FILE *ifp, *ofp;
  int j;
  int i=0;
  int key=0;
  int k=0;
  struct WORDNODE *cur = NULL;
	
  /* ========= FILL ======== */
  // any additional variables can be defined here
  /* ========= END OF FILL ======== */

#ifdef MEASURE_TIME
  clock_t start, end;
  double cpu_time_used;
  start = clock();
#endif

  // INPUT ARGUMENTS
  if ( argc != 4 ) {
    //fprintf(stderr, "%d\n",argc);
    fprintf(stderr, "usage: %s hashtablesize infile outfile\n",argv[0]);
    exit(0);
  }
  else { 
    htabsize = atoi(argv[1]);
    infile = argv[2];
    outfile = argv[3];
  }

  // create a hash table
  hashtable = create_word_hashtable(htabsize);

  /* DESCRIPTION */
  /* The hash table uses the sum of ascii codes of the input word,
   * divided by the hash table size (variable htabsize, argv[1]),
   * and the hash value is the remainder of the integer division (%)
   * Each bucket is a linked list of struct WORDNODE, sorted by an alphabetical order.
   *
   * Detailed procedure:
   * 1. read word
   * 2. compute its hash value (bucket index)
   * 3. insert the word into the right bucket of the hash table 
   *    when the word is already in the hash table, increase count
   *    otherwise, create a new node and insert it into the right bucket 
   *    in an sorted order 
   * 4. repeat reading words until the end of file
   * 5. print the hash table to file "outfile"
   * 6. print the sorted list of all the words
   *
   * Template file: template_hw4-1.c
   * - codes for struct WORDNODE, struct WORDHASHTABLE, 
   *   and their creation and free to show how to allocate memory 
   *   with malloc_c, instead of malloc, to count the memory usage
   * - the struct can be changed, as long as the requirements being satisfied
   *   and the output file is the same
   * - codes for execution time measurement and memory usage counting
   * - codes for file I/O (but not all) is given to show
   *   how to read a text file to the end of it
   *
   * Execution example) (more examples can be found in example4-1.log)
 
$ cat input/i31.txt 	// input file
sit lorem dolor diam diam lorem sed dolor diam sit amet 
diam diam lorem ipsum lorem ipsum amet sit lorem sed 
ipsum sit ipsum amet dolor dolor sed sit ipsum diam

$ ./hw4-1.exe 3 input/i31.txt output/i31_4-1_h3.txt
0.00026 seconds
324 bytes ( 46.286 x 7 )

$ cat output/i31_4-1_h3.txt	// output file
hashtable 3 buckets 7 words
bucket 0 (amet 3) (diam 6) (ipsum 5) (lorem 5) (sit 5)
bucket 1 (dolor 4) (sed 3)
bucket 2

alphabet-sorted 7 words
(amet 3) (diam 6) (dolor 4) (ipsum 5) (lorem 5) (sed 3) (sit 5)

   */

  // check for input file existence
  if ( (ifp = fopen(infile,"r")) == NULL ){
    fprintf(stderr, "cannot open file %s\n",infile);
    return -1;
  }
  else {
    // read word one by one to the end of file
    for (j=0; !feof(ifp); j++){	// feof: end of file
      // 1. read word
      if ( fscanf(ifp, "%s", buffer) != 1 ) break;	// out of the loop
      else {
		   while(buffer[i]!=NULL){
			   key += buffer[i];
			   i++;
		   }
		   key = key%htabsize;
		   insert(hashtable, key, buffer);
	/* ========= FILL ======== */
	// 2. compute its hash value (bucket index)
	// 3. insert the word into the right bucket of the hash table 
	//   when the word is already in the hash table, increase count
	//   otherwise, create a new node and insert it into the right bucket 
	//   in an sorted order 
	// 4 repeat reading words until the end of file
	/* ========= END OF FILL ======== */
      }
	  key =0;
	  i=0;
    }
    fclose(ifp);	// read done

    // check for output file
    if ( (ofp = fopen(outfile,"w")) == NULL ) {
      fprintf(stderr, "cannot open file for write %s\n",outfile);
      return -1;
    }
    else {
//		print_hashtable(hashtable);

		for(k=0; k<hashtable->size; k++){
			cur = hashtable->wnode[k];
			fprintf(ofp,"bucket %d ",k);
			while(cur != NULL){
				fprintf(ofp, "(");
				fprintf(ofp,"%s ",cur->word);
				fprintf(ofp,"%d", cur->count);
				fprintf(ofp,") ");
				cur=cur->next;
			}
			fprintf(ofp,"\n");
		}
		free_wnodelist_recursive(cur);
      /* ========= FILL ======== */
      // 5. print the hash table to the output file (ofp)
      // 6. print the sorted list of all the words to the output file (ofp)
      /* ========= END OF FILL ======== */
      fclose(ofp);
    }
  } 

#ifdef MEASURE_TIME
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  fprintf(stdout,"%.5f seconds\n",cpu_time_used);
#endif
#ifdef MEASURE_MEMORY
  fprintf(stdout,"%lu bytes ( %.3f x %d )\n",
      used_memory,(double)used_memory/(double)hashtable->num_words,hashtable->num_words);
#endif
  free_hashtable(hashtable);
  return 0;
}
