#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// => Definição de estruturas
typedef struct Ponto{
    int id;
    // struct Cidade *proximaCidade;
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
    struct Aresta *proximaAresta;
} Aresta;

typedef struct Grafo{
    int quantidadePontos;
    Ponto *pontos;
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

Aresta *criarAresta(int idDestino){
    Aresta *novaAresta = malloc(sizeof(Aresta));
    if(!novaAresta){ return NULL; }

    novaAresta->idDestino = idDestino;
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
    novoPonto->ponto = ponto;
    return novoNo;
}

Grafo *criarGrafo(){
    Grafo *novoGrafo = malloc(sizeof(Grafo));
    if(!novoGrafo){ return NULL; }

    novoGrafo->pontos = NULL;
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

void carregarGrafos(Grafo **grafo){
    Grafo *novoGrafo = criarGrafo();
    if(!novoGrafo){ return; }

    (*grafo) = novoGrafo;

    FILE *pontosCSV = fopen("../data/pontos_waze.csv", "rb");
    if(!pontosCSV){ return; }

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
        
        inserirArvore(
            nomePontoGravar, 
            latitudePontoGravar, 
            longitudePontoGravar, 
            idPontoGravar, 
            &(*grafo)->raiz,
            novoPontoGravar
        );

    }
}

int main(){
    Grafo *grafo;
    carregarGrafos(&grafo);
    return 0;
}