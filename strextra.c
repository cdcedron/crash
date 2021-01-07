#include "strextra.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

char * strmerge(char *s1, char *s2){
	assert(s1 != NULL && s2 != NULL);
	char * s;
	s = calloc(strlen(s1)+strlen(s2)+1,sizeof(char));
	strcpy(s,s1);
	strcat(s,s2);
	
	return s;
}
