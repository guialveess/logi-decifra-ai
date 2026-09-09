#ifndef GAME_H
#define GAME_H

#define NUM_DESAFIOS 5
#define NOME_MAX 64

typedef struct {
    char enunciado[256];
    char resposta_correta;
    char explicacao[256];
    int nivel;
} Desafio;

typedef struct {
    char nome[NOME_MAX];
    int pontuacao;
    int acertos;
} Jogador;

extern Desafio desafios[NUM_DESAFIOS];
extern Jogador jogador;

void inicializar_jogo(void);
void executar_desafio(int indice);
void finalizar_jogo(void);

#endif
