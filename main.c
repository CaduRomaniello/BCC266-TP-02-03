#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "block.h"
#include "cache.h"
#include "funcoes.h"
#include "define.h"
#include "instrucao.h"

//Atualizar para struct Instruction --> colocar time.h
int main(){
	int tam;
	long double custo = 0;
	setlocale(LC_ALL,"Portuguese");
	srand(time(NULL)); //Semente do gerador de números aleatórios

	Block** RAM = montar_RAM(size_ram);
	Cache* cache1 = cria_cache(size_cache1);
	Cache* cache2 = cria_cache(size_cache2);
	Cache* cache3 = cria_cache(size_cache3);
	//printa_cache(cache1, cache2, cache3);

	Inst** instrucoes = montar_instrucoes(&tam);
	
	//Programa de Somas e Subtrações Aleatórias
	printf("=== Somas e Subtracoes Aleatorias ===\n");
	time_t inicio = time(NULL);
	programaSomaSubtracao(RAM, instrucoes, cache1, cache2, cache3, &custo);
	time_t fim = time(NULL);
	for(int i = 0; i < tam + 1; i++){
		libera_inst(instrucoes[i]);
	}
	//printa_cache(cache1, cache2, cache3);

	printf("==================================\n");
	printf("==================================\n\n");
	printf("Tempo de execução: %.3f\n", difftime(fim, inicio));
	printf("Hit cache1 = %d, Miss cache1 = %d\n", get_hit_cache(cache1), get_miss_cache(cache1));
	printf("Hit cache2 = %d, Miss cache2 = %d\n", get_hit_cache(cache2), get_miss_cache(cache2));
	printf("Hit cache3 = %d, Miss cache3 = %d\n", get_hit_cache(cache3), get_miss_cache(cache3));
	printf("Custo: %.0Lf\n", custo);

	//Desalocando as memórias
	desalocarRAM(RAM, size_ram);
	libera_cache(cache1, size_cache1);
	libera_cache(cache2, size_cache2);
	libera_cache(cache3, size_cache3);

	return 0;
}