#ifndef SCORE_H
#define SCORE_H

#define SCORE_MAX       5
#define SCORE_NOME_MAX 40

#define SCORE_MODO_BASICO    0
#define SCORE_MODO_AVANCADO  1
#define SCORE_MODO_TEMPO     2
#define SCORE_MODO_SOBREVIVENCIA 3   /* US22: ranking proprio */
#define SCORE_NUM_MODOS      4

void score_definir_modo(int modo);

typedef struct {
    char nome[SCORE_NOME_MAX];
    int  pontos;
} ScoreEntry;

int score_carregar(ScoreEntry *out, int max);

int score_melhor(void);

int score_inserir(const char *nome, int pontos);

/* US22: le o ranking de um modo especifico sem alterar o modo ativo.
 * Arquivo ausente/ilegivel => 0 entradas (nunca falha). */
int score_carregar_modo(int modo, ScoreEntry *out, int max);

/* US22: nome exibivel do modo ("Basico", "Avancado", ...). */
const char *score_nome_modo(int modo);

/* US22: aplica ao nome a mesma limpeza usada ao salvar no ranking,
 * para comparar o jogador atual com as entradas gravadas. */
void score_normalizar_nome(const char *src, char *dst);

#endif
