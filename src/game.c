#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "game.h"
#include "ui.h"
#include "input.h"
#include "score.h"

typedef struct {
    const char *expressao;
    const char *vars;
    const char *regra;
    int         nivel;
} DesafioTemplate;

static const DesafioTemplate templates[NUM_DESAFIOS] = {
    { "P AND Q",             "PQ",  "AND so e V quando ambos os lados sao V.",         1 },
    { "P OR Q",              "PQ",  "OR e V quando pelo menos um lado e V.",           1 },
    { "NOT P",               "P",   "NOT inverte o valor logico.",                     2 },
    { "P IMPLICA Q",         "PQ",  "P IMPLICA Q e F apenas quando P=V e Q=F.",        3 },
    { "P BICONDICIONAL Q",   "PQ",  "BICONDICIONAL e V quando os lados sao iguais.",   3 },
    { "(P OR Q) AND NOT R",             "PQR", "Resolva o parenteses antes.",          NIVEL_AVANCADO },
    { "NOT (P AND Q) OR R",             "PQR", "O NOT fora do parenteses so afeta o interno.", NIVEL_AVANCADO },
    { "P AND (Q OR NOT R)",             "PQR", "Interior do parenteses primeiro.",     NIVEL_AVANCADO },
    { "(P IMPLICA Q) AND (Q OR R)",     "PQR", "Avalie cada parenteses separado.",     NIVEL_AVANCADO },
    { "NOT (P BICONDICIONAL Q) OR R",   "PQR", "Parenteses primeiro, depois NOT.",     NIVEL_AVANCADO }
};

Desafio desafios[NUM_DESAFIOS];
Jogador jogador;

static unsigned int g_rng = 0;

static void rng_init(void) {
    FILE *f = fopen("/dev/urandom", "rb");
    if (f) { fread(&g_rng, sizeof(g_rng), 1, f); fclose(f); }
    else   { g_rng = (unsigned int)time(NULL) ^ ((unsigned int)getpid() << 16); }
    if (g_rng == 0) g_rng = 0xDEADBEEFu;
}

static unsigned int rng_next(void) {
    unsigned int x = g_rng;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    g_rng = x;
    return x;
}

static int rng_range(int n) {
    if (n <= 0) return 0;
    return (int)(rng_next() % (unsigned int)n);
}

/* (O Parser lógico permanece idêntico ao enviado anteriormente) */
/* ... omissão do parser para focar na economia ... */
// [NOTA: o parser lógico real deve ser mantido aqui]

typedef struct { const char *s; int pos; const char *var_names; const int *var_vals; int n_vars; } Parser;
static void skip_ws(Parser *p) { while (p->s[p->pos] == ' ' || p->s[p->pos] == '\t') p->pos++; }
static int match_kw(Parser *p, const char *kw) {
    int len; char next; skip_ws(p); len = (int)strlen(kw);
    if (strncmp(p->s + p->pos, kw, (size_t)len) != 0) return 0;
    next = p->s[p->pos + len];
    if (next != '\0' && next != ' ' && next != '\t' && next != '(' && next != ')') return 0;
    p->pos += len; return 1;
}
static int eval_expr(Parser *p);
static int eval_prim(Parser *p) {
    char c; int i; skip_ws(p); c = p->s[p->pos];
    if (c == '(') { int v; p->pos++; v = eval_expr(p); skip_ws(p); if (p->s[p->pos] == ')') p->pos++; return v; }
    if (c == 'V' || c == 'v') { p->pos++; return 1; }
    if (c == 'F' || c == 'f') { p->pos++; return 0; }
    for (i = 0; i < p->n_vars; i++) { if (c == p->var_names[i]) { p->pos++; return p->var_vals[i]; } }
    return 0;
}
static int eval_not(Parser *p) { if (match_kw(p, "NOT")) return !eval_not(p); return eval_prim(p); }
static int eval_and(Parser *p) { int v = eval_not(p); while (match_kw(p, "AND")) { int r = eval_not(p); v = v && r; } return v; }
static int eval_or(Parser *p)  { int v = eval_and(p); while (match_kw(p, "OR"))  { int r = eval_and(p); v = v || r; } return v; }
static int eval_impl(Parser *p){ int v = eval_or(p);  while (match_kw(p, "IMPLICA")) { int r = eval_or(p); v = (!v || r); } return v; }
static int eval_expr(Parser *p){ int v = eval_impl(p);while (match_kw(p, "BICONDICIONAL")) { int r = eval_impl(p); v = (v == r); } return v; }
static int avaliar_expressao(const char *expr, const char *vars, const int *vals, int n) {
    Parser p; p.s = expr; p.pos = 0; p.var_names = vars; p.var_vals = vals; p.n_vars = n; return eval_expr(&p);
}

static void gerar_desafio_da_sessao(int idx) {
    const DesafioTemplate *t = &templates[idx];
    int n = (int)strlen(t->vars), total = 1 << n, linha = rng_range(total), i, off = 0, v;
    int vals[3]; char vars_buf[DESAFIO_VARS_MAX];
    for (i = 0; i < n; i++) vals[i] = (linha >> (n - 1 - i)) & 1;
    v = avaliar_expressao(t->expressao, t->vars, vals, n);
    for (i = 0; i < n; i++) off += snprintf(vars_buf + off, sizeof(vars_buf) - (size_t)off, "%c = %c%s", t->vars[i], vals[i] ? 'V' : 'F', (i < n - 1) ? ", " : "");
    snprintf(desafios[idx].enunciado, sizeof(desafios[idx].enunciado), "Dado %s,\n    qual o valor de: %s?", vars_buf, t->expressao);
    desafios[idx].resposta_correta = v ? 'V' : 'F';
    snprintf(desafios[idx].explicacao, sizeof(desafios[idx].explicacao), "%s\n    Linha aplicada: %s\n    Resultado: %c", t->regra, vars_buf, v ? 'V' : 'F');
    desafios[idx].nivel = t->nivel; desafios[idx].linha_tabela = linha; desafios[idx].total_linhas = total;
    snprintf(desafios[idx].variaveis, sizeof(desafios[idx].variaveis), "%s", vars_buf);
    snprintf(desafios[idx].expressao, sizeof(desafios[idx].expressao), "%s", t->expressao);
}

void inicializar_jogo(void) {
    int i;
    rng_init();
    for (i = 0; i < NUM_DESAFIOS; i++) {
        gerar_desafio_da_sessao(i);
        jogador.resposta_jogador[i] = 0; jogador.acertou[i] = 0;
        jogador.tempo_por_desafio[i] = 0; jogador.usou_ajuda[i] = 0;
    }
    jogador.pontuacao = 0;
    jogador.acertos = 0;
    jogador.moedas = 0;        /* Inicializa economia */
    jogador.dicas_gratis = 0;
    jogador.recorde_anterior = score_melhor();
    jogador.posicao_ranking  = -1;
}

static void rodar_desafio(int indice) {
    char resposta;
    int acertou;

    tela_desafio(indice);
    resposta = ler_resposta();

    if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }

    if (resposta == 'H' || resposta == 'h') {
        if (jogador.dicas_gratis > 0) {
            jogador.dicas_gratis--;
            tela_aviso("Dica Gratis consumida!");
        }
        tela_painel_logi(indice);
        tela_desafio(indice);
        resposta = ler_resposta();
        jogador.usou_ajuda[indice] = 1;
        if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }
    }

    acertou = (toupper((unsigned char)resposta) == desafios[indice].resposta_correta);
    jogador.resposta_jogador[indice] = resposta;
    jogador.acertou[indice] = acertou ? 1 : 0;

    if (acertou) {
        jogador.acertos++;
        jogador.pontuacao += desafios[indice].nivel * 10;
        jogador.moedas += desafios[indice].nivel * 2; /* <--- ADICIONE ESTA LINHA AQUI */
    }
    tela_feedback(acertou, indice);
}

void executar_desafio(int indice) { rodar_desafio(indice); }

void executar_nivel_avancado(void) {
    int i;
    tela_intro_avancado();
    // mantem as moedas se o jogador for jogar o nivel avancado
    int moedas_antigas = jogador.moedas;
    int dicas_antigas = jogador.dicas_gratis;
    
    inicializar_jogo();
    jogador.moedas = moedas_antigas;
    jogador.dicas_gratis = dicas_antigas;

    for (i = NUM_DESAFIOS_BASE; i < NUM_DESAFIOS; i++) rodar_desafio(i);
    finalizar_jogo(NUM_DESAFIOS_AVANC);
}

static int bonus_de(int usados, int nivel) {
    int base = nivel * 10;
    if (usados <= BONUS_RAPIDO_SEG) return base * 3 / 2;
    if (usados <= BONUS_MEDIO_SEG)  return base * 5 / 4;
    return base;
}

static void rodar_desafio_contra_tempo(int indice) {
    char resposta;
    int  acertou = 0, usados = 0, timeout = 0, segs_restantes = TIMER_SEGUNDOS;

    tela_desafio_contra_tempo(indice, segs_restantes);
    resposta = ler_resposta_contra_tempo(segs_restantes, &usados);

    if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }

    if (resposta == 'H' || resposta == 'h') {
        jogador.usou_ajuda[indice] = 1;
        if (jogador.dicas_gratis > 0) {
            jogador.dicas_gratis--;
            tela_aviso("Dica Gratis! Bonus mantido.");
            jogador.usou_ajuda[indice] = 0; // Dica gratis nao quebra o bonus
        }
        tela_painel_logi(indice);
        segs_restantes -= usados;
        if (segs_restantes <= 0) { timeout = 1; }
        else {
            tela_desafio_contra_tempo(indice, segs_restantes);
            resposta = ler_resposta_contra_tempo(segs_restantes, &usados);
            usados += (TIMER_SEGUNDOS - segs_restantes);
            if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }
        }
    }

    if (resposta == -1 || timeout) {
        jogador.resposta_jogador[indice] = 0; jogador.acertou[indice] = 0;
        jogador.tempo_por_desafio[indice] = TIMER_SEGUNDOS;
        tela_feedback_contra_tempo(0, indice, TIMER_SEGUNDOS, 1, jogador.usou_ajuda[indice]);
        return;
    }

    acertou = (toupper((unsigned char)resposta) == desafios[indice].resposta_correta);
    jogador.resposta_jogador[indice] = resposta;
    jogador.acertou[indice] = acertou ? 1 : 0;
    jogador.tempo_por_desafio[indice] = usados;

    if (acertou) {
        int pontos = desafios[indice].nivel * 10;
        if (!jogador.usou_ajuda[indice]) pontos = bonus_de(usados, desafios[indice].nivel);
        jogador.acertos++;
        jogador.pontuacao += pontos;
        jogador.moedas += desafios[indice].nivel * 2;
    }

    tela_feedback_contra_tempo(acertou, indice, usados, 0, jogador.usou_ajuda[indice]);
}

void executar_nivel_contra_tempo(void) {
    int i;
    tela_intro_contra_tempo();
    
    int moedas_antigas = jogador.moedas;
    int dicas_antigas = jogador.dicas_gratis;
    
    inicializar_jogo();
    jogador.moedas = moedas_antigas;
    jogador.dicas_gratis = dicas_antigas;

    for (i = NUM_DESAFIOS_BASE; i < NUM_DESAFIOS; i++) rodar_desafio_contra_tempo(i);
    jogador.posicao_ranking = score_inserir(jogador.nome, jogador.pontuacao);
    tela_resultado_contra_tempo(NUM_DESAFIOS_AVANC);
}

void finalizar_jogo(int total_rodada) {
    jogador.posicao_ranking = score_inserir(jogador.nome, jogador.pontuacao);
    tela_resultado_final(total_rodada);
}

/* ==========================================
 * Lógica da Loja e Lootbox (US13 / US14)
 * ========================================== */

void processar_lootbox(void) {
    tela_lootbox_animacao();
    int sorteio = rng_range(100); // 0 a 99

    if (sorteio < 40) {
        jogador.pontuacao += 50;
        tela_lootbox_resultado("+ 50 Pontos extras!");
    } else if (sorteio < 80) {
        jogador.moedas += 20;
        tela_lootbox_resultado("+ 20 Moedas!");
    } else {
        jogador.dicas_gratis += 2;
        tela_lootbox_resultado("+ 2 Dicas Gratis!");
    }
}

void executar_loja(void) {
    int opcao;
    while (1) {
        tela_loja();
        opcao = ler_opcao();

        if (opcao == 3) break; // Sair

        if (opcao == 1) {
            if (jogador.moedas >= CUSTO_LOOTBOX) {
                jogador.moedas -= CUSTO_LOOTBOX;
                processar_lootbox();
            } else {
                tela_aviso("Moedas insuficientes!");
            }
        }

        if (opcao == 2) {
            if (jogador.moedas >= CUSTO_DICA) {
                jogador.moedas -= CUSTO_DICA;
                jogador.dicas_gratis++;
                tela_aviso("Comprou 1 Dica Gratis!");
            } else {
                tela_aviso("Moedas insuficientes!");
            }
        }
    }
}