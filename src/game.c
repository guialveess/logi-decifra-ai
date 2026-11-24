#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "game.h"
#include "ui.h"
#include "input.h"
#include "audio.h"
#include "score.h"
#include "narrativa.h"
#include "conquistas.h"

typedef struct {
    const char *expressao;
    const char *vars;
    const char *regra;
    int         nivel;
} DesafioTemplate;

static const DesafioTemplate templates[NUM_DESAFIOS] = {
    /* Níveis Base */
    { "P AND Q",             "PQ",  "AND so e V quando ambos os lados sao V.",         1 },
    { "P OR Q",              "PQ",  "OR e V quando pelo menos um lado e V.",           1 },
    { "NOT P",               "P",   "NOT inverte o valor logico.",                     2 },
    { "P IMPLICA Q",         "PQ",  "P IMPLICA Q e F apenas quando P=V e Q=F.",        3 },
    { "P BICONDICIONAL Q",   "PQ",  "BICONDICIONAL e V quando os lados sao iguais.",   3 },
    /* Nível Avançado (Com parênteses) */
    { "(P OR Q) AND NOT R",             "PQR", "Resolva o parenteses antes.",          NIVEL_AVANCADO },
    { "NOT (P AND Q) OR R",             "PQR", "O NOT fora do parenteses so afeta o interno.", NIVEL_AVANCADO },
    { "P AND (Q OR NOT R)",             "PQR", "Interior do parenteses primeiro.",     NIVEL_AVANCADO },
    { "(P IMPLICA Q) AND (Q OR R)",     "PQR", "Avalie cada parenteses separado.",     NIVEL_AVANCADO },
    { "NOT (P BICONDICIONAL Q) OR R",   "PQR", "Parenteses primeiro, depois NOT.",     NIVEL_AVANCADO }
};

Desafio desafios[NUM_DESAFIOS];
Jogador jogador;

/* Trava do ranking: garante UM registro por partida, em qualquer modo. */
static int g_score_registrado = 0;

static void registrar_score(void) {
    if (g_score_registrado) return;
    jogador.posicao_ranking = score_inserir(jogador.nome, jogador.pontuacao);
    g_score_registrado = 1;
}

/* ═══════════════ RNG ═══════════════ */
static unsigned int g_rng = 0;

static void rng_init(void) {
    FILE *f = fopen("/dev/urandom", "rb");
    if (f) { if (fread(&g_rng, sizeof(g_rng), 1, f) != 1) g_rng = 0; fclose(f); }
    else   { g_rng = (unsigned int)time(NULL) ^ ((unsigned int)getpid() << 16); }
    if (g_rng == 0) g_rng = (unsigned int)time(NULL) ^ 0xDEADBEEFu;
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

/* ═══════════════ Parser Lógico ═══════════════ */
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

/* Pública (sem static) para poder ser testada em tests/. Protótipo em game.h. */
int avaliar_expressao(const char *expr, const char *vars, const int *vals, int n) {
    Parser p; p.s = expr; p.pos = 0; p.var_names = vars; p.var_vals = vals; p.n_vars = n; return eval_expr(&p);
}

static void gerar_desafio_da_sessao(int idx) {
    const DesafioTemplate *t = &templates[idx];
    int n = (int)strlen(t->vars), total = 1 << n, linha = rng_range(total), i, off = 0, v;
    int vals[3] = {0, 0, 0};
    char vars_buf[DESAFIO_VARS_MAX];
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

/* ═══════════════ Ciclo de Vida ═══════════════ */

/* Prepara uma nova partida. NAO mexe no inventario (moedas, dicas, pulos,
 * botoes de tempo): ele persiste entre partidas e e ganho/gasto na Loja. */
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
    jogador.vidas = 3;          /* US20 */
    jogador.sequencia = 0;      /* US23 */
    jogador.hardcore = 0;       /* US25: so o modo basico pergunta */
    g_score_registrado = 0;     /* nova partida pode registrar no ranking */
}

/* US23: atualiza a sequencia de acertos e mostra as conquistas novas.
 * Chamada depois da tela de feedback de cada desafio, em todos os modos. */
static void checar_conquistas(int acertou) {
    int novas[CQ_TOTAL], n, i;
    char msg[128];

    jogador.sequencia = acertou ? jogador.sequencia + 1 : 0;
    n = conquistas_verificar(jogador.acertos, jogador.sequencia,
                             jogador.pontuacao, jogador.moedas, novas);
    for (i = 0; i < n; i++) {
        snprintf(msg, sizeof(msg), "CONQUISTA DESBLOQUEADA: %s (%s)",
                 conquista_nome(novas[i]), conquista_descricao(novas[i]));
        tela_aviso(msg);
    }
}

static void rodar_desafio(int indice) {
    char resposta;
    int acertou;

    tela_desafio(indice);
    resposta = ler_resposta();

    /* US25: no modo Hardcore, ajuda [H] e pulo [P] ficam bloqueados */
    while (jogador.hardcore &&
           (toupper((unsigned char)resposta) == 'H' || toupper((unsigned char)resposta) == 'P')) {
        tela_aviso("Bloqueado no modo Hardcore! Responda V ou F.");
        tela_desafio(indice);
        resposta = ler_resposta();
    }

    if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }

    /* US17: Pular Pergunta */
    if (toupper((unsigned char)resposta) == 'P') {
        if (jogador.pulos_gratis > 0) {
            jogador.pulos_gratis--;
            tela_aviso("Pulo usado! Gerando nova questao...");
            gerar_desafio_da_sessao(indice);
            tela_desafio(indice);
            resposta = ler_resposta();
        } else {
            tela_aviso("Voce nao tem pulos!");
            tela_desafio(indice);
            resposta = ler_resposta();
        }
    }

    /* Dica LOGI */
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
        jogador.moedas += desafios[indice].nivel * 2;
    }
    tela_feedback(acertou, indice);
    checar_conquistas(acertou);
}

/* Modo basico: depois de cada desafio a LOGI compartilha uma reflexao (US21). */
void executar_desafio(int indice) {
    const FalaLogi *fala;
    rodar_desafio(indice);
    fala = narrativa_proxima();
    tela_narracao_logi(fala->tema, fala->fala, narrativa_exibidas());
}

void executar_nivel_avancado(void) {
    int i;
    tela_intro_avancado();
    score_definir_modo(SCORE_MODO_AVANCADO);
    inicializar_jogo();

    for (i = NUM_DESAFIOS_BASE; i < NUM_DESAFIOS; i++) {
        rodar_desafio(i);
    }

    /* O registro no ranking acontece APENAS dentro de finalizar_jogo(). */
    finalizar_jogo(NUM_DESAFIOS_AVANC);
}

/* ═══════════════ Contra-Tempo (US11 / US18) ═══════════════ */

static int bonus_de(int usados, int nivel) {
    int base = nivel * 10;
    if (usados <= BONUS_RAPIDO_SEG) return base * 3 / 2;
    if (usados <= BONUS_MEDIO_SEG)  return base * 5 / 4;
    return base;
}

static void rodar_desafio_contra_tempo(int indice) {
    char resposta;
    int  acertou = 0, usados = 0, timeout = 0, segs_restantes = TIMER_SEGUNDOS;
    int usados_totais = 0;

    while (1) {
        tela_desafio_contra_tempo(indice, segs_restantes);
        resposta = ler_resposta_contra_tempo(segs_restantes, &usados);
        segs_restantes -= usados;
        usados_totais += usados;

        /* US18: Adicionar Tempo */
        if (toupper((unsigned char)resposta) == 'T') {
            if (jogador.botoes_tempo > 0) {
                jogador.botoes_tempo--;
                segs_restantes += 10;
                tela_aviso("+10 Segundos adicionados!");
                continue;
            } else {
                tela_aviso("Voce nao tem botoes de tempo!");
                continue;
            }
        }
        break;
    }

    if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }

    if (resposta == 'H' || resposta == 'h') {
        jogador.usou_ajuda[indice] = 1;
        if (jogador.dicas_gratis > 0) {
            jogador.dicas_gratis--;
            tela_aviso("Dica Gratis! Bonus mantido.");
            jogador.usou_ajuda[indice] = 0;
        }
        tela_painel_logi(indice);
        /* Correcao: o tempo ja foi descontado no loop acima; nao descontar de novo. */
        if (segs_restantes <= 0) { timeout = 1; }
        else {
            tela_desafio_contra_tempo(indice, segs_restantes);
            resposta = ler_resposta_contra_tempo(segs_restantes, &usados);
            usados_totais += usados;
            segs_restantes -= usados;
            if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }
        }
    }

    if (resposta == -1 || timeout) {
        jogador.resposta_jogador[indice] = 0; jogador.acertou[indice] = 0;
        jogador.tempo_por_desafio[indice] = TIMER_SEGUNDOS;
        tela_feedback_contra_tempo(0, indice, TIMER_SEGUNDOS, 1, jogador.usou_ajuda[indice]);
        checar_conquistas(0);
        return;
    }

    acertou = (toupper((unsigned char)resposta) == desafios[indice].resposta_correta);
    jogador.resposta_jogador[indice] = resposta;
    jogador.acertou[indice] = acertou ? 1 : 0;
    jogador.tempo_por_desafio[indice] = usados_totais;

    if (acertou) {
        int pontos = desafios[indice].nivel * 10;
        if (!jogador.usou_ajuda[indice]) pontos = bonus_de(usados_totais, desafios[indice].nivel);
        jogador.acertos++;
        jogador.pontuacao += pontos;
        jogador.moedas += desafios[indice].nivel * 2;
    }

    tela_feedback_contra_tempo(acertou, indice, usados_totais, 0, jogador.usou_ajuda[indice]);
    checar_conquistas(acertou);
}

void executar_nivel_contra_tempo(void) {
    int i;
    tela_intro_contra_tempo();
    score_definir_modo(SCORE_MODO_TEMPO);
    inicializar_jogo();

    for (i = NUM_DESAFIOS_BASE; i < NUM_DESAFIOS; i++) rodar_desafio_contra_tempo(i);
    registrar_score();
    tela_resultado_contra_tempo(NUM_DESAFIOS_AVANC);
}

/* ═══════════════ Sobrevivência (US20) ═══════════════ */

void executar_nivel_sobrevivencia(void) {
    tela_intro_sobrevivencia();
    score_definir_modo(SCORE_MODO_SOBREVIVENCIA);   /* US22: ranking proprio */
    inicializar_jogo();

    while (jogador.vidas > 0) {
        /* Sorteia qualquer desafio (Base ou Avançado) */
        int idx = rng_range(NUM_DESAFIOS);
        char resposta;
        int acertou;

        gerar_desafio_da_sessao(idx);

        tela_desafio_sobrevivencia(idx);
        resposta = ler_resposta();

        if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }

        if (resposta == 'H' || resposta == 'h') {
            if (jogador.dicas_gratis > 0) {
                jogador.dicas_gratis--;
                tela_aviso("Dica Gratis consumida!");
            }
            tela_painel_logi(idx);
            tela_desafio_sobrevivencia(idx);
            resposta = ler_resposta();
            if (toupper((unsigned char)resposta) == 'Q') { tela_saida(); exit(0); }
        }

        acertou = (toupper((unsigned char)resposta) == desafios[idx].resposta_correta);

        if (acertou) {
            jogador.acertos++;
            jogador.pontuacao += desafios[idx].nivel * 10;
            jogador.moedas += desafios[idx].nivel * 2;
            tela_feedback(1, idx);
            checar_conquistas(1);
        } else {
            jogador.vidas--;
            if (jogador.vidas > 0) {
                tela_feedback(0, idx);
            }
            checar_conquistas(0);
        }
    }
    registrar_score();
    tela_game_over();
}

/* ═══════════════ Resumo por operador (US24) ═══════════════ */

static const char *nomes_op[OP_TOTAL] = { "AND", "OR", "NOT", "IMPLICA", "BICONDICIONAL" };

const char *nome_operador(int op) {
    return (op >= 0 && op < OP_TOTAL) ? nomes_op[op] : "";
}

/* 1 se a expressao contem o operador como palavra inteira. */
static int contem_operador(const char *expr, const char *op) {
    size_t n = strlen(op);
    const char *p = expr;
    while ((p = strstr(p, op)) != NULL) {
        int antes_ok  = (p == expr) || !isalpha((unsigned char)p[-1]);
        int depois_ok = !isalpha((unsigned char)p[n]);
        if (antes_ok && depois_ok) return 1;
        p += n;
    }
    return 0;
}

void resumo_operadores(int acertos[OP_TOTAL], int total[OP_TOTAL]) {
    int i, op;
    for (op = 0; op < OP_TOTAL; op++) { acertos[op] = 0; total[op] = 0; }
    for (i = 0; i < NUM_DESAFIOS; i++) {
        if (jogador.resposta_jogador[i] == 0) continue;     /* nao respondido */
        for (op = 0; op < OP_TOTAL; op++) {
            if (!contem_operador(desafios[i].expressao, nomes_op[op])) continue;
            total[op]++;
            if (jogador.acertou[i]) acertos[op]++;
        }
    }
}

int operador_mais_fraco(const int acertos[OP_TOTAL], const int total[OP_TOTAL]) {
    int op, pior = -1;
    for (op = 0; op < OP_TOTAL; op++) {
        if (total[op] == 0 || acertos[op] == total[op]) continue;
        /* compara acertos/total sem divisao: a/b < c/d  <=>  a*d < c*b */
        if (pior < 0 || acertos[op] * total[pior] < acertos[pior] * total[op]) pior = op;
    }
    return pior;
}

int pontos_com_bonus_hardcore(int pontos) {
    if (pontos <= 0) return pontos;
    return pontos + pontos * BONUS_HARDCORE_PCT / 100;
}

void finalizar_jogo(int total_rodada) {
    /* US25: bonus aplicado antes de registrar no ranking */
    if (jogador.hardcore && jogador.pontuacao > 0) {
        char msg[96];
        int antes = jogador.pontuacao;
        jogador.pontuacao = pontos_com_bonus_hardcore(antes);
        snprintf(msg, sizeof(msg), "Bonus Hardcore +%d%%: %d -> %d pontos!",
                 BONUS_HARDCORE_PCT, antes, jogador.pontuacao);
        tela_aviso(msg);
    }
    /* Ponto unico de registro dos modos Basico e Avancado. */
    registrar_score();
    tela_resultado_final(total_rodada);
}

/* ═══════════════ Loja e Lootbox ═══════════════ */

void processar_lootbox(void) {
    int sorteio;
    tela_lootbox_animacao();
    sorteio = rng_range(100);

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

        if (opcao == 5) break;

        if (opcao == 1) {
            if (jogador.moedas >= CUSTO_LOOTBOX) {
                jogador.moedas -= CUSTO_LOOTBOX;
                processar_lootbox();
            } else tela_aviso("Moedas insuficientes!");
        }
        if (opcao == 2) {
            if (jogador.moedas >= CUSTO_DICA) {
                jogador.moedas -= CUSTO_DICA;
                jogador.dicas_gratis++;
                tela_aviso("Comprou 1 Dica Gratis!");
            } else tela_aviso("Moedas insuficientes!");
        }
        if (opcao == 3) {
            if (jogador.moedas >= CUSTO_PULO) {
                jogador.moedas -= CUSTO_PULO;
                jogador.pulos_gratis++;
                tela_aviso("Comprou 1 Pulo!");
            } else tela_aviso("Moedas insuficientes!");
        }
        if (opcao == 4) {
            if (jogador.moedas >= CUSTO_TEMPO) {
                jogador.moedas -= CUSTO_TEMPO;
                jogador.botoes_tempo++;
                tela_aviso("Comprou +10 Segundos!");
            } else tela_aviso("Moedas insuficientes!");
        }
    }
}