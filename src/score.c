#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

static int g_modo = SCORE_MODO_BASICO;

void score_definir_modo(int modo) { g_modo = modo; }

static int caminho_arquivo(char *buf, size_t n) {
    const char *sufixo = "";
    const char *home = getenv("HOME");
    if (!home || !*home) home = getenv("USERPROFILE");   /* fallback Windows */
    if (!home || !*home) return -1;

    if (g_modo == SCORE_MODO_AVANCADO)   sufixo = "_avancado";
    else if (g_modo == SCORE_MODO_TEMPO) sufixo = "_tempo";

    snprintf(buf, n, "%s/.decifra_score%s", home, sufixo);
    return 0;
}

int score_carregar(ScoreEntry *out, int max) {
    char path[512], linha[160];
    FILE *f;
    int n = 0, i, j;

    if (caminho_arquivo(path, sizeof(path)) != 0) return 0;
    f = fopen(path, "r");
    if (!f) return 0;                    

    while (n < max && fgets(linha, sizeof(linha), f)) {
        char *sep = strrchr(linha, '|');
        if (!sep) continue;                 
        *sep = '\0';
        strncpy(out[n].nome, linha, SCORE_NOME_MAX - 1);
        out[n].nome[SCORE_NOME_MAX - 1] = '\0';
        out[n].pontos = atoi(sep + 1);
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

int score_melhor(void) {
    ScoreEntry lista[SCORE_MAX];
    return (score_carregar(lista, SCORE_MAX) > 0) ? lista[0].pontos : 0;
}

static int salvar(const ScoreEntry *lista, int n) {
    char path[512];
    FILE *f;
    int i;
    if (caminho_arquivo(path, sizeof(path)) != 0) return -1;
    f = fopen(path, "w");
    if (!f) return -1;
    for (i = 0; i < n; i++)
        fprintf(f, "%s|%d\n", lista[i].nome, lista[i].pontos);
    fclose(f);
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
