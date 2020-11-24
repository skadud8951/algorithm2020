/* ID: 2016113566
 * NAME: KIMNAMYOUNG
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void sort(int a, int b, int c){
	int temp;
	int n[] = {a, b, c};
	int count=3;
	
	for(int i =0; i<count; i++){
		for(int j=0; j<count-1; j++){
			if(n[j]<n[j+1]){
				temp = n[j+1];
				n[j+1] = n[j];
				n[j] = temp;
			}
		}
	}
	printf("result: %d, %d, %d\n", n[0],n[1],n[2]);
}

int main(int argc, char *argv[]){
	int a;
	int b;
	int c;
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	c = atoi(argv[3]);
	
	if(argc != 4){
		printf("usage: %s <integer> <integer> <integer>\n", argv[0]);
	}
	else{
		sort(a, b, c);
	}
}
