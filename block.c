#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "block.h"
#include "define.h"

//ATUALIZAÇÂO

struct block{
	int words[4];
	int add;
	int hit;
	int empty;
	time_t insercao;
};

Block* cria_block(){
	Block* block = malloc(sizeof(Block));
	block->hit = 0;
	block->empty = 0;
	return block;
}

void set_words(Block* block, int posicao, int word){
	block->words[posicao] = word;
}
void set_add(Block* block, int posicao){
	block->add = posicao;
}
void set_hit(Block* block, int hit){
	block->hit = hit;
}
void set_empty(Block* block, int empty){
	block->empty = empty;
}
void set_insercao(Block* block, time_t insercao){
	block->insercao = insercao;
}

void random_words(Block* block){
	for(int i = 0; i < 4; i++){
		block->words[i] = (rand() % 100) + 1;
	}
}
void sum_hit(Block* block){
	block->hit += 1;
}
void write_empty(Block* block){
	if(block->empty == 0){
		block->empty = 1;
	}
}
void reset_insercao(Block* block){
	block->insercao = time(NULL);
}

int get_words(Block* block, int posicao){
	return block->words[posicao];
}
int* get_vet_words(Block* block){
	return block->words;
}
int get_add(Block* block){
	return block->add;
}
int get_hit(Block* block){
	return block->hit;
}
int get_empty(Block* block){
	return block->empty;
}
time_t get_insercao(Block* block){
	return block->insercao;
}

void libera_block(Block* block){
	free(block);
}