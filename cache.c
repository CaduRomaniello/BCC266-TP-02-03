#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "block.h"
#include "cache.h"
#include "define.h"

struct cache{
	Block** block;
	int size;
	int hit;
	int miss;
};

Cache* cria_cache(int tamanho){
	Cache* cache = malloc(sizeof(Cache));
	cache->block = malloc( tamanho * sizeof(Block*));
	for(int i = 0; i < tamanho; i++){
		cache->block[i] = cria_block();
		set_add(cache->block[i], -1);
		set_hit(cache->block[i], -1);
		set_insercao(cache->block[i], -1);
	}

	set_size(cache, tamanho);
	cache->hit = 0;
	cache->miss = 0;
	return cache;
}

void set_size(Cache* cache, int tamanho){
	cache->size = tamanho;
}
void set_hit_cache(Cache* cache){
	cache->hit += 1;
}
void set_miss_cache(Cache* cache){
	cache->miss += 1;
}

Block* get_block(Cache* cache, int posicao){
	return cache->block[posicao];
}
int get_hit_cache(Cache* cache){
	return cache->hit;
}
int get_miss_cache(Cache* cache){
	return cache->miss;
}

void libera_cache(Cache* cache, int tamanho){
	for(int i = 0; i < tamanho; i++){
		libera_block(cache->block[i]);
	}
	free(cache);
}