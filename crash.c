#include "command.h"
#include "builtin.h"
#include "execute.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main (void){
	pipeline pipe = NULL;
	Parser parser = NULL;
	while(true){
		parser = parser_new(stdin);
		pipe = parse_pipeline(parser);
		if(pipe != NULL){
		execute_pipeline(pipe);
		break;
		}
	}
	return 0;
}
