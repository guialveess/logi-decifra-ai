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
#endif

static double g_b    = 1.0;
static int    g_idx  = 0;

static void pausa_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void limpar_tela(void) {
    printf("\033[2J\033[H");
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

static void linha_divisoria(void) {
    set_cor(60, 90, 180);
    printf("    ────────────────────────────────────────────────\n");
    rst();
}

static void mostrar_cursor(void)  { printf("\033[?25h"); fflush(stdout); }
static void esconder_cursor(void) { printf("\033[?25l"); fflush(stdout); }

static void fade_tela(void (*render)(void)) {
    static const double steps[] = {0.05, 0.15, 0.3, 0.5, 0.7, 0.87, 1.0};
    int n = 7, i;
    esconder_cursor();
    limpar_tela();
    for (i = 0; i < n; i++) {
        if (i > 0) { printf("\033[H"); fflush(stdout); }
        g_b = steps[i];
        render();
        fflush(stdout);
        pausa_ms(32);
    }
    g_b = 1.0;
    mostrar_cursor();
}

static void loading_item(const char *texto) {
    static const char *frames[] = {
        "\xe2\xa0\x8b", "\xe2\xa0\x99", "\xe2\xa0\xb9", "\xe2\xa0\xb8",
        "\xe2\xa0\xbc", "\xe2\xa0\xb4", "\xe2\xa0\xa6", "\xe2\xa0\xa7",
        "\xe2\xa0\x87", "\xe2\xa0\x8f"
    };
    int i;
    for (i = 0; i < 12; i++) {
        set_cor(100, 120, 160);
        printf("\r    %s  %s", frames[i % 10], texto);
        rst(); fflush(stdout);
        pausa_ms(70);
    }
    set_cor(100, 140, 255);
    printf("\r    ✔   "); rst();
    printf("%s\n", texto);
    fflush(stdout);
}

static void render_menu(void) {
    printf("\n");
    p(100,140,255, "    LOGI - Decifra.IA");
    p(120,125,160, "  Logica e Seguranca em IA\n\n");
    linha_divisoria();
    printf("\n");
    p(100,140,255, "    1"); p(200,210,230, "   Jogar\n");
    p(100,140,255, "    2"); p(200,210,230, "   Como Jogar\n");
    p(100,140,255, "    3"); p(200,210,230, "   Sair\n");
    printf("\n");
    linha_divisoria();
    printf("\n");
    p(120,125,160, "    opcao: ");
}

void tela_menu(void) {
    fade_tela(render_menu);
}

static void render_como_jogar(void) {
    printf("\n");
    p(100,140,255, "    Como Jogar\n\n");
    linha_divisoria();
    printf("\n");
    p(200,210,230, "    Voce recebera desafios de logica proposicional e seguranca em IA.\n");
    p(200,210,230, "    Para os desafios de logica, responda V (Verdadeiro) ou F (Falso).\n\n");
    p(200,210,230, "    Durante o desafio, pressione ");
    p(100,140,255, "H");
    p(200,210,230, " para pedir ajuda ao LOGI,\n");
    p(200,210,230, "    a IA tutora do jogo.\n\n");
    p(120,125,160, "    Operadores logicos:\n\n");
    p(100,140,255, "      AND            "); p(200,210,230, "verdadeiro quando ambos os lados sao V\n");
    p(100,140,255, "      OR             "); p(200,210,230, "verdadeiro quando pelo menos um lado e V\n");
    p(100,140,255, "      NOT            "); p(200,210,230, "inverte o valor logico\n");
    p(100,140,255, "      IMPLICA        "); p(200,210,230, "falso apenas quando P = V e Q = F\n");
    p(100,140,255, "      BICONDICIONAL  "); p(200,210,230, "verdadeiro quando os dois lados sao iguais\n\n");
    p(200,210,230, "    Cada acerto vale pontos de acordo com o nivel do desafio.\n");
    p(100,140,255, "    Boa sorte!\n\n");
    linha_divisoria();
    p(120,125,160, "\n    Pressione ENTER para continuar...");
}

void tela_como_jogar(void) {
    fade_tela(render_como_jogar);
    mostrar_cursor();
    getchar();
    esconder_cursor();
}

void tela_loading(void) {
    esconder_cursor();
    limpar_tela();
    printf("\n");
    p(100,140,255, "    LOGI - Decifra.IA\n\n");

    loading_item("Inicializando sistema de logica e IA");
    loading_item("Carregando banco de questoes");
    loading_item("Conectando a IA tutora");
    loading_item("Pronto");
    printf("\n");
    pausa_ms(180);

    set_cor(60, 90, 180);
    printf("    ┌─ sistema ────────────────────────────────────────────────┐\n");
    printf("    │"); rst();
    printf("  "); p(120,125,160,"Jogo    "); printf("    "); p(100,140,255,"LOGI - Decifra.IA");
    printf("                       "); set_cor(60,90,180); printf("│\n"); rst();
    printf("    │"); rst();
    printf("  "); p(120,125,160,"Versao  "); printf("    "); p(100,140,255,"1.0  PI2");
    printf("                                "); set_cor(60,90,180); printf("│\n"); rst();
    printf("    │"); rst();
    printf("  "); p(120,125,160,"Modo    "); printf("    "); p(100,140,255,"Logica Proposicional e Seguranca em IA");
    printf("  "); set_cor(60,90,180); printf("│\n"); rst();
    printf("    │"); rst();
    printf("  "); p(120,125,160,"Status  "); printf("    "); p(100,140,255,"PRONTO");
    printf("                                  "); set_cor(60,90,180); printf("│\n"); rst();
    set_cor(60, 90, 180);
    printf("    └──────────────────────────────────────────────────────────┘\n");
    rst();

    p(120,125,160, "\n    Pressione ENTER para continuar...");
    mostrar_cursor();
    getchar();
    esconder_cursor();
}

static void render_nome(void) {
    printf("\n");
    p(100,140,255, "    LOGI - Decifra.IA\n\n");
    linha_divisoria();
    printf("\n");
    p(200,210,230, "    Como voce se chama?\n\n");
    p(120,125,160, "    Nome: ");
}

void tela_nome(void) {
    fade_tela(render_nome);
}

static void render_desafio(void) {
    char buf[256];
    printf("\n");
    snprintf(buf, sizeof(buf),
             "    LOGI - Decifra.IA  |  Desafio %d de %d  |  Nivel %d  |  Pontos: %d\n",
             g_idx + 1, NUM_DESAFIOS, desafios[g_idx].nivel, jogador.pontuacao);
    p(100,140,255, buf);
    printf("\n");
    linha_divisoria();
    printf("\n");
    p(200,210,230, "    ");
    p(200,210,230, desafios[g_idx].enunciado);
    printf("\n\n");
    linha_divisoria();
    printf("\n");
    p(100,140,255, "    [V]"); p(200,210,230, " Verdadeiro");
    p(100,140,255, "     [F]"); p(200,210,230, " Falso");
    p(100,140,255, "     [H]"); p(200,210,230, " Pedir ajuda ao LOGI\n\n");
    p(120,125,160, "    resposta: ");
}

void tela_desafio(int indice) {
    g_idx = indice;
    fade_tela(render_desafio);
}

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
    printf("\n");
    p(100,140,255, "    LOGI");
    p(120,125,160, "  Tutora de Logica e Seguranca em IA\n\n");
    linha_divisoria();
    printf("\n");
    snprintf(buf, sizeof(buf), "    Dica para o desafio %d:\n\n", g_idx + 1);
    p(120,125,160, buf);
    p(100,140,255, "    "); p(100,140,255, dicas_l1[g_idx]); printf("\n");
    p(100,140,255, "    "); p(100,140,255, dicas_l2[g_idx]); printf("\n\n");
    linha_divisoria();
    p(120,125,160, "\n    Pressione ENTER para continuar...");
}

void tela_painel_logi(int indice) {
    g_idx = indice;
    fade_tela(render_logi);
    mostrar_cursor();
    getchar();
    esconder_cursor();
}

static int g_acertou = 0;

static void render_feedback(void) {
    char buf[64];
    printf("\n");
    snprintf(buf, sizeof(buf), "    LOGI - Decifra.IA  |  Desafio %d de %d\n",
             g_idx + 1, NUM_DESAFIOS);
    p(100,140,255, buf);
    printf("\n");
    linha_divisoria();
    printf("\n");
    if (g_acertou) {
        snprintf(buf, sizeof(buf), "    CORRETO  +%d pontos\n\n",
                 desafios[g_idx].nivel * 10);
        p(80,200,120, buf);
    } else {
        snprintf(buf, sizeof(buf), "    INCORRETO  resposta correta: %c\n\n",
                 desafios[g_idx].resposta_correta);
        p(220,70,70, buf);
    }
    p(120,125,160, "    Explicacao:\n");
    p(200,210,230, "    "); p(200,210,230, desafios[g_idx].explicacao); printf("\n\n");
    linha_divisoria();
    p(120,125,160, "\n    Pressione ENTER para continuar...");
}

void tela_feedback(int acertou, int indice) {
    g_idx    = indice;
    g_acertou = acertou;
    fade_tela(render_feedback);
    mostrar_cursor();
    getchar();
    esconder_cursor();
}

static void render_resultado(void) {
    char buf[64];
    printf("\n");
    p(100,140,255, "    LOGI - Decifra.IA");
    p(120,125,160, "  Resultado Final\n\n");
    linha_divisoria();
    printf("\n");
    snprintf(buf, sizeof(buf), "    Jogador    %s\n", jogador.nome);
    p(120,125,160, buf);
    snprintf(buf, sizeof(buf), "    Acertos    %d de %d\n", jogador.acertos, NUM_DESAFIOS);
    p(100,140,255, buf);
    snprintf(buf, sizeof(buf), "    Pontos     %d\n\n", jogador.pontuacao);
    p(100,140,255, buf);
    if (jogador.acertos == NUM_DESAFIOS)
        p(80,200,120, "    PERFEITO  Voce domina a logica proposicional!\n");
    else if (jogador.acertos >= 3)
        p(100,140,255, "    BOM  Continue praticando!\n");
    else
        p(120,125,160, "    INICIANTE  Nao desista, a logica leva tempo.\n");
    printf("\n");
    linha_divisoria();
    p(120,125,160, "\n    Pressione ENTER para continuar...");
}

void tela_resultado_final(void) {
    fade_tela(render_resultado);
    mostrar_cursor();
    getchar();
    esconder_cursor();
}

void tela_saida(void) {
    static const char *frames[] = {
        "\xe2\xa0\x8b", "\xe2\xa0\x99", "\xe2\xa0\xb9", "\xe2\xa0\xb8",
        "\xe2\xa0\xbc", "\xe2\xa0\xb4", "\xe2\xa0\xa6", "\xe2\xa0\xa7",
        "\xe2\xa0\x87", "\xe2\xa0\x8f"
    };
    int i;
    limpar_tela();
    printf("\n");
    for (i = 0; i < 16; i++) {
        set_cor(100 * (i + 1) / 16, 140 * (i + 1) / 16, 255 * (i + 1) / 16);
        printf("\r    %s  Ate logo!", frames[i % 10]);
        rst();
        fflush(stdout);
        pausa_ms(80);
    }
    set_cor(100, 140, 255);
    printf("\r    ✔   Ate logo!\n\n");
    rst();
    fflush(stdout);
    pausa_ms(400);
}
