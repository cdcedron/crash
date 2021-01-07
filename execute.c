#include "command.h"
#include "builtin.h"
#include "execute.h"
#include "tests/syscall_mock.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


void execute_pipeline(pipeline apipe){
	assert(apipe != NULL );
	scommand aux = NULL;
	bool mwait = pipeline_get_wait(apipe);
	int f;
	unsigned int n;
	//int i = 0;
	
	while(!pipeline_is_empty(apipe)){
		aux = pipeline_front(apipe);
		/*char * red = scommand_to_string(aux);
		if (i>0){
			scommand_set_redir_in(aux,red);
		}*/
		if (builtin_is_internal(aux)){
			builtin_exec(aux);
			break;
		}

		else{
			f = fork();
			if (f==-1){
				printf("Child process could not be created.\n");
			}
			else if (f == 0){
				char * chain[513];
				char * command = scommand_front(aux);
				
				for(unsigned int i=0; i<513;i++){
					chain[i] = NULL;
				}
				n = scommand_length(aux);
				for(unsigned int j=0;j<n;j++){
					chain[j] = scommand_front(aux);
					scommand_pop_front(aux);
				}
				execvp(command,chain);
			}
			else{
				if(mwait){
					wait(NULL);
				}
			}
			
		}
		pipeline_pop_front(apipe);
		//i++;
		break;
	}
}

