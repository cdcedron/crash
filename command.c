#include "command.h"
#include "strextra.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <gmodule.h>
#include <assert.h>
#define REDIR_OUT	 '>'
#define REDIR_IN 	 '<'


struct scommand_s {
	GSequence * seq;
	char * redir_in;
	char * redir_out;
};

scommand scommand_new(void) {
	scommand result = calloc(1, sizeof(struct scommand_s));
	result->seq = g_sequence_new(NULL);
	result->redir_in = NULL;
	result->redir_out = NULL;
	return result;

}

scommand scommand_destroy(scommand self){
	assert(self != NULL);
	if (self->seq != NULL){
		g_sequence_free(self->seq);
		self->seq = NULL;
	}
	if (self->redir_in != NULL) {
		free(self->redir_in);
		self->redir_in = NULL;
	}
	if (self->redir_out != NULL) {
		free(self->redir_out);
		self->redir_out = NULL;
	}
	free(self);
	return NULL;
}

bool scommand_is_empty(const scommand self){
	assert(self != NULL);
	bool is_empty;
	is_empty = g_sequence_is_empty(self->seq);
	return is_empty;
}

void scommand_push_back(scommand self, char * argument){
	assert(self != NULL && argument != NULL);
	g_sequence_append(self->seq, argument);
	assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){
	assert(self != NULL && !scommand_is_empty(self));
	g_sequence_remove(g_sequence_get_begin_iter(self->seq));
}

void scommand_set_redir_in(scommand self, char * filename){
	assert(self != NULL);
	self->redir_in = filename;
}
void scommand_set_redir_out(scommand self, char * filename){
	assert(self != NULL);
	self->redir_out = filename;
}

unsigned int scommand_length(const scommand self){
	assert(self != NULL);
	unsigned int n;
	n = g_sequence_get_length (self->seq);
	return n;
}

char * scommand_front(const scommand self) {
	assert(self != NULL && !scommand_is_empty(self));
	return g_sequence_get(g_sequence_get_begin_iter(self->seq));
}

char * scommand_get_redir_in(const scommand self){
	assert(self != NULL);
	return self->redir_in;
	
}
char * scommand_get_redir_out(const scommand self){
	assert(self != NULL);
	return self->redir_out;
}

char * scommand_to_string(const scommand self){
	assert(self != NULL);

	char * s = calloc(1,sizeof(char));
	GSequenceIter * i;
	
	if (scommand_is_empty(self)){
		return s;
	} else {
		s = "\0";
		while(g_sequence_get_length (self->seq)>0){
			i = g_sequence_get_begin_iter(self->seq);
			s = strmerge(s, g_sequence_get(i));
			s = strmerge(s," ");
			g_sequence_remove(i);
		}
		if(self->redir_in != NULL){
				s = strmerge(s," < ");
			s = strmerge(s, " ");
				s = strmerge(s,scommand_get_redir_in(self));
			}

		if(self->redir_out != NULL){
				s = strmerge(s," > ");
				s = strmerge(s, " ");
				s = strmerge(s,scommand_get_redir_out(self));
		}
	}	
	return s;
}

struct pipeline_s {
	GSequence * pipseq;
	bool wait;
};

pipeline pipeline_new(void){
	pipeline result = calloc(1, sizeof(struct pipeline_s));
	result->pipseq = g_sequence_new(NULL);
	result->wait = true;
	return result;
}

pipeline pipeline_destroy(pipeline self){
	assert(self != NULL);
	if(self->pipseq != NULL){
		g_sequence_free(self->pipseq);
		self->pipseq = NULL;
	}
	free(self);
	self = NULL;
	return NULL;
}

bool pipeline_is_empty(const pipeline self){
	assert(self != NULL);
	bool is_empty = g_sequence_is_empty(self->pipseq);
	return is_empty;
}


void pipeline_push_back(pipeline self, scommand sc){
	assert(self != NULL && sc != NULL);
	g_sequence_append(self->pipseq, sc);
}

void pipeline_pop_front(pipeline self){
	assert(self != NULL && !pipeline_is_empty(self));
	g_sequence_remove(g_sequence_get_begin_iter(self->pipseq));
}	

void pipeline_set_wait(pipeline self, const bool w){
	assert(self != NULL);
	self->wait = w;
}

unsigned int pipeline_length(const pipeline self){
	assert(self != NULL);
	unsigned int n = g_sequence_get_length(self->pipseq);
	return n;
}

scommand pipeline_front(const pipeline self){
	assert(self != NULL && !pipeline_is_empty(self));
	return g_sequence_get(g_sequence_get_begin_iter(self->pipseq));
}
bool pipeline_get_wait(const pipeline self){
	assert(self != NULL);
	bool w;
	if(self->wait == true){
		w = true;
	}
	if(self->wait == false){
		w = false;
	}
	return w;
}

char * pipeline_to_string(const pipeline self){
	assert(self != NULL);
	char * p = calloc(1,sizeof(char));
	GSequenceIter * i;
	i = g_sequence_get_begin_iter(self->pipseq); 
	
	if (g_sequence_is_empty(self->pipseq)){
		p = p;
	}
	else{	
		while (!g_sequence_iter_is_end(i)){
			p = strmerge(p,scommand_to_string(g_sequence_get(i)));
			i = g_sequence_iter_next(i);
			if(i != g_sequence_get_end_iter(self->pipseq)){
				p = strmerge(p, "|");
			}
		}
		if(!pipeline_get_wait(self) && !pipeline_is_empty(self)){
		p = strmerge(p,"&");	
		}
	}
	return p;
}
