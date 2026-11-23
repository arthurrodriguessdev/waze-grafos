#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// => Definição de estruturas
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

// => Funções utilitárias (criação de objetos)
Cidade *criarCidade(char *nomeCidade, int indiceCidade){
    Cidade *novaCidade = malloc(sizeof(Cidade));
    if(!novaCidade){
        return NULL;
    }

    strcpy(novaCidade->nome, nomeCidade);
    novaCidade->identificador = indiceCidade;
    novaCidade->ligacoesCidades = NULL;
    novaCidade->proximaCidade = NULL;
    return novaCidade;
}

Aresta *criarAresta(float distancia, Cidade *cidadeDestino){
    Aresta *novaAresta = malloc(sizeof(Aresta));
    if(!novaAresta){ return NULL; }

    novaAresta->cidadeDestino = cidadeDestino;
    novaAresta->distancia = distancia;
    novaAresta->proxima = NULL;
    return novaAresta;
}

void iniciarGrafo(Grafo **grafo){
    *grafo = malloc(sizeof(Grafo));

    if(*grafo){
        (*grafo)->cidades = NULL;
        (*grafo)->quantidadeCidades = 0;
    }
}

int adicionarCidade(char *nomeCidade, Grafo **grafo){
    Cidade *novaCidade = criarCidade(nomeCidade, (*grafo)->quantidadeCidades);
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

// Aresta não direcionada (vai e volta)
int adicionarAresta(Grafo **grafo, int idCidadeOrigem, int idCidadeDestino, float distancia){
    if(!(*grafo)){ return 0; }
    if(idCidadeDestino == idCidadeOrigem){ return 0; }

    Cidade *cidadeBase = (*grafo)->cidades;
    if(!cidadeBase){ return 0; }

    Cidade *cidadeOrigem = NULL, *cidadeDestino = NULL;
    while(cidadeBase != NULL){
        if(cidadeBase->identificador == idCidadeOrigem){
            cidadeOrigem = cidadeBase;
        }

        if(cidadeBase->identificador == idCidadeDestino){
            cidadeDestino = cidadeBase;
        }

        cidadeBase = cidadeBase->proximaCidade;
    }

    // Encontrou as duas cidades (foram adicionadas)
    if(cidadeOrigem && cidadeDestino){
        Aresta *arestaOrigemParaDestino = criarAresta(distancia, cidadeDestino);
        Aresta *arestaDestinoParaOrigem = criarAresta(distancia, cidadeOrigem);

        // Se a criação das arestas funcionaram
        if(arestaDestinoParaOrigem && arestaOrigemParaDestino){
            Aresta *base = NULL;

            // Origem -> Destino
            if(!cidadeOrigem->ligacoesCidades){
                cidadeOrigem->ligacoesCidades = arestaOrigemParaDestino;
            } else{
                base = cidadeOrigem->ligacoesCidades;
                while(base->proxima != NULL){
                    base = base->proxima;
                }

                base->proxima = arestaOrigemParaDestino;
            }

            // Destino -> Origem
            if(!cidadeDestino->ligacoesCidades){
                cidadeDestino->ligacoesCidades = arestaDestinoParaOrigem;
            } else{
                base = cidadeDestino->ligacoesCidades;
                while(base->proxima != NULL){
                    base = base->proxima;
                }

                base->proxima = arestaDestinoParaOrigem;
            }

            return 1;
        }
    }
    
    return 0;
}

void exibirCaminhos(Grafo *grafo){
    if(!grafo || !grafo->cidades){
        return;
    }

    Cidade *cidadeBase = grafo->cidades;
    while(cidadeBase != NULL){
        Aresta *arestaBase = cidadeBase->ligacoesCidades;

        if(arestaBase){
            printf("\nCAMINHOS DA CIDADE: %s\n", cidadeBase->nome);
            while(arestaBase != NULL){
                printf("- %s (%.2f km)\n",
                    arestaBase->cidadeDestino->nome,
                    arestaBase->distancia
                );

                arestaBase = arestaBase->proxima;
            }

        } else{
            printf("\nSEM CAMINHOS PARA A CIDADE: %s\n", cidadeBase->nome);
        }

        cidadeBase = cidadeBase->proximaCidade;
    }
}

int main(){
    Grafo *grafo;
    iniciarGrafo(&grafo);

    adicionarCidade("Goiânia", &grafo);
    adicionarCidade("Anápolis", &grafo);
    adicionarCidade("Aparecida de Goiânia", &grafo);

    adicionarAresta(&grafo, 0, 1, 800);
    
    exibirCidades(grafo);
    exibirCaminhos(grafo);
    return 0;
}