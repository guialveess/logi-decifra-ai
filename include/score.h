#ifndef SCORE_H
#define SCORE_H

#define SCORE_MAX       5
#define SCORE_NOME_MAX 40

typedef struct {
    char nome[SCORE_NOME_MAX];
    int  pontos;
} ScoreEntry;

int score_carregar(ScoreEntry *out, int max);

int score_melhor(void);

int score_inserir(const char *nome, int pontos);

#endif
