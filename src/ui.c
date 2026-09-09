#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "game.h"
#include "input.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

static double g_b     = 1.0;
static int    g_idx   = 0;
static int    g_acertou = 0;
static int    g_rows  = 24;

static void pausa_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

static void atualizar_tamanho(void) {
#ifndef _WIN32
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 4) {
        g_rows = ws.ws_row;
    }
#endif
}

void limpar_tela(void) {
    printf("\033[H\033[3J\033[2J\033[H");
    fflush(stdout);
}

static void set_cor(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm",
           (int)(r * g_b), (int)(g * g_b), (int)(b * g_b));
}
static void rst(void) { printf("\033[0m"); }

static void p(int r, int g, int b, const char *s) {
    set_cor(r, g, b); printf("%s", s); rst();
}

static void ir(int r, int c) {
    printf("\033[%d;%dH", r, c);
}

static void linha_div(void) {
    set_cor(60, 90, 180);
    printf("────────────────────────────────────────────────");
    rst();
}

static void mostrar_cursor(void)  { printf("\033[?25h"); fflush(stdout); }
static void esconder_cursor(void) { printf("\033[?25l"); fflush(stdout); }

static void ler_enter(void) {
#ifndef _WIN32
    struct termios orig, raw;
    int c;
    tcgetattr(STDIN_FILENO, &orig);
    raw = orig;
    raw.c_lflag &= ~(unsigned)(ECHO | ICANON);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    do { c = getchar(); } while (c != '\n' && c != '\r' && c != EOF);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
#else
    getchar();
#endif
}

static void fade_tela(void (*render)(void)) {
    static const double steps[] = {0.05, 0.15, 0.3, 0.5, 0.7, 0.87, 1.0};
    int n = 7, i;
    atualizar_tamanho();
    esconder_cursor();
    for (i = 0; i < n; i++) {
        printf("\033[H\033[3J\033[2J\033[H");
        fflush(stdout);
        g_b = steps[i];
        render();
        fflush(stdout);
        pausa_ms(32);
    }
    g_b = 1.0;
    mostrar_cursor();
}

static const char *g_frames[] = {
    "\xe2\xa0\x8b", "\xe2\xa0\x99", "\xe2\xa0\xb9", "\xe2\xa0\xb8",
    "\xe2\xa0\xbc", "\xe2\xa0\xb4", "\xe2\xa0\xa6", "\xe2\xa0\xa7",
    "\xe2\xa0\x87", "\xe2\xa0\x8f"
};

static void loading_item_em(int linha, const char *texto) {
    int i;
    for (i = 0; i < 12; i++) {
        ir(linha, 5);
        set_cor(100, 120, 160);
        printf("%s  %s", g_frames[i % 10], texto);
        rst(); fflush(stdout);
        pausa_ms(70);
    }
    ir(linha, 5);
    set_cor(100, 140, 255);
    printf("✔   ");
    rst();
    printf("%s", texto);
    fflush(stdout);
    pausa_ms(80);
}

/* ── MENU ─────────────────────────────────────────────────── */

static void render_menu(void) {
    int r = (g_rows - 10) / 2;
    int c = 5;
    if (r < 1) r = 1;
    ir(r,    c); p(100,140,255, "LOGI - Decifra.IA");
                 p(120,125,160, "  Logica e Seguranca em IA");
    ir(r+2,  c); linha_div();
    ir(r+4,  c); p(100,140,255, "1"); p(200,210,230, "   Jogar");
    ir(r+5,  c); p(100,140,255, "2"); p(200,210,230, "   Como Jogar");
    ir(r+6,  c); p(100,140,255, "3"); p(200,210,230, "   Sair");
    ir(r+8,  c); linha_div();
    ir(r+10, c); p(120,125,160, "opcao: ");
}

void tela_menu(void) {
    fade_tela(render_menu);
}

/* ── COMO JOGAR ───────────────────────────────────────────── */

static void render_como_jogar(void) {
    int r = (g_rows - 18) / 2;
    int c = 5;
    if (r < 1) r = 1;
    ir(r,    c); p(100,140,255, "Como Jogar");
    ir(r+1,  c); linha_div();
    ir(r+3,  c); p(200,210,230, "Voce recebera desafios de logica proposicional e seguranca em IA.");
    ir(r+4,  c); p(200,210,230, "Para os desafios de logica, responda V (Verdadeiro) ou F (Falso).");
    ir(r+6,  c); p(200,210,230, "Pressione "); p(100,140,255, "H");
                 p(200,210,230, " para pedir ajuda ao LOGI, a IA tutora do jogo.");
    ir(r+8,  c); p(120,125,160, "Operadores logicos:");
    ir(r+10, c); p(100,140,255, "AND            "); p(200,210,230, "verdadeiro quando ambos os lados sao V");
    ir(r+11, c); p(100,140,255, "OR             "); p(200,210,230, "verdadeiro quando pelo menos um lado e V");
    ir(r+12, c); p(100,140,255, "NOT            "); p(200,210,230, "inverte o valor logico");
    ir(r+13, c); p(100,140,255, "IMPLICA        "); p(200,210,230, "falso apenas quando P = V e Q = F");
    ir(r+14, c); p(100,140,255, "BICONDICIONAL  "); p(200,210,230, "verdadeiro quando os dois lados sao iguais");
    ir(r+16, c); linha_div();
    ir(r+17, c); p(120,125,160, "Pressione ENTER para continuar...");
}

void tela_como_jogar(void) {
    fade_tela(render_como_jogar);
    mostrar_cursor();
    ler_enter();
    esconder_cursor();
}

/* ── LOADING ──────────────────────────────────────────────── */

void tela_loading(void) {
    int mid, c;
    atualizar_tamanho();
    mid = g_rows / 2;
    c = 5;
    esconder_cursor();
    limpar_tela();

    ir(mid - 8, c);
    p(100,140,255, "LOGI - Decifra.IA");
    rst(); fflush(stdout);

    loading_item_em(mid - 6, "Inicializando sistema de logica e IA");
    loading_item_em(mid - 4, "Carregando banco de questoes");
    loading_item_em(mid - 2, "Conectando a IA tutora");
    loading_item_em(mid,     "Pronto");
    pausa_ms(180);

    set_cor(60, 90, 180);
    ir(mid+2, c); printf("┌─ sistema ────────────────────────────────────────────────┐");
    ir(mid+3, c); printf("│"); rst();
    printf("  "); p(120,125,160,"Jogo    "); printf("    "); p(100,140,255,"LOGI - Decifra.IA");
    printf("                       "); set_cor(60,90,180); printf("│"); rst();
    ir(mid+4, c); set_cor(60,90,180); printf("│"); rst();
    printf("  "); p(120,125,160,"Versao  "); printf("    "); p(100,140,255,"1.0  PI2");
    printf("                                "); set_cor(60,90,180); printf("│"); rst();
    ir(mid+5, c); set_cor(60,90,180); printf("│"); rst();
    printf("  "); p(120,125,160,"Modo    "); printf("    "); p(100,140,255,"Logica Proposicional e Seguranca em IA");
    printf("  "); set_cor(60,90,180); printf("│"); rst();
    ir(mid+6, c); set_cor(60,90,180); printf("│"); rst();
    printf("  "); p(120,125,160,"Status  "); printf("    "); p(100,140,255,"PRONTO");
    printf("                                  "); set_cor(60,90,180); printf("│"); rst();
    set_cor(60, 90, 180);
    ir(mid+7, c); printf("└──────────────────────────────────────────────────────────┘");
    rst(); fflush(stdout);

    ir(mid+9, c);
    p(120,125,160, "Pressione ENTER para continuar...");
    mostrar_cursor(); fflush(stdout);
    ler_enter();
    esconder_cursor();
}

/* ── NOME ─────────────────────────────────────────────────── */

static void render_nome(void) {
    int r = (g_rows - 6) / 2;
    int c = 5;
    if (r < 1) r = 1;
    ir(r,   c); p(100,140,255, "LOGI - Decifra.IA");
    ir(r+2, c); linha_div();
    ir(r+4, c); p(200,210,230, "Como voce se chama?");
    ir(r+6, c); p(120,125,160, "Nome: ");
}

void tela_nome(void) {
    fade_tela(render_nome);
}

/* ── DESAFIO ──────────────────────────────────────────────── */

static void render_desafio(void) {
    char buf[256];
    int r = (g_rows - 12) / 2;
    int c = 5;
    if (r < 1) r = 1;
    snprintf(buf, sizeof(buf),
             "LOGI - Decifra.IA  |  Desafio %d de %d  |  Nivel %d  |  Pontos: %d",
             g_idx + 1, NUM_DESAFIOS, desafios[g_idx].nivel, jogador.pontuacao);
    ir(r,    c); p(100,140,255, buf);
    ir(r+2,  c); linha_div();
    ir(r+4,  c); p(200,210,230, desafios[g_idx].enunciado);
    ir(r+7,  c); linha_div();
    ir(r+9,  c); p(100,140,255, "[V]"); p(200,210,230, " Verdadeiro");
                 p(100,140,255, "     [F]"); p(200,210,230, " Falso");
                 p(100,140,255, "     [H]"); p(200,210,230, " Pedir ajuda ao LOGI");
    ir(r+11, c); p(120,125,160, "resposta: ");
}

void tela_desafio(int indice) {
    g_idx = indice;
    fade_tela(render_desafio);
}

/* ── PAINEL LOGI ──────────────────────────────────────────── */

static const char *dicas_l1[NUM_DESAFIOS] = {
    "AND so e verdadeiro quando os dois lados sao V.",
    "OR e verdadeiro quando pelo menos um lado e V.",
    "NOT inverte o valor logico.",
    "P IMPLICA Q e falso apenas quando P = V e Q = F.",
    "BICONDICIONAL e V quando os dois lados tem o mesmo valor."
};
static const char *dicas_l2[NUM_DESAFIOS] = {
    "Se qualquer lado for F, o resultado e F.",
    "So e falso quando os dois lados sao F.",
    "NOT V = F  e  NOT F = V.",
    "Em todos os outros casos o resultado e V.",
    "V BICON V = V  e  F BICON F = V."
};

static void render_logi(void) {
    char buf[48];
    int r = (g_rows - 10) / 2;
    int c = 5;
    if (r < 1) r = 1;
    ir(r,    c); p(100,140,255, "LOGI");
                 p(120,125,160, "  Tutora de Logica e Seguranca em IA");
    ir(r+2,  c); linha_div();
    snprintf(buf, sizeof(buf), "Dica para o desafio %d:", g_idx + 1);
    ir(r+4,  c); p(120,125,160, buf);
    ir(r+6,  c); p(100,140,255, dicas_l1[g_idx]);
    ir(r+7,  c); p(100,140,255, dicas_l2[g_idx]);
    ir(r+9,  c); linha_div();
    ir(r+10, c); p(120,125,160, "Pressione ENTER para continuar...");
}

void tela_painel_logi(int indice) {
    g_idx = indice;
    fade_tela(render_logi);
    mostrar_cursor();
    ler_enter();
    esconder_cursor();
}

/* ── FEEDBACK ─────────────────────────────────────────────── */

static void render_feedback(void) {
    char buf[128];
    int r = (g_rows - 12) / 2;
    int c = 5;
    if (r < 1) r = 1;
    snprintf(buf, sizeof(buf), "LOGI - Decifra.IA  |  Desafio %d de %d",
             g_idx + 1, NUM_DESAFIOS);
    ir(r,    c); p(100,140,255, buf);
    ir(r+2,  c); linha_div();
    if (g_acertou) {
        snprintf(buf, sizeof(buf), "CORRETO  +%d pontos", desafios[g_idx].nivel * 10);
        ir(r+4, c); p(80,200,120, buf);
    } else {
        snprintf(buf, sizeof(buf), "INCORRETO  resposta correta: %c",
                 desafios[g_idx].resposta_correta);
        ir(r+4, c); p(220,70,70, buf);
    }
    ir(r+6,  c); p(120,125,160, "Explicacao:");
    ir(r+7,  c); p(200,210,230, desafios[g_idx].explicacao);
    ir(r+10, c); linha_div();
    ir(r+11, c); p(120,125,160, "Pressione ENTER para continuar...");
}

void tela_feedback(int acertou, int indice) {
    g_idx     = indice;
    g_acertou = acertou;
    fade_tela(render_feedback);
    mostrar_cursor();
    ler_enter();
    esconder_cursor();
}

/* ── RESULTADO FINAL ──────────────────────────────────────── */

static void render_resultado(void) {
    char buf[64];
    int r = (g_rows - 10) / 2;
    int c = 5;
    if (r < 1) r = 1;
    ir(r,   c); p(100,140,255, "LOGI - Decifra.IA");
                p(120,125,160, "  Resultado Final");
    ir(r+2, c); linha_div();
    snprintf(buf, sizeof(buf), "Jogador    %s", jogador.nome);
    ir(r+4, c); p(120,125,160, buf);
    snprintf(buf, sizeof(buf), "Acertos    %d de %d", jogador.acertos, NUM_DESAFIOS);
    ir(r+5, c); p(100,140,255, buf);
    snprintf(buf, sizeof(buf), "Pontos     %d", jogador.pontuacao);
    ir(r+6, c); p(100,140,255, buf);
    if (jogador.acertos == NUM_DESAFIOS)
        { ir(r+8, c); p(80,200,120,  "PERFEITO  Voce domina a logica proposicional!"); }
    else if (jogador.acertos >= 3)
        { ir(r+8, c); p(100,140,255, "BOM  Continue praticando!"); }
    else
        { ir(r+8, c); p(120,125,160, "INICIANTE  Nao desista, a logica leva tempo."); }
    ir(r+9,  c); linha_div();
    ir(r+10, c); p(120,125,160, "Pressione ENTER para continuar...");
}

void tela_resultado_final(void) {
    fade_tela(render_resultado);
    mostrar_cursor();
    ler_enter();
    esconder_cursor();
}

/* ── SAIDA ────────────────────────────────────────────────── */

void tela_saida(void) {
    int i, r, c;
    atualizar_tamanho();
    r = g_rows / 2;
    c = 5;
    limpar_tela();
    for (i = 0; i < 16; i++) {
        ir(r, c);
        set_cor(100 * (i + 1) / 16, 140 * (i + 1) / 16, 255 * (i + 1) / 16);
        printf("%s  Ate logo!", g_frames[i % 10]);
        rst(); fflush(stdout);
        pausa_ms(80);
    }
    ir(r, c);
    set_cor(100, 140, 255);
    printf("✔   Ate logo!");
    rst(); fflush(stdout);
    pausa_ms(400);
}
