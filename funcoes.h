#include "cache.h"
#include "block.h"
#include "instrucao.h"

Block** montar_RAM(int tamanho);
int** montarMemoriaSomaSub();
void programaSomaSubtracao(Block** RAM, Inst** inst, Cache* cache1, Cache* cache2, Cache* cache3, long double* custo);
void desalocarRAM(Block** RAM, int tamanho);
void desalocarMemoria(int** memoria);

int* MMU(Cache* cache1, Cache* cache2, Cache* cache3, Block** RAM, int end, long double* custo);
void saving_operation(Cache* cache1, Cache* cache2, Cache* cache3, Block** RAM, int end, int word, int resultado, long double* custo);
void sobe_da_cache2(Cache* cache1, Cache* cache2, int posicao);
void sobe_da_cache3(Cache* cache1, Cache* cache2, Cache* cache3, int posicao);
void sobe_da_RAM(Cache* cache1, Cache* cache2, Cache* cache3, Block** RAM, int posicao);

void copia_block(Block* block_original, Block* block_copia);
void printa_cache(Cache* cache1, Cache* cache2, Cache* cache3);

Inst** montar_instrucoes(int* tam);