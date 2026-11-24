#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

static int g_modo = SCORE_MODO_BASICO;

void score_definir_modo(int modo) { g_modo = modo; }

const char *score_nome_modo(int modo) {
    switch (modo) {
        case SCORE_MODO_AVANCADO:      return "Avancado";
        case SCORE_MODO_TEMPO:         return "Contra-Tempo";
        case SCORE_MODO_SOBREVIVENCIA: return "Sobrevivencia";
        default:                       return "Basico";
    }
}

static int caminho_arquivo(int modo, char *buf, size_t n) {
    const char *sufixo = "";
    const char *home = getenv("HOME");
    if (!home || !*home) home = getenv("USERPROFILE");   /* fallback Windows */
    if (!home || !*home) return -1;

    if (modo == SCORE_MODO_AVANCADO)              sufixo = "_avancado";
    else if (modo == SCORE_MODO_TEMPO)            sufixo = "_tempo";
    else if (modo == SCORE_MODO_SOBREVIVENCIA)    sufixo = "_sobrevivencia";

    snprintf(buf, n, "%s/.decifra_score%s", home, sufixo);
    return 0;
}

static void limpar_nome(const char *src, char *dst) {
    int i, len;
    strncpy(dst, src, SCORE_NOME_MAX - 1);
    dst[SCORE_NOME_MAX - 1] = '\0';
    for (i = 0; dst[i]; i++)
        if (dst[i] == '|' || dst[i] == '\n' || dst[i] == '\r') dst[i] = ' ';
    len = (int)strlen(dst);
    while (len > 0 && dst[len - 1] == ' ') dst[--len] = '\0';
    if (len == 0) strcpy(dst, "Anonimo");
}

void score_normalizar_nome(const char *src, char *dst) { limpar_nome(src, dst); }

int score_carregar_modo(int modo, ScoreEntry *out, int max) {
    char path[512], linha[160];
    FILE *f;
    int n = 0, i, j;

    if (caminho_arquivo(modo, path, sizeof(path)) != 0) return 0;
    f = fopen(path, "r");
    if (!f) return 0;                       /* arquivo ausente: ranking vazio */

    while (n < max && fgets(linha, sizeof(linha), f)) {
        char *sep, *fim;
        long pts;
        size_t len = strlen(linha);

        /* Linha maior que o buffer: descarta o restante dela. */
        if (len > 0 && linha[len - 1] != '\n' && !feof(f)) {
            int c;
            while ((c = fgetc(f)) != EOF && c != '\n') { }
        }

        sep = strrchr(linha, '|');
        if (!sep) continue;                 /* linha corrompida */
        *sep = '\0';

        pts = strtol(sep + 1, &fim, 10);
        if (fim == sep + 1 || pts <= 0) continue;  /* pontuacao invalida */

        limpar_nome(linha, out[n].nome);
        out[n].pontos = (int)pts;
        n++;
    }
    fclose(f);

    for (i = 1; i < n; i++) {
        ScoreEntry tmp = out[i];
        for (j = i - 1; j >= 0 && out[j].pontos < tmp.pontos; j--)
            out[j + 1] = out[j];
        out[j + 1] = tmp;
    }
    return n;
}

int score_carregar(ScoreEntry *out, int max) {
    return score_carregar_modo(g_modo, out, max);
}

int score_melhor(void) {
    ScoreEntry lista[SCORE_MAX];
    return (score_carregar(lista, SCORE_MAX) > 0) ? lista[0].pontos : 0;
}

static int salvar(const ScoreEntry *lista, int n) {
    char path[512];
    FILE *f;
    int i;
    if (caminho_arquivo(g_modo, path, sizeof(path)) != 0) return -1;
    f = fopen(path, "w");
    if (!f) return -1;
    for (i = 0; i < n; i++)
        fprintf(f, "%s|%d\n", lista[i].nome, lista[i].pontos);
    fclose(f);
    return 0;
}

int score_inserir(const char *nome, int pontos) {
    ScoreEntry lista[SCORE_MAX + 1];
    int n, pos, i;

    if (pontos <= 0) return -1;

    n = score_carregar(lista, SCORE_MAX);
    pos = n;
    for (i = 0; i < n; i++)
        if (pontos > lista[i].pontos) { pos = i; break; }

    if (pos >= SCORE_MAX) return -1;
    if (n == SCORE_MAX) n--;

    for (i = n; i > pos; i--) lista[i] = lista[i - 1];
    limpar_nome(nome, lista[pos].nome);
    lista[pos].pontos = pontos;
    n++;

    return (salvar(lista, n) == 0) ? pos : -1;
}
