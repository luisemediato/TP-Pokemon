#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char nome[20];    //pokemon com maior nome tem 15 caracteres, então 20 são mais do que suficientes.
    float ataque;
    float defesa;
    float vida;
    char tipo[10];    //eletrico(8 char + '/0' + '/n' = 10), água, fogo, gelo e pedra
}POKEMON;    //estrutura de um pokémon

POKEMON* ler_arquivo(int *N, int *M){
    FILE *arq;
    arq = fopen("pokemon1.txt", "r"); 
    
    if(arq == NULL){    //caso o arquivo não seja encontrado ou tenha algum problema no processo de abertura
        fprintf(stdout, "Arquivo não encontrado ou permissão negada.\n");
        return NULL;
    }
    fscanf(arq, "%d %d", N, M);
    if(*N > 100 || *M > 100){     //caso no aquivo tenha mais de 100 pokémon para um treinador
        fprintf(stdout, "Treinador com mais de 100 Pokémon\n");
        fclose(arq);
        return NULL;
    }
    POKEMON *pokemon = (POKEMON *)malloc((*N + *M) * sizeof(POKEMON));    //espaço alocado dinamicamente para a leitura e armazenamento dos dados
    if(pokemon == NULL){    //caso haja erro na alocação dinâmica
        fprintf(stdout, "Erro na alocação de memória\n");
        fclose(arq);
        return NULL;
    }
    //retorna NULL sempre que há algum erro no processo de leitura

    //lê dos dados
    for(int i = 0; i < *N+*M; i++){    
        fscanf(arq, "%s %f %f %f %s", pokemon[i].nome, &pokemon[i].ataque, &pokemon[i].defesa, &pokemon[i].vida, pokemon[i].tipo); 
    }
    fclose(arq);
    
    //imprime o arquivo txt lido
    printf("%d %d\n", *N, *M);
    for(int i = 0; i < *N+*M; i++){
        printf("%s %.f %.f %.f %s\n", pokemon[i].nome, pokemon[i].ataque, pokemon[i].defesa, pokemon[i].vida, pokemon[i].tipo);
    }
    printf("\n");
    return pokemon;    //retorna o vetor da estrutura POKEMON do tipo *pokemon
}               
    //Os tipos são eletrico, agua, fogo, gelo e pedra:
int vigoridade_da_luta(POKEMON *pokemon, int t1, int t2){
    if(strcmp(pokemon[t1].tipo, "eletrico") == 0 && strcmp(pokemon[t2].tipo, "agua")==0) return 1;
    if(strcmp(pokemon[t2].tipo, "eletrico") == 0 && strcmp(pokemon[t1].tipo, "agua")==0) return 2;
    if(strcmp(pokemon[t1].tipo, "agua") == 0 && strcmp(pokemon[t2].tipo, "fogo")==0) return 1;
    if(strcmp(pokemon[t2].tipo, "agua") == 0 && strcmp(pokemon[t1].tipo, "fogo")==0) return 2;
    if(strcmp(pokemon[t1].tipo, "fogo") == 0 && strcmp(pokemon[t2].tipo, "gelo")==0) return 1;
    if(strcmp(pokemon[t2].tipo, "fogo") == 0 && strcmp(pokemon[t1].tipo, "gelo")==0) return 2;
    if(strcmp(pokemon[t1].tipo, "gelo") == 0 && strcmp(pokemon[t2].tipo, "pedra")==0) return 1;
    if(strcmp(pokemon[t2].tipo, "gelo") == 0 && strcmp(pokemon[t1].tipo, "pedra")==0) return 2;
    if(strcmp(pokemon[t1].tipo, "pedra") == 0 && strcmp(pokemon[t2].tipo, "eletrico")==0) return 1;
    if(strcmp(pokemon[t2].tipo, "pedra") == 0 && strcmp(pokemon[t1].tipo, "eletrico")==0) return 2;
    else return 0;  //retorna 0 se não há vantagem, 1 se t1 tem vantagem e 2 se t2 tem vantagem
}
int luta_pokemon(int turno, int t1, int t2, POKEMON *pokemon){
    //armazena os valores originais de ataque
    int ataque_original_t1 = pokemon[t1].ataque;
    int ataque_original_t2 = pokemon[t2].ataque;
    //verifica a vigoridade entre os tipos dos pokémon
    int vantagem = vigoridade_da_luta(pokemon, t1, t2);
    //aplica a vigoridade, se 0 nada muda
    if(vantagem == 1){
        pokemon[t1].ataque = pokemon[t1].ataque * 1.2;
        pokemon[t2].ataque = pokemon[t2].ataque * 0.8; 
    }else if(vantagem == 2){
        pokemon[t2].ataque = pokemon[t2].ataque * 1.2;
        pokemon[t1].ataque = pokemon[t1].ataque * 0.8; 
    }
    //simula até algum pokémon ficar sem vida
    while(pokemon[t1].vida > 0 && pokemon[t2].vida > 0){
        
        //turno de t1 atacar
        if(turno == 1){
            if(pokemon[t2].defesa >= pokemon[t1].ataque){ 
                pokemon[t2].vida--; 
            }else if(pokemon[t2].defesa < pokemon[t1].ataque){
                pokemon[t2].vida -= (pokemon[t1].ataque - pokemon[t2].defesa);
            }
            turno ++;    //turno passa a ser igual a 2, então t2 ataca
        }
        if(pokemon[t2].vida <=0 )break; //verifica se t2 ainda tem vida e interrompe o loop caso não tenha
        
        //turno de t2 atacar
        if(turno == 2){
            if(pokemon[t1].defesa >= pokemon[t2].ataque){ 
                pokemon[t1].vida--; 
            }else if(pokemon[t1].defesa < pokemon[t2].ataque){
                pokemon[t1].vida -= (pokemon[t2].ataque - pokemon[t1].defesa);
            }
            turno --;    //turno passa a ser igual a 1, então t1 ataca
        }
    }
    //restaura os valores originais de ataque
    pokemon[t1].ataque = ataque_original_t1;
    pokemon[t2].ataque = ataque_original_t2;
    return turno;    //retorna qual turno deve ser o próximo
}
int main() {
    POKEMON *pokemon = NULL;
    //N é o treinador 1 e M é o treinador 2
    int N, M, total;
    
    //leitura do arquivo txt
    pokemon = ler_arquivo(&N, &M);
    if (pokemon == NULL){    //se houver erro na leitura, encerra o programa
        return 1;
    }

    total = N + M;    //guarda o número total original de pokémon
    int t1 = 0;    //contador de N, a partir do primeiro pokémon do treinador 1
    int t2 = N;    //contador de M, a partir do primeiro pokémon do treinador 2

    //controle dos turnos
    int turno = 1;    //t1 começa atacando

    //simula a batalha
    while(N > 0 && M > 0){
        turno = luta_pokemon(turno, t1, t2, pokemon);    //turno é sempre atualizado após cada luta

        //Imprime quem ganhou a luta, remove um Pokémon e adiciona um no contador do treinador que perdeu, seja ele N ou M
        if(pokemon[t1].vida <= 0){
            printf("%s venceu %s\n",pokemon[t2].nome, pokemon[t1].nome);
            N--;
            t1++;
        }else if(pokemon[t2].vida <= 0){
            printf("%s venceu %s\n",pokemon[t1].nome, pokemon[t2].nome);
            M--;
            t2++;
        }
    }

    //imprime quem ganhou a batalha
    if(M == 0){
        printf("Jogador 1 venceu\n");
    }else if(N == 0){
        printf("Jogador 2 venceu\n");
    }
    printf("Pokémon sobreviventes:\n");
    for(int i=0; i < total; i++){
        if(pokemon[i].vida >0){
            printf("%s\n", pokemon[i].nome);
        }
    }
    printf("Pokémon derrotados:\n");
    for(int i = 0; i < total ; i++){
        if(pokemon[i].vida <=0){
            printf("%s\n", pokemon[i].nome);
        }
    }
    free(pokemon);//liberar a memória alocada dinamicamente
    return 0;
}
