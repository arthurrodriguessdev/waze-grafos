#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cidade{
    char nome[100];
    int identificador;
    struct Cidade *proximaCidade;
    struct Aresta *ligacoesCidades;
} Cidade;

typedef struct Grafo{
    int quantidadeCidades;
    Cidade *cidades;
} Grafo;

typedef struct Aresta{
    float distancia;
    struct Aresta *proxima;
    struct Cidade *cidadeDestino;
} Aresta;

void iniciarGrafo(Grafo **grafo){
    *grafo = malloc(sizeof(Grafo));

    if(*grafo){
        (*grafo)->cidades = NULL;
        (*grafo)->quantidadeCidades = 0;
    }
}

Cidade *criarCidade(char *nomeCidade){
    Cidade *novaCidade = malloc(sizeof(Cidade));
    if(!novaCidade){
        return NULL;
    }

    strcpy(novaCidade->nome, nomeCidade);
    novaCidade->identificador = 1; // TO DO: Criar função para gerar incremental ou pegar da qtd de cidades
    novaCidade->ligacoesCidades = NULL;
    novaCidade->proximaCidade = NULL;
    return novaCidade;
}

int adicionarCidade(char *nomeCidade, Grafo **grafo){
    Cidade *novaCidade = criarCidade(nomeCidade);
    if(!novaCidade){
        return 0;
    }

    Cidade *base = (*grafo)->cidades;

    // Se for a primeira cidade sendo cadastrada:
    if(!base){
        (*grafo)->cidades = novaCidade;
        (*grafo)->quantidadeCidades++;
        return 1;
    }

    // Caso não seja a primeira, busca a última na lista encadeada que vai apontar para a nova
    while(base->proximaCidade != NULL){
        base = base->proximaCidade;
    }

    base->proximaCidade = novaCidade;
    (*grafo)->quantidadeCidades++;
    return 1;
}

void exibirCidades(Grafo *grafo){
    Cidade *base = grafo->cidades;
    
    if(base != NULL){
        while(base != NULL){
            printf("=> [%d] - %s\n", base->identificador, base->nome);
            base = base->proximaCidade;
        }
    }
}

int main(){
    Grafo *grafo;
    iniciarGrafo(&grafo);

    adicionarCidade("Goiânia", &grafo);
    adicionarCidade("Anápolis", &grafo);
    adicionarCidade("Aparecida de Goiânia", &grafo);
    
    exibirCidades(grafo);
    return 0;
}