/* ID: 2016113566
 * NAME: KIMNAMYOUNG
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
 
int main(int argc, char *argv[]){
	FILE *fp;
	int num;
	int min=0;
	int max=0;
	float mean=0;
	float variance=0;
	float sum=0;
		
	fp = fopen(argv[1], "r");
	
	fscanf(fp, "%d", &num);
	
	int A[num];

	for(int i=0; i<num; i++){
		fscanf(fp, "%d", &A[i]);
	}
	
	min=A[0];
	max=A[0];
	for(int i=0; i<num; i++){
		if(min>=A[i]){
			min = A[i];
		}
		if(max<=A[i]){
			max = A[i];
		}
	}
	
	for(int i=0; i<num; i++){
		sum = sum + A[i];
	}
	mean = sum/num;
	sum =0;
	for(int i=0; i<num; i++){
		sum = sum + (A[i]-mean)*(A[i]-mean);
	}
	variance = sum/num;
	
	printf("#data\tmin\tmax\tmean\tvariance\n");
	printf("%d\t%d\t%d\t%.1f\t%.1f\n", num, min, max, mean, variance);
	fclose(fp);
}
