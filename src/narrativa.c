#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "narrativa.h"

/* ═══════════════ Banco de falas (US21) ═══════════════
 * Temas: vies algoritmico, opacidade de modelos e tomada de decisao
 * automatizada. Texto sem acentos, como o restante da interface. */

static const FalaLogi banco[] = {
    /* Vies algoritmico */
    { "Vies algoritmico",
      "Um modelo aprende com dados do passado. Se esses dados carregam "
      "preconceitos, a IA os repete com cara de neutralidade." },
    { "Vies algoritmico",
      "Uma regra pode ser logicamente valida e ainda partir de uma premissa "
      "falsa. Dados enviesados sao essa premissa: o raciocinio funciona, a "
      "conclusao nao." },
    { "Vies algoritmico",
      "Sistemas de reconhecimento facial ja erraram muito mais com pessoas "
      "de pele escura por terem sido treinados com poucas imagens delas. "
      "Quem fica fora dos dados fica fora da precisao." },
    { "Vies algoritmico",
      "Apagar a coluna 'genero' nao elimina o vies: CEP, nome ou historico "
      "de compras podem funcionar como variaveis substitutas." },

    /* Opacidade de modelos */
    { "Opacidade de modelos",
      "Voce acabou de seguir cada passo do seu raciocinio. Muitos modelos de "
      "IA nao conseguem: bilhoes de parametros geram uma resposta sem uma "
      "explicacao legivel." },
    { "Opacidade de modelos",
      "Uma caixa-preta acerta ou erra sem dizer por que. Quando nao da para "
      "auditar o raciocinio, fica dificil contestar a decisao." },
    { "Opacidade de modelos",
      "Uma tabela-verdade lista todos os casos possiveis. Um modelo com "
      "milhares de variaveis tem combinacoes demais para testar uma a uma. "
      "Por isso explicabilidade virou area de pesquisa." },
    { "Opacidade de modelos",
      "Confianca alta nao e o mesmo que estar certo. Um modelo pode dar 99% "
      "de certeza para uma resposta errada, e sem transparencia ninguem "
      "percebe." },

    /* Tomada de decisao automatizada */
    { "Decisao automatizada",
      "Algoritmos ja aprovam credito, filtram curriculos e priorizam "
      "atendimentos. Uma regra mal escrita, aplicada a milhoes de pessoas, "
      "vira um erro em escala." },
    { "Decisao automatizada",
      "SE renda < X ENTAO negar credito. Parece logica pura, mas quem "
      "escolheu o X? Toda regra automatica esconde decisoes humanas." },
    { "Decisao automatizada",
      "A LGPD garante o direito de pedir revisao de decisoes tomadas apenas "
      "por tratamento automatizado. Em decisoes que afetam pessoas, um "
      "humano deve poder revisar." },
    { "Decisao automatizada",
      "Automatizar nao tira a responsabilidade de ninguem. Se o sistema "
      "erra, alguem o projetou, treinou ou escolheu usa-lo. Quem responde "
      "por essa decisao?" },

    /* Fechamento: pensamento critico */
    { "Pensamento critico",
      "Pensar criticamente e o que voce faz aqui: checar cada premissa antes "
      "de aceitar a conclusao. Faca o mesmo com respostas de IA, inclusive "
      "as minhas." }
};

#define TOTAL_FALAS ((int)(sizeof(banco) / sizeof(banco[0])))

/* ═══════════════ Saco embaralhado (sem reposicao) ═══════════════ */

static int g_ordem[TOTAL_FALAS];
static int g_pos       = TOTAL_FALAS;   /* forca embaralhar na 1a chamada */
static int g_ultima    = -1;
static int g_exibidas  = 0;
static unsigned int g_seed = 0;

static unsigned int rng_next(void) {
    unsigned int x;
    if (g_seed == 0) {
        FILE *f = fopen("/dev/urandom", "rb");
        if (f) { if (fread(&g_seed, sizeof(g_seed), 1, f) != 1) g_seed = 0; fclose(f); }
        if (g_seed == 0) g_seed = (unsigned int)time(NULL) ^ ((unsigned int)getpid() << 16);
        if (g_seed == 0) g_seed = 0x9E3779B9u;
    }
    x = g_seed;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    g_seed = x;
    return x;
}

static void embaralhar(void) {
    int i, j, tmp;
    for (i = 0; i < TOTAL_FALAS; i++) g_ordem[i] = i;
    for (i = TOTAL_FALAS - 1; i > 0; i--) {          /* Fisher-Yates */
        j = (int)(rng_next() % (unsigned int)(i + 1));
        tmp = g_ordem[i]; g_ordem[i] = g_ordem[j]; g_ordem[j] = tmp;
    }
    /* Na virada do saco, nao repetir a ultima fala logo em seguida. */
    if (TOTAL_FALAS > 1 && g_ordem[0] == g_ultima) {
        tmp = g_ordem[0]; g_ordem[0] = g_ordem[1]; g_ordem[1] = tmp;
    }
    g_pos = 0;
}

int narrativa_total(void)     { return TOTAL_FALAS; }
int narrativa_exibidas(void)  { return g_exibidas; }

const FalaLogi *narrativa_proxima(void) {
    if (g_pos >= TOTAL_FALAS) embaralhar();
    g_ultima = g_ordem[g_pos++];
    g_exibidas++;
    return &banco[g_ultima];
}
