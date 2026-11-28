#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Ponto Ponto;
typedef struct Aresta Aresta;

// => Definição de estruturas
typedef struct Ponto{
    int id;
    struct Aresta *ligacoesCidades;
} Ponto;

typedef struct No{
    char nome[50];
    struct No *esquerda;
    struct No *direita;
    double latitude;
    double longitude;
    int id;
    Ponto* ponto;
} No;

typedef struct Aresta{
    int idDestino;
    struct Ponto *destino;
    struct Aresta *proximaAresta;
} Aresta;

typedef struct Grafo{
    int quantidadePontos;
    // Ponto *pontos;
    No *raiz;
} Grafo;

// => Funções utilitárias (criação de objetos)
Ponto *criarPonto(int id){
    Ponto *novoPonto = malloc(sizeof(Ponto));
    if(!novoPonto){ return NULL; }

    novoPonto->id = id;
    novoPonto->ligacoesCidades = NULL;
    return novoPonto;
}

Aresta *criarAresta(Ponto* destino){
    Aresta *novaAresta = malloc(sizeof(Aresta));
    if(!novaAresta){ return NULL; }

    novaAresta->destino = destino;
    novaAresta->idDestino = novaAresta->destino->id;
    novaAresta->proximaAresta = NULL;
    return novaAresta;
}

No *criarNo(char nome[50], double latitude, double longitude, int id, Ponto* ponto){
    No *novoNo = malloc(sizeof(No));
    if(!novoNo){ return NULL; }

    strcpy(novoNo->nome, nome);
    novoNo->latitude = latitude;
    novoNo->longitude = longitude;
    novoNo->id = id;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->ponto = ponto;
    return novoNo;
}

Grafo *criarGrafo(){
    Grafo *novoGrafo = malloc(sizeof(Grafo));
    if(!novoGrafo){ return NULL; }

    // novoGrafo->pontos = NULL;
    novoGrafo->quantidadePontos = 0;
    novoGrafo->raiz = NULL;
    return novoGrafo;
}

// => Funções relacionadas às árvores
int inserirArvore(char nome[50], double latitude, double longitude, int id, No **raiz, Ponto* ponto){
    if(!(*raiz)){
        No *novoNo = criarNo(nome, latitude, longitude, id, ponto);
        if(!novoNo){ return 0; }

        (*raiz) = novoNo;
        return 1;
    }

    if(id > (*raiz)->id){
        return inserirArvore(nome, latitude, longitude, id, &(*raiz)->direita, ponto);
    }

    return inserirArvore(nome, latitude, longitude, id, &(*raiz)->esquerda, ponto);
}

No *buscarArvore(int id, No **raiz){
    if(!(*raiz)){
        return NULL;
    }

    if(id == (*raiz)->id){
        return (*raiz);
    }

    if(id > (*raiz)->id){
        return buscarArvore(id, &(*raiz)->direita);
    }

    return buscarArvore(id, &(*raiz)->esquerda);
}

void lerPrimeiraLinha(FILE *arquivo){
    char linha[255];
    fgets(linha, sizeof(linha), arquivo);
}

void adicionarArestas(Ponto* pontoOrigem, Ponto* pontoDestino){
    Aresta *novaAresta = criarAresta(pontoDestino);
    if(!novaAresta){ return; }

    if (pontoOrigem->ligacoesCidades == NULL) {
        pontoOrigem->ligacoesCidades = novaAresta;
    } else {
        Aresta* arestaBase = pontoOrigem->ligacoesCidades;
        while (arestaBase->proximaAresta != NULL){
            arestaBase = arestaBase->proximaAresta;
        }

        arestaBase->proximaAresta = novaAresta;
    }
}

void carregarArestas(Grafo *grafo){
    FILE* conexoesEntrePontos = fopen("../data/conexoes_entre_pontos.csv", "r");
    if (!conexoesEntrePontos) return;

    lerPrimeiraLinha(conexoesEntrePontos);

    int pontoIdOrigem = -1, pontoIdDestino = -1;
    while(fscanf(conexoesEntrePontos, "%d,%d", &pontoIdOrigem, &pontoIdDestino) == 2){
        No* noOrigem = buscarArvore(pontoIdOrigem, &grafo->raiz);
        No* noDestino = buscarArvore(pontoIdDestino, &grafo->raiz);

        if(noOrigem && noDestino){
            adicionarArestas(noOrigem->ponto, noDestino->ponto);
            adicionarArestas(noDestino->ponto, noOrigem->ponto);
        }
    }

    fclose(conexoesEntrePontos);
}

void carregarGrafos(Grafo **grafo){
    Grafo *novoGrafo = criarGrafo();
    if(!novoGrafo){ return; }

    (*grafo) = novoGrafo;

    FILE *pontosCSV = fopen("../data/pontos_waze.csv", "rb");
    if(!pontosCSV){ return; }

    lerPrimeiraLinha(pontosCSV);
    char linhaCSV[255];
    char *token = NULL;
    char vetorValores[4][255]; // Vai armazenar os tokens (dados divididos)
    int indiceVetor;

    int idPontoGravar;
    char nomePontoGravar[50];
    double latitudePontoGravar, longitudePontoGravar;

    while(fgets(linhaCSV, sizeof(linhaCSV), pontosCSV)){
        token = strtok(linhaCSV, ",");
        indiceVetor = 0;

        while(token){
            strcpy(vetorValores[indiceVetor], token);
            indiceVetor++;
            token = strtok(NULL, ",");
        }
        
        // Faz o casting de char para os tipos esperados na struct "Ponto"
        idPontoGravar = atoi(vetorValores[0]);
        strcpy(nomePontoGravar, vetorValores[1]);
        latitudePontoGravar = atof(vetorValores[2]);
        longitudePontoGravar = atof(vetorValores[3]);

        Ponto *novoPontoGravar = criarPonto(idPontoGravar);
        if(!novoPontoGravar){ return; }
        
        int inseriu = inserirArvore(
            nomePontoGravar, 
            latitudePontoGravar, 
            longitudePontoGravar, 
            idPontoGravar, 
            &(*grafo)->raiz,
            novoPontoGravar
        );

        if(inseriu){
            (*grafo)->quantidadePontos++;
        }
    }

    carregarArestas(*grafo);
    fclose(pontosCSV);
}

// Percorre a árvore em ordem para mostrar os pontos salvos
void mostrarPontos(No *raiz){
    if(raiz){
        mostrarPontos(raiz->esquerda);
        printf(
            "Ponto: [%d] %s\nLatitude: %.2f\nLongitude: %.2f\n", 
            raiz->id,
            raiz->nome,
            raiz->latitude,
            raiz->longitude
        );
        printf("\n");

        mostrarPontos(raiz->direita);
    }
}

// raizEstatica -> raiz que não muda (necessária para a busca)
void mostrarRotas(No *raiz, No *raizEstatica){
    Ponto *pontoInicial = NULL;
    Aresta *arestaBase = NULL;
    No *noDestino = NULL;

    if(raiz){
        mostrarRotas(raiz->esquerda, raizEstatica);

        pontoInicial = raiz->ponto;
        arestaBase = pontoInicial->ligacoesCidades;
        if(arestaBase){
            printf("Rotas do Ponto: [%d] %s\n", raiz->id, raiz->nome);

            while(arestaBase != NULL){
                noDestino = buscarArvore(arestaBase->idDestino, &raizEstatica);
                printf("-> [%d] %s\n", noDestino->id, noDestino->nome);
                arestaBase = arestaBase->proximaAresta;
            }

            printf("\n");
        }

        mostrarRotas(raiz->direita, raizEstatica);
    }
}

int main(){
    Grafo *grafo;
    carregarGrafos(&grafo);
    mostrarRotas(grafo->raiz, grafo->raiz);
    return 0;
}