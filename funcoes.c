#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include "funcoes.h"
#include "block.h"
#include "cache.h"
#include "define.h"
#include "instrucao.h"

Block** montar_RAM(int tamanho){
	Block** RAM;
	RAM = malloc(tamanho * sizeof(Block*));
	for (int i = 0; i < tamanho; i++){
		RAM[i] = cria_block();
		random_words(RAM[i]);
		set_add(RAM[i], i);
	}
	return RAM;
}
int** montarMemoriaSomaSub(){
	int** memoriaSomaSub;

	//Alocando memória
	memoriaSomaSub = malloc(100 * sizeof(int*));
	for(int i = 0 ; i < 100 ; i++){
		memoriaSomaSub[i] = malloc(7 * sizeof(int));
	}

	//Gerando instruções de somar e subtrair aleatórias
	for(int i = 0 ; i < 99; i++){
		memoriaSomaSub[i][0] = rand() % 2;    		//opcode
		memoriaSomaSub[i][1] = rand() % size_ram;   //endereço 1
		memoriaSomaSub[i][2] = rand() % 4;    		//word 1
		memoriaSomaSub[i][3] = rand() % size_ram;   //endereco 2
		memoriaSomaSub[i][4] = rand() % 4;    		//word 2
		memoriaSomaSub[i][5] = rand() % size_ram;   //endereco 3
		memoriaSomaSub[i][6] = rand() % 4;    		//word 3
	}
		
	//Instrução de Halt
	memoriaSomaSub[99][0] = -1;
	memoriaSomaSub[99][1] = -1;
	memoriaSomaSub[99][2] = -1;
	memoriaSomaSub[99][3] = -1;
	memoriaSomaSub[99][4] = -1;
	memoriaSomaSub[99][5] = -1;
	memoriaSomaSub[99][6] = -1;

	return memoriaSomaSub;
}
void programaSomaSubtracao(Block** RAM, Inst** inst, Cache* cache1, Cache* cache2, Cache* cache3, long double* custo){
	//0 => opcode => Somar
	//1 => opcode => Subtrair
	//2 => opcode => Levar um valor para um enderço da RAM ("Salvar")
	//-1 => Halt

	//Registradores
	int pc = 0;                                  //Program Counter
	int opcode = -2;                             //Anulando o primeiro Opcode para entrar no loop
	int end1, end2, end3; 
	int word1, word2, word3;                     
	int* conteudoRam1;							 
	int aux1[4];			
	int* conteudoRam2; 							 
	int aux2[4];  						 
	int resultado=0; 
	int cont = -1;                            	 

	while(opcode!=-1){ //Enquanto não for Halt...
		//sleep(1);
		cont++;
		opcode =  get_opcode(inst[cont]);

		switch(opcode){
			case 1: //Somar
				end1  = get_end1_block(inst[cont]);
				word1 = get_end1_word(inst[cont]);
				end2  = get_end2_block(inst[cont]);
				word2 = get_end2_word(inst[cont]);
				end3  = get_end3_block(inst[cont]);
				word3 = get_end3_word(inst[cont]);

				//Buscando os dados na cache
				conteudoRam1 = MMU(cache1, cache2, cache3, RAM, end1, custo);
				for(int i = 0; i < 4; i++){
					aux1[i] = conteudoRam1[i];
				}
				conteudoRam2 = MMU(cache1, cache2, cache3, RAM, end2, custo);
				for(int i = 0; i < 4; i++){
					aux2[i] = conteudoRam2[i];
				}

				//Salvando o resultado na RAM
				resultado = aux1[word1] + aux2[word2];
				saving_operation(cache1, cache2, cache3, RAM, end3, word3, resultado, custo);
				
				//printf("Somando: %d + %d = %d\n", aux1[word1], aux2[word2], resultado);
				break;
			
			case 2: //Subtrair
				end1  = get_end1_block(inst[cont]);
				word1 = get_end1_word(inst[cont]);
				end2  = get_end2_block(inst[cont]);
				word2 = get_end2_word(inst[cont]);
				end3  = get_end3_block(inst[cont]);
				word3 = get_end3_word(inst[cont]);

				//Buscando os dados na RAM
				conteudoRam1 = MMU(cache1, cache2, cache3, RAM, end1, custo);
				for(int i = 0; i < 4; i++){
					aux1[i] = conteudoRam1[i];
				}
				conteudoRam2 = MMU(cache1, cache2, cache3, RAM, end2, custo);
				for(int i = 0; i < 4; i++){
					aux2[i] = conteudoRam2[i];
				}
				
				//Salvando o resultado na RAM
				resultado = aux1[word1] - aux2[word2];
				saving_operation(cache1, cache2, cache3, RAM, end3, word3, resultado, custo);
					
				//printf("Subtraindo: %d - %d = %d\n", aux1[word1], aux2[word2], resultado);
				break;
		}	
		pc++;
	}	
}
void desalocarRAM(Block** RAM, int tamanho){
	for (int i = 0; i < tamanho; i++){
		libera_block(RAM[i]);
	}
	free(RAM);
}
void desalocarMemoria(int** memoria){
	for(int i = 0 ; i < size_ram ; i++){
		free(memoria[i]);
	}
	free(memoria);
}

int* MMU(Cache* cache1, Cache* cache2, Cache* cache3, Block** RAM, int end, long double* custo){
	//printa_cache(cache1, cache2, cache3);

	int aux_cache2[4];
	int aux_cache3[4];
	int aux_RAM[4];
	int* retorno_cache2;
	int* retorno_cache3;
	int* retorno_RAM;
	int posicao;

	//procurando o end na cache 1
	*custo += custo_cache1;
	for (int i = 0; i < size_cache1; i++){
		if (end == get_add(get_block(cache1, i))){
			reset_insercao(get_block(cache1, i));
			sum_hit(get_block(cache1, i));
			write_empty(get_block(cache1, i));
			set_hit_cache(cache1);
			return get_vet_words(get_block(cache1, i));
		}
	}

	set_miss_cache(cache1);

	//procurando o end na cache 2
	*custo += custo_cache2;
	for (int i = 0; i < size_cache2; i++){
		if (end == get_add(get_block(cache2, i))){
			reset_insercao(get_block(cache2, i));
			posicao = i;
			sum_hit(get_block(cache2, i));
			write_empty(get_block(cache2, i));
			set_hit_cache(cache2);
			for(int j = 0; j < 4; j++){
				aux_cache2[j] = get_words(get_block(cache2, i), j);
			}
			retorno_cache2 = aux_cache2;
			sobe_da_cache2(cache1, cache2, posicao);
			return retorno_cache2;
		}
	}

	set_miss_cache(cache2);

	//procurando o end na cache 3
	*custo += custo_cache3;
	for (int i = 0; i < size_cache3; i++){
		if (end == get_add(get_block(cache3, i))){
			reset_insercao(get_block(cache3, i));
			posicao = i;
			sum_hit(get_block(cache3, i));
			write_empty(get_block(cache3, i));
			set_hit_cache(cache3);
			for(int j = 0; j < 4; j++){
				aux_cache3[j] = get_words(get_block(cache3, i), j);
			}
			retorno_cache3 = aux_cache3;
			sobe_da_cache3(cache1, cache2, cache3, posicao);
			return retorno_cache3;
		}
	}

	set_miss_cache(cache3);

	//procurando o end na RAM
	*custo += custo_RAM;
	for (int i = 0; i < size_ram; i++){
		if (end == get_add(RAM[i])){
			reset_insercao(RAM[i]);
			posicao = i;
			sum_hit(RAM[i]);
			for(int j = 0; j < 4; j++){
				aux_RAM[j] = get_words(RAM[i], j);
			}
			retorno_RAM = aux_RAM;
			sobe_da_RAM(cache1, cache2, cache3, RAM, posicao);
			return retorno_RAM;
		}
	}

	return NULL;
}
void saving_operation(Cache* cache1, Cache* cache2, Cache* cache3, Block** RAM, int end, int word, int resultado, long double* custo){
	int posicao;

	//procurando o end na cache 1
	*custo += custo_cache1;
	for (int i = 0; i < size_cache1; i++){
		if (end == get_add(get_block(cache1, i))){
			reset_insercao(get_block(cache1, i));
			sum_hit(get_block(cache1, i));
			write_empty(get_block(cache1, i));
			set_hit_cache(cache1);
			set_words(get_block(cache1, i), word, resultado);
			return;
		}
	}

	set_miss_cache(cache1);

	//procurando o end na cache 2
	*custo += custo_cache2;
	for (int i = 0; i < size_cache2; i++){
		if (end == get_add(get_block(cache2, i))){
			reset_insercao(get_block(cache2, i));
			posicao = i;
			sum_hit(get_block(cache2, i));
			write_empty(get_block(cache2, i));
			set_hit_cache(cache2);
			set_words(get_block(cache2, i), word, resultado);
			sobe_da_cache2(cache1, cache2, posicao);
			return;
		}
	}

	set_miss_cache(cache2);

	//procurando o end na cache 3
	*custo += custo_cache3;
	for (int i = 0; i < size_cache3; i++){
		if (end == get_add(get_block(cache3, i))){
			reset_insercao(get_block(cache3, i));
			posicao = i;
			sum_hit(get_block(cache3, i));
			write_empty(get_block(cache3, i));
			set_hit_cache(cache3);
			set_words(get_block(cache3, i), word, resultado);
			sobe_da_cache3(cache1, cache2, cache3, posicao);
			return;
		}
	}

	set_miss_cache(cache3);

	//procurando o end na RAM
	*custo += custo_RAM;
	for (int i = 0; i < size_ram; i++){
		if (end == get_add(RAM[i])){
			reset_insercao(RAM[i]);
			posicao = i;
			sum_hit(RAM[i]);
			set_words(RAM[i], word, resultado);
			sobe_da_RAM(cache1, cache2, cache3, RAM, posicao);
			return;
		}
	}
}

void sobe_da_cache2(Cache* cache1, Cache* cache2, int posicao){
	int posicao_min;
	double comparacao = INT_MIN;
	time_t saida = time(NULL);
	double tempo_bloco;
	Block* block_down = cria_block();
	Block* block_up = cria_block();

	for (int i = 0; i < size_cache1; i++){
		saida = time(NULL);
		if(comparacao < difftime(saida, get_insercao(get_block(cache1, i)))){
			comparacao = difftime(saida, get_insercao(get_block(cache1, i)));
			posicao_min = i;
		}
	}

	tempo_bloco = difftime(saida,  get_insercao(get_block(cache2, posicao)));

	if(get_hit(get_block(cache1, posicao_min)) == -1){
		copia_block(get_block(cache1, posicao_min), block_down);
		copia_block(get_block(cache2, posicao), block_up);
		copia_block(block_down, get_block(cache2, posicao));
		copia_block(block_up, get_block(cache1, posicao_min));
	}

	else{
		if(tempo_bloco <= comparacao){
			copia_block(get_block(cache1, posicao_min), block_down);
			copia_block(get_block(cache2, posicao), block_up);
			copia_block(block_down, get_block(cache2, posicao));
			copia_block(block_up, get_block(cache1, posicao_min));
		}
	}

	libera_block(block_up);
	libera_block(block_down);
}
void sobe_da_cache3(Cache* cache1, Cache* cache2, Cache* cache3, int posicao){
	int posicao_min;
	double comparacao = INT_MIN;
	time_t saida = time(NULL);
	double tempo_bloco;
	Block* block_down = cria_block();
	Block* block_up = cria_block();

	for (int i = 0; i < size_cache2; i++){
		saida = time(NULL);
		if(comparacao < difftime(saida, get_insercao(get_block(cache2, i)))){
			comparacao = difftime(saida, get_insercao(get_block(cache2, i)));
			posicao_min = i;
		}
	}

	tempo_bloco = difftime(saida,  get_insercao(get_block(cache3, posicao)));

	if(get_hit(get_block(cache2, posicao_min)) == -1){
		copia_block(get_block(cache2, posicao_min), block_down);
		copia_block(get_block(cache3, posicao), block_up);
		copia_block(block_down, get_block(cache3, posicao));
		copia_block(block_up, get_block(cache2, posicao_min));
		sobe_da_cache2(cache1, cache2, posicao_min);
	}
	else{
		if(tempo_bloco <= comparacao){
			copia_block(get_block(cache2, posicao_min), block_down);
			copia_block(get_block(cache3, posicao), block_up);
			copia_block(block_down, get_block(cache3, posicao));
			copia_block(block_up, get_block(cache2, posicao_min));
			sobe_da_cache2(cache1, cache2, posicao_min);
		}
	}

	libera_block(block_up);
	libera_block(block_down);
}
void sobe_da_RAM(Cache* cache1, Cache* cache2, Cache* cache3, Block** RAM, int posicao){
	int posicao_min;
	double comparacao = INT_MIN;
	time_t saida = time(NULL);
	double tempo_bloco;
	Block* block_down = cria_block();
	Block* block_up = cria_block();

	for (int i = 0; i < size_cache3; i++){
		saida = time(NULL);
		if(comparacao < difftime(saida, get_insercao(get_block(cache3, i)))){
			comparacao = difftime(saida, get_insercao(get_block(cache3, i)));
			posicao_min = i;
		}
	}
	tempo_bloco = difftime(saida,  get_insercao(RAM[posicao]));

	if(get_hit(get_block(cache3, posicao_min)) == -1){
		copia_block(RAM[posicao], block_up);
		copia_block(block_up, get_block(cache3, posicao_min));
		sobe_da_cache3(cache1, cache2, cache3, posicao_min);
	}
	else{	
		if(tempo_bloco <= comparacao){
			copia_block(get_block(cache3, posicao_min), block_down);
			copia_block(RAM[posicao], block_up);
			copia_block(block_down, RAM[posicao]);
			copia_block(block_up, get_block(cache3, posicao_min));
			sobe_da_cache3(cache1, cache2, cache3, posicao_min);
		}
	}

	libera_block(block_up);
	libera_block(block_down);
}

void copia_block(Block* block_original, Block* block_copia){
	for(int i = 0; i < 4; i++){
		set_words(block_copia, i, get_words(block_original, i));
	}

	set_add(block_copia, get_add(block_original));
	set_hit(block_copia, get_hit(block_original));
	set_empty(block_copia, get_empty(block_original));
	set_insercao(block_copia, get_insercao(block_original));
}

void printa_cache(Cache* cache1, Cache* cache2, Cache* cache3){
	time_t saida = time(NULL);
	printf("====================================================\n");
	printf("====================================================\n\n");

	printf("****Cache 1****\n\n");
	for(int i = 0; i < size_cache1; i++){
		printf(" | %5d | ", get_add(get_block(cache1, i)));
	}
	printf("\n");
	for(int i = 0; i < size_cache1; i++){
		if(get_add(get_block(cache1, i)) == -1){
			printf(" | %5d | ", -1);
		}
		else{
			printf(" | %5.2lf | ", difftime(saida, get_insercao(get_block(cache1, i))));
		}
	}
	printf("\n\n");

	printf("****Cache 2****\n\n");
	for(int i = 0; i < size_cache2; i++){
		printf(" | %5d | ", get_add(get_block(cache2, i)));
	}
	printf("\n");
	for(int i = 0; i < size_cache2; i++){
		if(get_add(get_block(cache2, i)) == -1){
			printf(" | %5d | ", -1);
		}
		else{
			printf(" | %5.2lf | ", difftime(saida, get_insercao(get_block(cache2, i))));
		}
	}
	printf("\n\n");

	printf("****Cache 3****\n\n");
	for(int i = 0; i < size_cache3; i++){
		printf(" | %5d | ", get_add(get_block(cache3, i)));
	}
	printf("\n");
	for(int i = 0; i < size_cache3; i++){
		if(get_add(get_block(cache3, i)) == -1){
			printf(" | %5d | ", -1);
		}
		else{
			printf(" | %5.2lf | ", difftime(saida, get_insercao(get_block(cache3, i))));
		}
	}
	printf("\n\n");
}

Inst** montar_instrucoes(int* tam){
	int opcode;
	int end1, end2, end3;
	int word1, word2, word3;
	Inst** inst;
	FILE* entrada = fopen("instructions.txt", "r");

	fscanf(entrada, "%d", tam);
	inst = malloc(((*tam) + 1) * sizeof(Inst*));
	for(int i = 0; i < (*tam); i++){
		inst[i] = cria_inst();

		fscanf(entrada, "%d", &opcode);
		fscanf(entrada, "%d %d", &end1, &word1);
		fscanf(entrada, "%d %d", &end2, &word2);
		fscanf(entrada, "%d %d", &end3, &word3);

		set_opcode(inst[i], opcode);
		set_end1(inst[i], end1, word1);
		set_end2(inst[i], end2, word2);
		set_end3(inst[i], end3, word3);
	}

	inst[*tam] = cria_inst();
	set_opcode(inst[*tam], -1);
	set_end1(inst[*tam], -1, -1);
	set_end2(inst[*tam], -1, -1);
	set_end3(inst[*tam], -1, -1);

	fclose(entrada);
	return inst;
}