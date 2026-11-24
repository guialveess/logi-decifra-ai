#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conquistas.h"

/* ═══════════════ Conquistas (US23) ═══════════════ */

typedef struct {
    const char *chave;      
    const char *nome;
    const char *descricao;
} Conquista;

static const Conquista tabela[CQ_TOTAL] = {
    { "primeiro_acerto", "Primeiro Acerto", "acertou o primeiro desafio" },
    { "triplete",        "Triplete",        "3 acertos seguidos"         },
    { "sequencia_5",     "Imparavel",       "5 acertos seguidos"         },
    { "centena",         "Centena",         "100 pontos numa partida"    },
    { "poupador",        "Poupador",        "juntou 50 moedas"           }
};

static int g_tem[CQ_TOTAL];
static int g_carregado = 0;

static int caminho(char *buf, size_t n) {
    const char *home = getenv("HOME");
    if (!home || !*home) home = getenv("USERPROFILE");
    if (!home || !*home) return -1;
    snprintf(buf, n, "%s/.decifra_conquistas", home);
    return 0;
}

static void carregar(void) {
    char path[512], linha[64];
    FILE *f;
    int i;

    memset(g_tem, 0, sizeof(g_tem));
    g_carregado = 1;
    if (caminho(path, sizeof(path)) != 0) return;
    f = fopen(path, "r");
    if (!f) return;                         /* sem arquivo: nenhuma conquista */
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\r\n")] = '\0';
        for (i = 0; i < CQ_TOTAL; i++)
            if (strcmp(linha, tabela[i].chave) == 0) g_tem[i] = 1;
    }
    fclose(f);
}

static void salvar(void) {
    char path[512];
    FILE *f;
    int i;
    if (caminho(path, sizeof(path)) != 0) return;
    f = fopen(path, "w");
    if (!f) return;
    for (i = 0; i < CQ_TOTAL; i++)
        if (g_tem[i]) fprintf(f, "%s\n", tabela[i].chave);
    fclose(f);
}

void conquistas_recarregar(void) { carregar(); }

const char *conquista_nome(int id) {
    return (id >= 0 && id < CQ_TOTAL) ? tabela[id].nome : "";
}

const char *conquista_descricao(int id) {
    return (id >= 0 && id < CQ_TOTAL) ? tabela[id].descricao : "";
}

int conquista_tem(int id) {
    if (!g_carregado) carregar();
    return (id >= 0 && id < CQ_TOTAL) ? g_tem[id] : 0;
}

int conquista_desbloquear(int id) {
    if (id < 0 || id >= CQ_TOTAL) return 0;
    if (!g_carregado) carregar();
    if (g_tem[id]) return 0;
    g_tem[id] = 1;
    salvar();
    return 1;
}

int conquistas_verificar(int acertos, int sequencia, int pontuacao, int moedas, int *novas) {
    int n = 0;
    if (acertos   >= 1   && conquista_desbloquear(CQ_PRIMEIRO_ACERTO)) novas[n++] = CQ_PRIMEIRO_ACERTO;
    if (sequencia >= 3   && conquista_desbloquear(CQ_TRIPLETE))        novas[n++] = CQ_TRIPLETE;
    if (sequencia >= 5   && conquista_desbloquear(CQ_SEQUENCIA_5))     novas[n++] = CQ_SEQUENCIA_5;
    if (pontuacao >= 100 && conquista_desbloquear(CQ_CENTENA))         novas[n++] = CQ_CENTENA;
    if (moedas    >= 50  && conquista_desbloquear(CQ_POUPADOR))        novas[n++] = CQ_POUPADOR;
    return n;
}
