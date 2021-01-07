#include "command.h"
#include "builtin.h"
#include "tests/syscall_mock.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

bool builtin_is_internal(scommand cmd){
	assert(cmd != NULL);
	bool is_builtin;
	is_builtin = (strcmp(scommand_front(cmd),"exit")==0 || strcmp(scommand_front(cmd),"cd")==0);
	return is_builtin;
}

void builtin_exec(scommand cmd){
	assert(builtin_is_internal(cmd));
	if (!scommand_is_empty(cmd)){
		if (strcmp(scommand_front(cmd),"cd")==0){
			scommand_pop_front(cmd);
			chdir(scommand_front(cmd));
		}
		if (strcmp(scommand_front(cmd),"exit")==0){
			scommand_destroy(cmd);	
			exit(0);
		}
	}
}



