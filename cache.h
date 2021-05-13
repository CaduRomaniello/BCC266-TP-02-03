#include "block.h"

typedef struct cache Cache;

Cache* cria_cache(int tamanho);

void set_size(Cache* cache, int tamanho);
void set_hit_cache(Cache* cache);
void set_miss_cache(Cache* cache);

Block* get_block(Cache* cache, int posicao);
int get_hit_cache(Cache* cache);
int get_miss_cache(Cache* cache);

void libera_cache(Cache* cache, int tamanho);