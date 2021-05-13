#include <time.h>

typedef struct block Block;

Block* cria_block();

void set_words(Block* block, int posicao, int word);
void set_add(Block* block, int posicao);
void set_hit(Block* block, int hit);
void set_empty(Block* block, int empty);
void set_insercao(Block* block, time_t insercao);

void random_words(Block* block);
void sum_hit(Block* block);
void write_empty(Block* block);
void reset_insercao(Block* block);

int get_words(Block* block, int posicao);
int* get_vet_words(Block* block);
int get_add(Block* block);
int get_hit(Block* block);
int get_empty(Block* block);
time_t get_insercao(Block* block);

void libera_block(Block* block);