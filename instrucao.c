#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "instrucao.h"

struct instruction {
    int op_code;
    Add* end_1;
    Add* end_2;
    Add* end_3;
};

struct adress {
    int add_block; //end bloco
    int add_word;  //end palavra
};

Inst* cria_inst(){
	Inst* inst = malloc(sizeof(Inst));
	inst->end_1 = cria_add(sizeof(Add));
	inst->end_2 = cria_add(sizeof(Add));
	inst->end_3 = cria_add(sizeof(Add));

	return inst;
}

void set_opcode(Inst* inst, int opcode){
	inst->op_code = opcode;
}
void set_end1(Inst* inst, int bloco, int palavra){
	set_block(inst->end_1, bloco);
	set_word(inst->end_1, palavra);
}
void set_end2(Inst* inst, int bloco, int palavra){
	set_block(inst->end_2, bloco);
	set_word(inst->end_2, palavra);
}
void set_end3(Inst* inst, int bloco, int palavra){
	set_block(inst->end_3, bloco);
	set_word(inst->end_3, palavra);
}

int get_opcode(Inst* inst){
	return inst->op_code;
}
int get_end1_block(Inst* inst){
	return get_block_add(inst->end_1);
}
int get_end1_word(Inst* inst){
	return get_word_add(inst->end_1);
}
int get_end2_block(Inst* inst){
	return get_block_add(inst->end_2);
}
int get_end2_word(Inst* inst){
	return get_word_add(inst->end_2);
}
int get_end3_block(Inst* inst){
	return get_block_add(inst->end_3);
}
int get_end3_word(Inst* inst){
	return get_word_add(inst->end_3);
}

void libera_inst(Inst* inst){
	libera_add(inst->end_1);
	libera_add(inst->end_2);
	libera_add(inst->end_3);
}


Add* cria_add(){
	Add* add = malloc(sizeof(Add));

	return add;
}

void set_block(Add* add, int block){
	add->add_block = block;
}
void set_word(Add* add, int word){
	add->add_word = word;
}

int get_block_add(Add* add){
	return add->add_block;
}
int get_word_add(Add* add){
	return add->add_word;
}

void libera_add(Add* add){
	free(add);
}