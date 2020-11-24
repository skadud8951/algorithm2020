/* ID: 2016113566
 * NAME: KIMNAMYOUNG
 * OS: linux, Ubuntu 16.04
 * Compiler version: gcc 5.4.0 20160609
 */
 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct student {
	int id;
	char name[128];
	char major[128];
};

int main(void){
	struct student *myself;
	myself = (struct student*)malloc(sizeof(struct student));
	
	myself->id = 2016113566;
	strcpy(myself->name, "KIMNAMYOUNG");
	strcpy(myself->major, "Electronics Engineering");
	
	printf("ID: %d\n", myself->id);
	printf("NAME: %s\n", myself->name);
	printf("MAJOR: %s\n", myself->major);
}
