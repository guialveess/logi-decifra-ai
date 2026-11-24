/* ============================================================================
 * Interface grafica (raylib) - PI2-189
 *
 * Backend alternativo ao terminal: implementa TODAS as funcoes de ui.h e de
 * input.h (tela_*, ler_*). Por isso game.c, score.c, narrativa.c e ai_client.c
 * funcionam sem nenhuma alteracao.
 *
 *   make gui        -> compila ./jogo_gui (usa ui_raylib.c no lugar de ui.c/input.c)
 *   make            -> continua gerando o jogo de terminal (./jogo)
 *
 * Funcionamento: cada tela_*() escolhe uma funcao de desenho (g_render) e os
 * ler_*() ficam num laco que redesenha a cena enquanto esperam a tecla/clique.
 * Obs.: a fonte padrao da raylib nao tem acentos; os textos seguem sem acento.
 * ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <raylib.h>
#include "ui.h"
#include "game.h"
#include "input.h"
#include "ai_client.h"
#include "audio.h"
#include "score.h"

#define LARG    960
#define ALT     600
#define MARGEM   48

/* ═══════════════ Paleta (mesma do terminal) ═══════════════ */

static Color rgb(int r, int g, int b) {
    Color c;
    c.r = (unsigned char)r; c.g = (unsigned char)g;
    c.b = (unsigned char)b; c.a = 255;
    return c;
}
#define COR_FUNDO    rgb(14, 16, 28)
#define COR_PAINEL   rgb(26, 30, 56)
#define COR_AZUL     rgb(100, 140, 255)
#define COR_AZUL_DIM rgb(60, 90, 180)
#define COR_BRANCO   rgb(200, 210, 230)
#define COR_CINZA    rgb(120, 125, 160)
#define COR_VERDE    rgb(80, 200, 120)
#define COR_VERMELHO rgb(220, 70, 70)
#define COR_AMARELO  rgb(255, 220, 80)
#define COR_AMBAR    rgb(255, 180, 60)
#define COR_LARANJA  rgb(255, 120, 60)

/* ═══════════════ Estado global da UI ═══════════════ */

static void (*g_render)(void) = NULL;   /* cena atual */
static int    g_click   = 0;            /* tecla "emulada" por clique de botao */
static int    g_idx     = 0;
static int    g_acertou = 0;
static int    g_total_rodada = 0;

static char   g_dica_ai[AI_RESP_MAX] = "";
static int    g_tem_ai  = 0;

static int g_var     = 0;               /* 0 basico/avancado, 1 contra-tempo, 2 sobrevivencia */
static int g_ct_segs = TIMER_SEGUNDOS;

static int g_ft_acertou = 0, g_ft_usados = 0, g_ft_timeout = 0, g_ft_ajuda = 0;

static char g_edit[NOME_MAX] = "";      /* texto sendo digitado */

/* ═══════════════ Nucleo: janela, quadro e entrada ═══════════════ */

void gui_init(void) {
    InitWindow(LARG, ALT, "LOGI - Decifra.IA");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);               /* ESC nao fecha o jogo por acidente */
}

void gui_fechar(void) { CloseWindow(); }

static void sair_gui(void) {
    audio_parar();
    CloseWindow();
    exit(0);
}

/* Desenha um quadro da cena atual. Fechar a janela encerra o jogo. */
static void frame(void) {
    if (WindowShouldClose()) sair_gui();
    BeginDrawing();
    ClearBackground(COR_FUNDO);
    if (g_render) g_render();
    EndDrawing();
}

void limpar_tela(void) { g_render = NULL; }

void limpar_stdin(void) { while (GetCharPressed()) { } }

static int enter_pressionado(void) {
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
}

/* Espera uma tecla: devolve 13 para ENTER, o caractere digitado, ou o codigo
 * de um botao clicado (g_click). */
static int esperar_tecla(void) {
    limpar_stdin();
    for (;;) {
        int c;
        g_click = 0;
        frame();
        if (enter_pressionado()) return 13;
        c = GetCharPressed();
        if (c) return c;
        if (g_click) return g_click;
    }
}

static void esperar_enter(void) {
    while (esperar_tecla() != 13) { }
}

void aguardar_enter(void) { esperar_enter(); }

/* Mostra a cena 'r' e espera ENTER. */
static void mostrar_e_esperar(void (*r)(void)) {
    g_render = r;
    esperar_enter();
}

/* ═══════════════ Primitivas de desenho ═══════════════ */

static int g_lim = -1;      /* limite de caracteres (efeito maquina de escrever); -1 = sem limite */

static void desenha_linha(int x, int y, int tam, Color cor, const char *linha) {
    char buf[640];
    int n = (int)strlen(linha);
    if (g_lim >= 0) {
        if (g_lim == 0) return;
        if (n > g_lim) n = g_lim;
        g_lim -= n + 1;
        if (g_lim < 0) g_lim = 0;
    }
    memcpy(buf, linha, (size_t)n);
    buf[n] = '\0';
    DrawText(buf, x, y, tam, cor);
}

/* Texto com quebra automatica de linha. Devolve a altura usada. */
static int txt_quebra(int x, int y, int maxw, int tam, Color cor, const char *s) {
    char linha[512], palavra[128], tent[640];
    int yy = y, lh = tam + 8, k;
    const char *p = s;

    linha[0] = '\0';
    while (*p) {
        if (*p == '\n') {
            desenha_linha(x, yy, tam, cor, linha);
            yy += lh; linha[0] = '\0';
            p++;
            while (*p == ' ') p++;          /* ignora o recuo do texto original */
            continue;
        }
        if (*p == ' ') { p++; continue; }

        k = 0;
        while (*p && *p != ' ' && *p != '\n' && k < (int)sizeof(palavra) - 1) palavra[k++] = *p++;
        palavra[k] = '\0';

        if (linha[0]) snprintf(tent, sizeof(tent), "%s %s", linha, palavra);
        else          snprintf(tent, sizeof(tent), "%s", palavra);

        if (linha[0] && MeasureText(tent, tam) > maxw) {
            desenha_linha(x, yy, tam, cor, linha);
            yy += lh;
            snprintf(linha, sizeof(linha), "%s", palavra);
        } else {
            snprintf(linha, sizeof(linha), "%.500s", tent);
        }
    }
    if (linha[0]) { desenha_linha(x, yy, tam, cor, linha); yy += lh; }
    return yy - y;
}

/* Enunciado do desafio: parenteses destacados em amarelo quando 'hl'. */
static void desenha_enunciado(int x, int y, int tam, int hl) {
    const char *s = desafios[g_idx].enunciado;
    char run[256];
    int cx = x, cy = y, n = 0;

    while (*s == ' ') s++;
    for (;; s++) {
        int fim = (*s == '\0'), nl = (*s == '\n'), par = (*s == '(' || *s == ')');
        if (fim || nl || (hl && par)) {
            if (n > 0) {
                run[n] = '\0';
                DrawText(run, cx, cy, tam, COR_BRANCO);
                cx += MeasureText(run, tam);
                n = 0;
            }
            if (hl && par) {
                char um[2]; um[0] = *s; um[1] = '\0';
                DrawText(um, cx, cy, tam, COR_AMARELO);
                cx += MeasureText(um, tam);
            }
            if (fim) break;
            if (nl) { cx = x; cy += tam + 12; while (s[1] == ' ') s++; }
        } else if (n < (int)sizeof(run) - 1) {
            run[n++] = *s;
        }
    }
}

static void cabecalho(const char *a, const char *b, Color cor) {
    DrawText(a, MARGEM, 34, 30, cor);
    if (b && *b) DrawText(b, MARGEM + MeasureText(a, 30) + 16, 42, 20, COR_CINZA);
    DrawRectangle(MARGEM, 80, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);
}

static void botao(int x, int y, int w, int h, const char *rot, Color cor, int tecla) {
    Rectangle r;
    int hover, tw;
    r.x = (float)x; r.y = (float)y; r.width = (float)w; r.height = (float)h;
    hover = CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRec(r, hover ? rgb(44, 52, 100) : COR_PAINEL);
    DrawRectangleLines(x, y, w, h, cor);
    tw = MeasureText(rot, 20);
    DrawText(rot, x + (w - tw) / 2, y + (h - 20) / 2, 20, cor);
    if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) g_click = tecla;
}

static void rodape_enter(const char *msg) {
    DrawRectangle(MARGEM, ALT - 84, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);
    DrawText(msg, MARGEM, ALT - 62, 20, COR_CINZA);
    botao(LARG - MARGEM - 170, ALT - 72, 170, 44, "ENTER", COR_AZUL, 13);
}

static int total_do_nivel(int nivel)  { return (nivel == NIVEL_AVANCADO) ? NUM_DESAFIOS_AVANC : NUM_DESAFIOS_BASE; }
static int offset_do_nivel(int nivel) { return (nivel == NIVEL_AVANCADO) ? NUM_DESAFIOS_BASE : 0; }

/* ═══════════════ Entrada de texto ═══════════════ */

/* Edita texto usando a cena atual (que deve mostrar g_edit). ENTER confirma. */
static void editar(char *dst, int max) {
    int n = 0, lim = (max - 1 < NOME_MAX - 1) ? max - 1 : NOME_MAX - 1, c;
    g_edit[0] = '\0';
    limpar_stdin();
    for (;;) {
        g_click = 0;
        frame();
        if (enter_pressionado() || g_click == 13) break;
        if (IsKeyPressed(KEY_BACKSPACE) && n > 0) g_edit[--n] = '\0';
        while ((c = GetCharPressed()) != 0)
            if (c >= 32 && c < 127 && n < lim) { g_edit[n++] = (char)c; g_edit[n] = '\0'; }
    }
    snprintf(dst, (size_t)max, "%s", g_edit);
}

void ler_nome(char *nome, int tamanho) { editar(nome, tamanho); }

int ler_opcao(void) {
    for (;;) {
        int c = esperar_tecla();
        if (c >= '1' && c <= '9') return c - '0';
    }
}

int ler_opcao_menu(void) {
    for (;;) {
        int c = esperar_tecla();
        if (c == 'r' || c == 'R') return OPCAO_RANKINGS;
        if (c >= '1' && c <= '9') return c - '0';
    }
}

int ler_sim_nao(void) {
    for (;;) {
        int c = esperar_tecla();
        if (c == 's' || c == 'S') return 1;
        if (c == 'n' || c == 'N') return 0;
    }
}

char ler_resposta(void) {
    for (;;) {
        int c = esperar_tecla(), up;
        if (c > 127) continue;
        up = toupper(c);
        if (up == 'V' || up == 'F' || up == 'H' || up == 'P' || up == 'Q') return (char)c;
    }
}

char ler_resposta_contra_tempo(int segundos, int *usados_out) {
    double ini = GetTime();
    int c, up, dec;

    limpar_stdin();
    for (;;) {
        dec = (int)(GetTime() - ini);
        g_ct_segs = segundos - dec;
        if (g_ct_segs <= 0) { g_ct_segs = 0; *usados_out = segundos; return (char)-1; }

        g_click = 0;
        frame();

        c = 0;
        while ((c = GetCharPressed()) != 0) {
            up = (c < 128) ? toupper(c) : 0;
            if (up == 'V' || up == 'F' || up == 'H' || up == 'P' || up == 'T' || up == 'Q') break;
            c = 0;
        }
        if (!c && g_click) c = g_click;
        if (c) {
            dec = (int)(GetTime() - ini);
            *usados_out = (dec > segundos) ? segundos : dec;
            return (char)c;
        }
    }
}

/* ═══════════════ MENU ═══════════════ */

static const char *menu_itens[8] = {
    "Jogar", "Como Jogar", "Exemplos de Escopo", "Nivel Avancado (Parenteses)",
    "Contra-Tempo (Avancado)", "Loja de Itens e Lootbox",
    "Modo Sobrevivencia (Hardcore)", "Sair"
};
static const unsigned char menu_cor[8][3] = {
    {100,140,255}, {100,140,255}, {100,140,255}, {255,180,60},
    {255,120,60},  {150,255,150}, {220,70,70},   {100,140,255}
};

static void render_menu(void) {
    char buf[96];
    int i;
    cabecalho("LOGI - Decifra.IA", "Logica e Seguranca em IA", COR_AZUL);
    for (i = 0; i < 8; i++) {
        snprintf(buf, sizeof(buf), "%d    %s", i + 1, menu_itens[i]);
        botao(MARGEM, 100 + i * 48, 560, 42, buf,
              rgb(menu_cor[i][0], menu_cor[i][1], menu_cor[i][2]), '1' + i);
    }
    botao(MARGEM, 100 + 8 * 48 + 8, 560, 42, "R    Rankings (todos os modos)", COR_AMARELO, 'R');
    DrawText("Use o teclado (1-8, R) ou clique nas opcoes.", MARGEM + 590, 110, 18, COR_CINZA);
}

void tela_menu(void) {
    audio_tocar(AUDIO_MENU);
    g_render = render_menu;
    frame();
}

/* ═══════════════ COMO JOGAR ═══════════════ */

static void render_como_jogar(void) {
    cabecalho("Como Jogar", "", COR_AZUL);
    DrawText("Responda V (Verdadeiro) ou F (Falso) para cada formula.", MARGEM, 104, 22, COR_BRANCO);
    DrawText("Pressione [H] para pedir ajuda a LOGI (IA tutora).", MARGEM, 136, 22, COR_BRANCO);
    DrawText("Ganhe Moedas ao acertar e use na Loja (opcao 6) para comprar", MARGEM, 180, 22, COR_BRANCO);
    DrawText("Lootboxes, Dicas, Pulos e Tempo.", MARGEM, 208, 22, COR_BRANCO);
    DrawText("Operadores logicos:", MARGEM, 256, 22, COR_CINZA);
    DrawText("AND",            MARGEM, 292, 22, COR_AZUL); DrawText("verdadeiro quando ambos os lados sao V",       MARGEM + 220, 292, 22, COR_BRANCO);
    DrawText("OR",             MARGEM, 324, 22, COR_AZUL); DrawText("verdadeiro quando pelo menos um lado e V",     MARGEM + 220, 324, 22, COR_BRANCO);
    DrawText("NOT",            MARGEM, 356, 22, COR_AZUL); DrawText("inverte o valor logico",                       MARGEM + 220, 356, 22, COR_BRANCO);
    DrawText("IMPLICA",        MARGEM, 388, 22, COR_AZUL); DrawText("falso apenas quando P = V e Q = F",            MARGEM + 220, 388, 22, COR_BRANCO);
    DrawText("BICONDICIONAL",  MARGEM, 420, 22, COR_AZUL); DrawText("verdadeiro quando os dois lados sao iguais",   MARGEM + 220, 420, 22, COR_BRANCO);
    DrawText("No Contra-Tempo, acertos rapidos geram bonus (50% em 10s).", MARGEM, 472, 20, COR_AMBAR);
    rodape_enter("Pressione ENTER para continuar...");
}

void tela_como_jogar(void) { mostrar_e_esperar(render_como_jogar); }

/* ═══════════════ EXEMPLOS DE ESCOPO ═══════════════ */

typedef struct {
    const char *variaveis;
    const char *expr_a, *passos_a, *res_a;
    const char *expr_b, *passos_b, *res_b;
    const char *explicacao;
} ExemploEscopo;

#define NUM_EXEMPLOS_ESCOPO 3
static int g_exemplo_idx = 0;

static const ExemploEscopo exemplos_escopo[NUM_EXEMPLOS_ESCOPO] = {
    { "P = V  |  Q = F  |  R = F",
      "P OR Q AND R", "V OR F AND F  ->  V OR F  ->  V", "V",
      "(P OR Q) AND R", "(V OR F) AND F  ->  V AND F  ->  F", "F",
      "Sem parenteses, AND tem precedencia sobre OR. Com parenteses, o OR e avaliado primeiro." },
    { "P = V  |  Q = F",
      "NOT P AND Q", "NOT V AND F  ->  F AND F  ->  F", "F",
      "NOT (P AND Q)", "NOT (V AND F)  ->  NOT F  ->  V", "V",
      "O parenteses muda o escopo do NOT: ele passa a aplicar-se ao resultado de P AND Q." },
    { "P = V  |  Q = V  |  R = F",
      "P AND Q OR R", "V AND V OR F  ->  V OR F  ->  V", "V",
      "P AND (Q OR R)", "V AND (V OR F)  ->  V AND V  ->  V", "V",
      "Aqui os dois cenarios coincidem, mas os passos intermediarios sao diferentes." }
};

static void render_exemplos(void) {
    char buf[200];
    const ExemploEscopo *e = &exemplos_escopo[g_exemplo_idx];
    cabecalho("LOGI - Decifra.IA", "Exemplos Didaticos: Escopo", COR_AZUL);

    snprintf(buf, sizeof(buf), "Variaveis: %s", e->variaveis);
    DrawText(buf, MARGEM, 100, 22, COR_BRANCO);

    DrawText("Cenario A:", MARGEM, 146, 22, COR_AZUL);  DrawText(e->expr_a, MARGEM + 130, 146, 22, COR_BRANCO);
    snprintf(buf, sizeof(buf), "Passos:  %s", e->passos_a);
    DrawText(buf, MARGEM + 20, 178, 20, COR_CINZA);
    snprintf(buf, sizeof(buf), "Resultado:  [%s]", e->res_a);
    DrawText(buf, MARGEM + 20, 206, 20, COR_VERDE);

    DrawText("Cenario B:", MARGEM, 256, 22, COR_AZUL);  DrawText(e->expr_b, MARGEM + 130, 256, 22, COR_BRANCO);
    snprintf(buf, sizeof(buf), "Passos:  %s", e->passos_b);
    DrawText(buf, MARGEM + 20, 288, 20, COR_CINZA);
    snprintf(buf, sizeof(buf), "Resultado:  [%s]", e->res_b);
    DrawText(buf, MARGEM + 20, 316, 20, COR_VERDE);

    DrawText("Por que muda?", MARGEM, 366, 22, COR_BRANCO);
    txt_quebra(MARGEM, 398, LARG - 2 * MARGEM, 20, COR_CINZA, e->explicacao);

    snprintf(buf, sizeof(buf), "Exemplo %d de %d", g_exemplo_idx + 1, NUM_EXEMPLOS_ESCOPO);
    DrawText(buf, MARGEM, ALT - 62, 20, COR_CINZA);
    DrawRectangle(MARGEM, ALT - 84, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);
    botao(LARG - MARGEM - 400, ALT - 72, 220, 44, "N  Proximo exemplo", COR_AZUL, 'N');
    botao(LARG - MARGEM - 170, ALT - 72, 170, 44, "ENTER  Voltar", COR_AZUL, 13);
}

void tela_exemplos_escopo(void) {
    g_render = render_exemplos;
    for (;;) {
        int c = esperar_tecla();
        if (c == 'n' || c == 'N') { g_exemplo_idx = (g_exemplo_idx + 1) % NUM_EXEMPLOS_ESCOPO; continue; }
        if (c == 13) break;
    }
}

/* ═══════════════ INTROS ═══════════════ */

static int g_intro = 0;

static void render_intro(void) {
    char buf[64];
    if (g_intro == 0) {
        cabecalho("NIVEL AVANCADO", "Escopo e Parenteses", COR_AMBAR);
        DrawText("Neste nivel os parenteses mudam quem e avaliado primeiro.", MARGEM, 110, 22, COR_BRANCO);
        DrawText("Resolva sempre de dentro para fora, seguindo a ordem:", MARGEM, 142, 22, COR_BRANCO);
        DrawText("NOT > AND > OR > IMPLICA > BICONDICIONAL", MARGEM, 196, 26, COR_AZUL);
        DrawText("Os parenteses aparecerao destacados em amarelo: ( )", MARGEM, 256, 22, COR_AMARELO);
        DrawText("Voce pode pedir ajuda a LOGI com a tecla H.", MARGEM, 310, 22, COR_BRANCO);
    } else if (g_intro == 1) {
        cabecalho("CONTRA-TEMPO", "Modo cronometrado", COR_LARANJA);
        snprintf(buf, sizeof(buf), "Voce tera %d segundos para responder cada questao.", TIMER_SEGUNDOS);
        DrawText(buf, MARGEM, 110, 22, COR_AMARELO);
        DrawText("Bonus de velocidade (apenas em acertos):", MARGEM, 160, 22, COR_BRANCO);
        DrawText("  - ate 10s  =>  +50%", MARGEM, 196, 22, COR_VERDE);
        DrawText("  - ate 15s  =>  +25%", MARGEM, 228, 22, COR_AMBAR);
        DrawText("  - acima    =>  pontos normais", MARGEM, 260, 22, COR_BRANCO);
        DrawText("Se o tempo esgotar, a questao conta como erro.", MARGEM, 312, 22, COR_BRANCO);
        DrawText("Pressionar [H] pausa o timer, mas desativa o bonus.", MARGEM, 344, 22, COR_BRANCO);
    } else {
        cabecalho("MODO SOBREVIVENCIA", "Hardcore", COR_VERMELHO);
        DrawText("Sobreviva o maximo que puder.", MARGEM, 110, 22, COR_BRANCO);
        DrawText("As questoes sao sorteadas de TODOS os niveis.", MARGEM, 142, 22, COR_BRANCO);
        DrawText("Voce tem 3 vidas.", MARGEM, 196, 22, COR_VERMELHO);
        DrawText("Cada erro subtrai 1 vida.", MARGEM, 228, 22, COR_BRANCO);
        DrawText("O jogo acaba quando as vidas zerarem.", MARGEM, 260, 22, COR_BRANCO);
        DrawText("Ganhe pontos e moedas pelos acertos!", MARGEM, 312, 22, COR_BRANCO);
    }
    rodape_enter("Pressione ENTER para comecar...");
}

/* US25: pergunta do modo Hardcore */
static void render_pergunta_hardcore(void) {
    char buf[96];
    cabecalho("MODO HARDCORE", "Desafio extra", COR_VERMELHO);
    DrawText("Jogar sem ajuda da LOGI [H] e sem pulos [P]?", MARGEM, 130, 26, COR_BRANCO);
    snprintf(buf, sizeof(buf), "Em troca, ganhe +%d%% na pontuacao final.", BONUS_HARDCORE_PCT);
    DrawText(buf, MARGEM, 176, 24, COR_AMARELO);
    botao(MARGEM,       260, 300, 56, "S  Sim, quero o desafio", COR_VERMELHO, 'S');
    botao(MARGEM + 320, 260, 260, 56, "N  Nao, modo normal",     COR_AZUL,     'N');
}

void tela_pergunta_hardcore(void) { g_render = render_pergunta_hardcore; frame(); }

void tela_intro_avancado(void)      { g_intro = 0; mostrar_e_esperar(render_intro); }
void tela_intro_contra_tempo(void)  { g_intro = 1; mostrar_e_esperar(render_intro); }
void tela_intro_sobrevivencia(void) { g_intro = 2; mostrar_e_esperar(render_intro); }

/* ═══════════════ LOADING ═══════════════ */

static double g_load_t = 0.0;

static void render_loading(void) {
    static const char *itens[4] = {
        "Inicializando sistema de logica e IA", "Carregando banco de questoes",
        "Conectando a IA tutora", "Pronto"
    };
    static const char *giro[4] = { "|", "/", "-", "\\" };
    int i;
    DrawText("LOGI - Decifra.IA", MARGEM, 60, 34, COR_AZUL);
    for (i = 0; i < 4; i++) {
        double ini = i * 0.6;
        int y = 140 + i * 48;
        if (g_load_t < ini) continue;
        if (g_load_t < ini + 0.55) {
            DrawText(giro[((int)(g_load_t * 12)) % 4], MARGEM, y, 24, COR_CINZA);
            DrawText(itens[i], MARGEM + 40, y, 24, COR_CINZA);
        } else {
            DrawText("OK", MARGEM, y, 24, COR_VERDE);
            DrawText(itens[i], MARGEM + 40, y, 24, COR_BRANCO);
        }
    }
    if (g_load_t > 2.6) {
        DrawRectangleLines(MARGEM, 360, LARG - 2 * MARGEM, 120, COR_AZUL_DIM);
        DrawText("Jogo      LOGI - Decifra.IA", MARGEM + 20, 372, 20, COR_BRANCO);
        DrawText("Versao    1.0  PI2", MARGEM + 20, 400, 20, COR_BRANCO);
        DrawText("Modo      Logica Proposicional e Seguranca em IA", MARGEM + 20, 428, 20, COR_BRANCO);
        DrawText("Status    PRONTO", MARGEM + 20, 456, 20, COR_VERDE);
        rodape_enter("Pressione ENTER para continuar...");
    }
}

void tela_loading(void) {
    double ini = GetTime();
    g_render = render_loading;
    do {
        g_load_t = GetTime() - ini;
        frame();
    } while (g_load_t <= 2.6);
    esperar_enter();
}

/* ═══════════════ NOME ═══════════════ */

static void render_nome(void) {
    ScoreEntry rk[SCORE_MAX];
    char buf[160];
    cabecalho("LOGI - Decifra.IA", "", COR_AZUL);
    DrawText("Como voce se chama?", MARGEM, 130, 28, COR_BRANCO);
    DrawRectangleLines(MARGEM, 190, 520, 50, COR_AZUL);
    snprintf(buf, sizeof(buf), "%s%s", g_edit, (((int)(GetTime() * 2)) % 2) ? "_" : "");
    DrawText(buf, MARGEM + 14, 203, 26, COR_BRANCO);
    if (score_carregar(rk, SCORE_MAX) > 0) {
        snprintf(buf, sizeof(buf), "Recorde atual: %d pts (%.20s)  - consegue superar?", rk[0].pontos, rk[0].nome);
        DrawText(buf, MARGEM, 270, 22, COR_AMARELO);
    }
    DrawText("Digite o nome e pressione ENTER.", MARGEM, ALT - 62, 20, COR_CINZA);
}

void tela_nome(void) { g_render = render_nome; frame(); }

/* ═══════════════ DESAFIOS ═══════════════ */

static void desenha_vidas(int x, int y) {
    int i;
    for (i = 0; i < 3; i++) {
        if (i < jogador.vidas) DrawCircle(x + i * 34, y, 12, COR_VERMELHO);
        else                   DrawCircleLines(x + i * 34, y, 12, COR_VERMELHO);
    }
}

static void render_desafio(void) {
    char buf[220];
    Desafio *d = &desafios[g_idx];
    int adv = (d->nivel == NIVEL_AVANCADO);
    Color cor = (g_var == 1) ? COR_LARANJA : (g_var == 2) ? COR_VERMELHO : adv ? COR_AMBAR : COR_AZUL;
    int by = ALT - 170;

    if (g_var == 2) {
        snprintf(buf, sizeof(buf), "SOBREVIVENCIA | Nv %d | Pts: %d | Moedas: %d", d->nivel, jogador.pontuacao, jogador.moedas);
    } else if (g_var == 1) {
        snprintf(buf, sizeof(buf), "CONTRA-TEMPO %d/%d | Nv %d | Pts: %d | Moedas: %d | Linha %d/%d",
                 g_idx - NUM_DESAFIOS_BASE + 1, NUM_DESAFIOS_AVANC, d->nivel, jogador.pontuacao,
                 jogador.moedas, d->linha_tabela + 1, d->total_linhas);
    } else {
        snprintf(buf, sizeof(buf), "%s %d/%d | Nv %d | Pts: %d | Moedas: %d | Linha %d/%d",
                 adv ? "AVANCADO" : "Desafio",
                 g_idx + 1 - offset_do_nivel(d->nivel), total_do_nivel(d->nivel),
                 d->nivel, jogador.pontuacao, jogador.moedas, d->linha_tabela + 1, d->total_linhas);
    }
    DrawText(buf, MARGEM, 30, 22, cor);
    DrawRectangle(MARGEM, 80, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);

    if (g_var == 2) desenha_vidas(MARGEM + 12, 104);
    if (g_var == 1) {
        float frac = (float)g_ct_segs / (float)TIMER_SEGUNDOS;
        Color ct = (g_ct_segs > 10) ? COR_VERDE : (g_ct_segs > 5) ? COR_AMBAR : COR_VERMELHO;
        if (frac > 1.0f) frac = 1.0f;
        snprintf(buf, sizeof(buf), "%2d s restantes", g_ct_segs);
        DrawText(buf, MARGEM, 96, 22, ct);
        DrawRectangleLines(MARGEM + 200, 98, 400, 18, COR_AZUL_DIM);
        DrawRectangle(MARGEM + 202, 100, (int)(396 * frac), 14, ct);
    }

    desenha_enunciado(MARGEM, 160, 34, (adv || g_var == 2));

    DrawRectangle(MARGEM, by - 20, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);
    botao(MARGEM,       by, 150, 60, "V  Verdadeiro", COR_VERDE, 'V');
    botao(MARGEM + 166, by, 150, 60, "F  Falso",      COR_VERMELHO, 'F');
    if (jogador.hardcore) {                                   /* US25 */
        DrawText("HARDCORE: sem ajuda e sem pulo", MARGEM + 340, by + 20, 20, COR_VERMELHO);
    } else {
        botao(MARGEM + 332, by, 170, 60, "H  Ajuda LOGI", COR_AZUL, 'H');
        if (g_var == 0 && !adv) botao(MARGEM + 518, by, 190, 60, "P  Pular (20 Mts)", COR_AMBAR, 'P');
    }
    if (g_var == 1)          botao(MARGEM + 518, by, 190, 60, "T  +10s (15 Mts)",  COR_AMBAR, 'T');
    DrawText("Responda com o teclado ou clicando.   Q = sair", MARGEM, by + 80, 18, COR_CINZA);
}

void tela_desafio(int indice) {
    audio_tocar(AUDIO_GAMEPLAY);
    g_idx = indice; g_var = 0;
    g_render = render_desafio; frame();
}

void tela_desafio_contra_tempo(int indice, int segs_restantes) {
    audio_tocar(AUDIO_GAMEPLAY);
    g_idx = indice; g_var = 1; g_ct_segs = segs_restantes;
    g_render = render_desafio; frame();
}

void tela_desafio_sobrevivencia(int indice) {
    audio_tocar(AUDIO_GAMEPLAY);
    g_idx = indice; g_var = 2;
    g_render = render_desafio; frame();
}

/* ═══════════════ PAINEL LOGI ═══════════════ */

static const char *dicas_l1[NUM_DESAFIOS] = {
    "AND so e verdadeiro quando os dois lados sao V.",
    "OR e verdadeiro quando pelo menos um lado e V.",
    "NOT inverte o valor logico.",
    "P IMPLICA Q e falso apenas quando P = V e Q = F.",
    "BICONDICIONAL e V quando os dois lados tem o mesmo valor.",
    "Parenteses sao avaliados antes de qualquer conectivo externo.",
    "NOT fora do parenteses so afeta o resultado interno.",
    "Comece resolvendo o interior de cada parenteses.",
    "Avalie cada parenteses separadamente e depois combine os resultados.",
    "Resolva o parenteses, depois o NOT externo, depois o OR."
};
static const char *dicas_l2[NUM_DESAFIOS] = {
    "Se qualquer lado for F, o resultado e F.",
    "So e falso quando os dois lados sao F.",
    "NOT V = F  e  NOT F = V.",
    "Em todos os outros casos o resultado e V.",
    "V BICON V = V  e  F BICON F = V.",
    "Ordem de precedencia: NOT > AND > OR > IMPLICA > BICONDICIONAL.",
    "Ex.: NOT (V AND F) = NOT F = V.",
    "Trabalhe de dentro para fora, um parenteses por vez.",
    "Combine com AND/OR/IMPLICA respeitando o resultado de cada bloco.",
    "Depois do parenteses, aplique NOT e siga a ordem normal."
};

static void render_logi(void) {
    char buf[64];
    cabecalho("LOGI", "Tutora de Logica e Seguranca em IA", COR_AZUL);
    snprintf(buf, sizeof(buf), "Dica para o desafio %d:", g_idx + 1 - offset_do_nivel(desafios[g_idx].nivel));
    DrawText(buf, MARGEM, 110, 22, COR_CINZA);
    if (g_tem_ai) {
        txt_quebra(MARGEM, 160, LARG - 2 * MARGEM, 26, COR_AZUL, g_dica_ai);
    } else {
        txt_quebra(MARGEM, 160, LARG - 2 * MARGEM, 26, COR_AZUL, dicas_l1[g_idx]);
        txt_quebra(MARGEM, 240, LARG - 2 * MARGEM, 26, COR_AZUL, dicas_l2[g_idx]);
    }
    rodape_enter("Pressione ENTER para continuar...");
}

void tela_painel_logi(int indice) {
    g_idx = indice;
    g_tem_ai = 0;
    g_dica_ai[0] = '\0';
    if (ai_consultar(desafios[indice].enunciado, g_dica_ai, sizeof(g_dica_ai)) == 0) g_tem_ai = 1;
    mostrar_e_esperar(render_logi);
}

/* ═══════════════ FEEDBACK ═══════════════ */

static void render_feedback(void) {
    char buf[160];
    Desafio *d = &desafios[g_idx];
    snprintf(buf, sizeof(buf), "Desafio %d/%d", g_idx + 1 - offset_do_nivel(d->nivel), total_do_nivel(d->nivel));
    cabecalho("LOGI", buf, COR_AZUL);
    if (g_acertou) {
        snprintf(buf, sizeof(buf), "CORRETO  +%d pontos", d->nivel * 10);
        DrawText(buf, MARGEM, 110, 34, COR_VERDE);
    } else {
        snprintf(buf, sizeof(buf), "INCORRETO  resposta correta: %c", d->resposta_correta);
        DrawText(buf, MARGEM, 110, 34, COR_VERMELHO);
    }
    DrawText("Explicacao:", MARGEM, 180, 22, COR_CINZA);
    txt_quebra(MARGEM, 214, LARG - 2 * MARGEM, 24, COR_BRANCO, d->explicacao);
    rodape_enter("Pressione ENTER para continuar...");
}

void tela_feedback(int acertou, int indice) {
    audio_tocar(acertou ? AUDIO_VITORIA : AUDIO_DERROTA);
    g_idx = indice; g_acertou = acertou;
    mostrar_e_esperar(render_feedback);
}

static void render_feedback_contra_tempo(void) {
    char buf[160];
    Desafio *d = &desafios[g_idx];
    int base = d->nivel * 10, pontos = base;

    snprintf(buf, sizeof(buf), "CONTRA-TEMPO | Desafio %d/%d", g_idx - NUM_DESAFIOS_BASE + 1, NUM_DESAFIOS_AVANC);
    cabecalho("LOGI", buf, COR_LARANJA);

    if (g_ft_timeout) {
        snprintf(buf, sizeof(buf), "TEMPO ESGOTADO  resposta correta: %c", d->resposta_correta);
        DrawText(buf, MARGEM, 110, 32, COR_VERMELHO);
    } else if (g_ft_acertou) {
        if (!g_ft_ajuda) pontos = (g_ft_usados <= BONUS_RAPIDO_SEG) ? base * 3 / 2
                                 : (g_ft_usados <= BONUS_MEDIO_SEG)  ? base * 5 / 4 : base;
        snprintf(buf, sizeof(buf), "CORRETO  +%d pontos  (%ds)", pontos, g_ft_usados);
        DrawText(buf, MARGEM, 110, 32, COR_VERDE);
        if (g_ft_ajuda) DrawText("bonus de velocidade desativado (usou [H])", MARGEM, 152, 20, COR_BRANCO);
    } else {
        snprintf(buf, sizeof(buf), "INCORRETO  resposta correta: %c  (%ds)", d->resposta_correta, g_ft_usados);
        DrawText(buf, MARGEM, 110, 32, COR_VERMELHO);
    }
    DrawText("Explicacao:", MARGEM, 196, 22, COR_CINZA);
    txt_quebra(MARGEM, 228, LARG - 2 * MARGEM, 24, COR_BRANCO, d->explicacao);
    rodape_enter("Pressione ENTER para continuar...");
}

void tela_feedback_contra_tempo(int acertou, int indice, int usados, int timeout, int usou_ajuda) {
    audio_tocar((acertou && !timeout) ? AUDIO_VITORIA : AUDIO_DERROTA);
    g_idx = indice; g_ft_acertou = acertou; g_ft_usados = usados;
    g_ft_timeout = timeout; g_ft_ajuda = usou_ajuda;
    mostrar_e_esperar(render_feedback_contra_tempo);
}

/* ═══════════════ NARRACAO DA LOGI (US21) ═══════════════ */

static const char *g_nar_tema = "";
static const char *g_nar_fala = "";
static int    g_nar_num = 0;
static double g_nar_t   = 0.0;

static void render_narracao(void) {
    char buf[120];
    float a = (float)(g_nar_t / 0.5);
    cabecalho("LOGI", "reflete entre desafios", COR_AZUL);
    snprintf(buf, sizeof(buf), "Reflexao #%d  |  %s", g_nar_num, g_nar_tema);
    DrawText(buf, MARGEM, 104, 22, COR_AMARELO);

    g_lim = (int)(g_nar_t * 55.0);           /* efeito maquina de escrever */
    txt_quebra(MARGEM, 168, LARG - 2 * MARGEM - 40, 28, COR_BRANCO, g_nar_fala);
    g_lim = -1;

    DrawRectangle(MARGEM, ALT - 84, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);
    DrawText((g_nar_t * 55.0 >= (double)strlen(g_nar_fala))
                 ? "Pressione qualquer tecla para continuar..."
                 : "qualquer tecla: pular",
             MARGEM, ALT - 62, 20, COR_CINZA);

    if (a < 1.0f) {                           /* fade-in de entrada */
        if (a < 0.0f) a = 0.0f;
        DrawRectangle(0, 0, LARG, ALT, Fade(COR_FUNDO, 1.0f - a));
    }
}

void tela_narracao_logi(const char *tema, const char *fala, int numero) {
    double ini = GetTime();
    g_nar_tema = tema; g_nar_fala = fala; g_nar_num = numero;
    g_render = render_narracao;
    limpar_stdin();
    for (;;) {
        g_nar_t = GetTime() - ini;
        g_click = 0;
        frame();
        if (g_nar_t < 0.25) { limpar_stdin(); continue; }   /* evita pular sem querer */
        if (enter_pressionado() || GetCharPressed() || g_click ||
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE)) break;
    }
}

/* ═══════════════ RESULTADO FINAL ═══════════════ */

static void render_ranking_lista(int x, int y) {
    ScoreEntry rk[SCORE_MAX];
    char buf[96];
    int n = score_carregar(rk, SCORE_MAX), i;
    DrawText("Ranking (Top 5):", x, y, 22, COR_CINZA);
    if (n == 0) { DrawText("(vazio)", x, y + 32, 22, COR_BRANCO); return; }
    for (i = 0; i < n; i++) {
        snprintf(buf, sizeof(buf), "%d. %-20.20s %5d", i + 1, rk[i].nome, rk[i].pontos);
        DrawText(buf, x, y + 32 + i * 28, 22, (i == jogador.posicao_ranking) ? COR_AMARELO : COR_BRANCO);
    }
}

/* US24: desempenho por operador, em coluna, a direita do ranking */
static void render_resumo_operadores(int x, int y) {
    int ac[OP_TOTAL], tot[OP_TOTAL], op, pior, algum = 0, yy = y + 32;
    char buf[96];

    resumo_operadores(ac, tot);
    DrawText("Desempenho por operador:", x, y, 22, COR_CINZA);
    for (op = 0; op < OP_TOTAL; op++) {
        Color c;
        if (tot[op] == 0) continue;
        c = (ac[op] == tot[op]) ? COR_VERDE : (ac[op] * 2 >= tot[op]) ? COR_AMBAR : COR_VERMELHO;
        DrawText(nome_operador(op), x, yy, 20, c);
        snprintf(buf, sizeof(buf), "%d/%d", ac[op], tot[op]);
        DrawText(buf, x + 190, yy, 20, c);
        yy += 26;
        algum = 1;
    }
    if (!algum) { DrawText("(nenhum desafio respondido)", x, yy, 20, COR_CINZA); yy += 26; }

    pior = operador_mais_fraco(ac, tot);
    if (pior >= 0) {
        snprintf(buf, sizeof(buf), "Dica: revise o operador %s na proxima jogada!", nome_operador(pior));
        txt_quebra(x, yy + 6, LARG - MARGEM - x, 18, COR_AMARELO, buf);
    } else if (algum) {
        DrawText("Mandou bem em todos os operadores!", x, yy + 6, 18, COR_VERDE);
    }
}

static void render_resultado(void) {
    char buf[96];
    cabecalho("LOGI - Decifra.IA", "Resultado Final", COR_AZUL);
    snprintf(buf, sizeof(buf), "Jogador    %s", jogador.nome);          DrawText(buf, MARGEM, 100, 24, COR_CINZA);
    snprintf(buf, sizeof(buf), "Acertos    %d de %d", jogador.acertos, g_total_rodada); DrawText(buf, MARGEM, 132, 24, COR_AZUL);
    snprintf(buf, sizeof(buf), "Pontos     %d", jogador.pontuacao);      DrawText(buf, MARGEM, 164, 24, COR_AZUL);

    if (jogador.acertos == g_total_rodada)
        DrawText("PERFEITO  Voce domina a logica proposicional!", MARGEM, 210, 24, COR_VERDE);
    else if (jogador.acertos * 2 >= g_total_rodada)
        DrawText("BOM  Continue praticando!", MARGEM, 210, 24, COR_AZUL);
    else
        DrawText("INICIANTE  Nao desista, a logica leva tempo.", MARGEM, 210, 24, COR_CINZA);

    if (jogador.posicao_ranking >= 0 && jogador.pontuacao > jogador.recorde_anterior)
        DrawText("*** NOVO RECORDE! ***", MARGEM, 244, 24, COR_AMARELO);
    else if (jogador.posicao_ranking >= 0)
        DrawText("Voce entrou no Top 5!", MARGEM, 244, 24, COR_AMARELO);

    render_ranking_lista(MARGEM, 300);
    render_resumo_operadores(LARG / 2 + 40, 300);   /* US24 */
    rodape_enter("Pressione ENTER para continuar...");
}

void tela_resultado_final(int total_rodada) {
    g_total_rodada = total_rodada;
    mostrar_e_esperar(render_resultado);
}

static void render_resultado_contra_tempo(void) {
    char buf[96];
    int i, soma = 0, n = 0, media;
    for (i = NUM_DESAFIOS_BASE; i < NUM_DESAFIOS_BASE + NUM_DESAFIOS_AVANC; i++) { soma += jogador.tempo_por_desafio[i]; n++; }
    media = (n > 0) ? soma / n : 0;

    cabecalho("LOGI - Decifra.IA", "Resultado - Contra-Tempo", COR_LARANJA);
    snprintf(buf, sizeof(buf), "Jogador       %s", jogador.nome);                         DrawText(buf, MARGEM, 100, 24, COR_CINZA);
    snprintf(buf, sizeof(buf), "Acertos       %d de %d", jogador.acertos, NUM_DESAFIOS_AVANC); DrawText(buf, MARGEM, 132, 24, COR_AZUL);
    snprintf(buf, sizeof(buf), "Pontos        %d", jogador.pontuacao);                    DrawText(buf, MARGEM, 164, 24, COR_AZUL);
    snprintf(buf, sizeof(buf), "Tempo medio   %ds por questao", media);                   DrawText(buf, MARGEM, 196, 24, COR_BRANCO);

    if (jogador.acertos == NUM_DESAFIOS_AVANC && media <= 8) DrawText("EXCELENTE  Rapido e preciso!", MARGEM, 244, 24, COR_VERDE);
    else if (jogador.acertos >= 4) DrawText("MUITO BOM  Escopo dominado, ganhe em velocidade.", MARGEM, 244, 24, COR_AZUL);
    else if (jogador.acertos >= 2) DrawText("BOM  Revise a ordem de precedencia e tente de novo.", MARGEM, 244, 24, COR_BRANCO);
    else DrawText("INICIANTE  Pratique primeiro o modo sem tempo.", MARGEM, 244, 24, COR_VERMELHO);

    if (jogador.posicao_ranking >= 0 && jogador.pontuacao > jogador.recorde_anterior)
        DrawText("*** NOVO RECORDE! ***", MARGEM, 278, 24, COR_AMARELO);
    else if (jogador.posicao_ranking >= 0)
        DrawText("Voce entrou no Top 5!", MARGEM, 278, 24, COR_AMARELO);

    render_ranking_lista(MARGEM, 320);
    rodape_enter("Pressione ENTER para continuar...");
}

void tela_resultado_contra_tempo(int total_rodada) {
    (void)total_rodada;
    mostrar_e_esperar(render_resultado_contra_tempo);
}

static void render_game_over(void) {
    char buf[64];
    cabecalho("GAME OVER", "", COR_VERMELHO);
    DrawText("Suas vidas acabaram!", MARGEM, 130, 30, COR_BRANCO);
    snprintf(buf, sizeof(buf), "Pontuacao Final: %d", jogador.pontuacao);
    DrawText(buf, MARGEM, 190, 30, COR_AMARELO);
    render_ranking_lista(MARGEM, 270);
    rodape_enter("Pressione ENTER...");
}

void tela_game_over(void) { mostrar_e_esperar(render_game_over); }

/* ═══════════════ LOJA E LOOTBOX ═══════════════ */

static const char *g_aviso = "";
static const char *g_premio = "";
static int g_lb_fase = 0;       /* 0 animando, 1 resultado */

static void render_aviso(void) {
    DrawRectangle(MARGEM, 150, LARG - 2 * MARGEM, 240, COR_PAINEL);
    DrawRectangleLines(MARGEM, 150, LARG - 2 * MARGEM, 240, COR_AMBAR);
    DrawText("AVISO", MARGEM + 30, 176, 28, COR_AMBAR);
    txt_quebra(MARGEM + 30, 230, LARG - 2 * MARGEM - 60, 26, COR_BRANCO, g_aviso);
    botao(LARG / 2 - 85, 330, 170, 44, "ENTER", COR_AZUL, 13);
}

void tela_aviso(const char *msg) { g_aviso = msg; mostrar_e_esperar(render_aviso); }

static void render_loja(void) {
    char buf[120];
    static const char *itens[4] = {
        "1    Lootbox         (30 Moedas)", "2    Dica Gratis     (15 Moedas)",
        "3    Pular Pergunta  (20 Moedas)", "4    +10 Segundos    (15 Moedas)"
    };
    int i;
    cabecalho("LOJA DE ITENS", "", rgb(150, 255, 150));
    snprintf(buf, sizeof(buf), "Saldo: %d Moedas", jogador.moedas);
    DrawText(buf, MARGEM, 100, 24, COR_AMARELO);
    snprintf(buf, sizeof(buf), "Dicas: %d  |  Pulos: %d  |  Tempo: %d", jogador.dicas_gratis, jogador.pulos_gratis, jogador.botoes_tempo);
    DrawText(buf, MARGEM, 134, 22, COR_CINZA);
    for (i = 0; i < 4; i++) botao(MARGEM, 190 + i * 56, 520, 48, itens[i], COR_AZUL, '1' + i);
    botao(MARGEM, 190 + 4 * 56, 520, 48, "5    Sair da Loja", COR_AZUL, '5');
}

void tela_loja(void) { g_render = render_loja; frame(); }

static void render_lootbox(void) {
    char buf[64];
    int dx = 0, bx = LARG / 2 - 90;
    cabecalho("LOOTBOX", "", COR_AMBAR);
    if (g_lb_fase == 0) dx = (((int)(GetTime() * 20)) % 2) ? 8 : -8;
    DrawRectangle(bx + dx, 150, 180, 130, COR_PAINEL);
    DrawRectangleLines(bx + dx, 150, 180, 130, COR_AMBAR);
    DrawText(g_lb_fase == 0 ? "?" : "*", bx + dx + 76, 190, 60, COR_AMBAR);
    if (g_lb_fase == 0) {
        DrawText("Abrindo caixa...", LARG / 2 - 100, 310, 24, COR_CINZA);
    } else {
        snprintf(buf, sizeof(buf), "VOCE GANHOU:");
        DrawText(buf, LARG / 2 - MeasureText(buf, 26) / 2, 310, 26, COR_VERDE);
        DrawText(g_premio, LARG / 2 - MeasureText(g_premio, 32) / 2, 350, 32, COR_AMARELO);
        botao(LARG / 2 - 85, 430, 170, 44, "ENTER", COR_AZUL, 13);
    }
}

void tela_lootbox_animacao(void) {
    double ini = GetTime();
    g_lb_fase = 0;
    g_render = render_lootbox;
    do { frame(); } while (GetTime() - ini < 1.5);
}

void tela_lootbox_resultado(const char *premio) {
    g_premio = premio;
    g_lb_fase = 1;
    mostrar_e_esperar(render_lootbox);
}

/* ═══════════════ RANKINGS CONSOLIDADOS (US22) ═══════════════ */

static ScoreEntry g_rk[SCORE_NUM_MODOS][SCORE_MAX];
static int  g_rk_n[SCORE_NUM_MODOS];
static int  g_rk_meu[SCORE_NUM_MODOS];
static char g_rk_nome[NOME_MAX]   = "";
static char g_rk_ultimo[NOME_MAX] = "";

static const unsigned char g_cor_modo[SCORE_NUM_MODOS][3] = {
    {100,140,255}, {255,180,60}, {255,120,60}, {220,70,70}
};

static void carregar_rankings(void) {
    char alvo[SCORE_NOME_MAX];
    int m, i;
    alvo[0] = '\0';
    if (g_rk_nome[0]) score_normalizar_nome(g_rk_nome, alvo);
    for (m = 0; m < SCORE_NUM_MODOS; m++) {
        g_rk_n[m]   = score_carregar_modo(m, g_rk[m], SCORE_MAX);   /* arquivo ausente => vazio */
        g_rk_meu[m] = -1;
        if (!alvo[0]) continue;
        for (i = 0; i < g_rk_n[m]; i++)
            if (strcasecmp(g_rk[m][i].nome, alvo) == 0) { g_rk_meu[m] = i; break; }
    }
}

static void render_rankings(void) {
    char buf[96], nome[32];
    int gap = 16, larg = (LARG - 2 * MARGEM - 3 * gap) / 4, m, i, x, y, w;

    cabecalho("LOGI - Decifra.IA", "Rankings Consolidados (Top 5)", COR_AZUL);

    for (m = 0; m < SCORE_NUM_MODOS; m++) {
        Color cor = rgb(g_cor_modo[m][0], g_cor_modo[m][1], g_cor_modo[m][2]);
        x = MARGEM + m * (larg + gap);
        DrawRectangle(x, 100, larg, 300, COR_PAINEL);
        DrawRectangle(x, 100, larg, 4, cor);

        snprintf(buf, sizeof(buf), "%s", score_nome_modo(m));
        for (i = 0; buf[i]; i++) buf[i] = (char)toupper((unsigned char)buf[i]);
        DrawText(buf, x + 10, 116, 18, cor);

        if (g_rk_n[m] == 0) { DrawText("(sem registros)", x + 10, 160, 18, COR_CINZA); continue; }

        for (i = 0; i < g_rk_n[m]; i++) {
            Color c = COR_BRANCO;
            y = 154 + i * 44;
            if (i == g_rk_meu[m]) {                       /* recorde do jogador */
                c = COR_VERDE;
                DrawRectangle(x + 4, y - 6, larg - 8, 38, rgb(24, 60, 44));
            } else if (i == 0) {                          /* recorde do modo */
                c = COR_AMARELO;
            }
            snprintf(nome, sizeof(nome), "%.9s", g_rk[m][i].nome);
            snprintf(buf, sizeof(buf), "%d  %s", i + 1, nome);
            DrawText(buf, x + 10, y, 18, c);
            snprintf(buf, sizeof(buf), "%d", g_rk[m][i].pontos);
            w = MeasureText(buf, 18);
            DrawText(buf, x + larg - 10 - w, y, 18, c);
            if (i == 0) DrawText("*", x + larg - 10 - w - 16, y, 18, c);
        }
    }

    DrawText("*  recorde do modo", MARGEM, 416, 18, COR_AMARELO);
    DrawText("destaque verde  =  seu melhor resultado", MARGEM + 220, 416, 18, COR_VERDE);

    if (g_rk_nome[0]) {
        snprintf(buf, sizeof(buf), "Seus recordes (%.20s):", g_rk_nome);
        DrawText(buf, MARGEM, 452, 20, COR_CINZA);
        x = MARGEM;
        for (m = 0; m < SCORE_NUM_MODOS; m++) {
            if (g_rk_meu[m] >= 0) snprintf(buf, sizeof(buf), "%s %d", score_nome_modo(m), g_rk[m][g_rk_meu[m]].pontos);
            else                  snprintf(buf, sizeof(buf), "%s --", score_nome_modo(m));
            DrawText(buf, x, 480, 20, g_rk_meu[m] >= 0 ? COR_VERDE : COR_CINZA);
            x += MeasureText(buf, 20) + 30;
        }
    } else {
        DrawText("Jogue uma partida ou pressione N para destacar seu nome.", MARGEM, 452, 20, COR_CINZA);
    }

    DrawRectangle(MARGEM, ALT - 84, LARG - 2 * MARGEM, 2, COR_AZUL_DIM);
    botao(LARG - MARGEM - 400, ALT - 72, 220, 44, "N  Destacar jogador", COR_AZUL, 'N');
    botao(LARG - MARGEM - 170, ALT - 72, 170, 44, "ENTER  Voltar", COR_AZUL, 13);
}

static void render_rankings_nome(void) {
    char buf[96];
    cabecalho("Rankings Consolidados", "", COR_AZUL);
    DrawText("Qual jogador destacar? (ENTER vazio remove o destaque)", MARGEM, 130, 24, COR_BRANCO);
    DrawRectangleLines(MARGEM, 190, 520, 50, COR_AZUL);
    snprintf(buf, sizeof(buf), "%s%s", g_edit, (((int)(GetTime() * 2)) % 2) ? "_" : "");
    DrawText(buf, MARGEM + 14, 203, 26, COR_BRANCO);
}

void tela_rankings_consolidados(void) {
    if (jogador.nome[0] && strcmp(jogador.nome, g_rk_ultimo) != 0) {
        snprintf(g_rk_ultimo, sizeof(g_rk_ultimo), "%s", jogador.nome);
        snprintf(g_rk_nome,   sizeof(g_rk_nome),   "%s", jogador.nome);
    }
    audio_tocar(AUDIO_MENU);
    for (;;) {
        int c;
        carregar_rankings();
        g_render = render_rankings;
        c = esperar_tecla();
        if (c == 'n' || c == 'N') {
            char nome[NOME_MAX];
            g_render = render_rankings_nome;
            editar(nome, NOME_MAX);
            snprintf(g_rk_nome, sizeof(g_rk_nome), "%s", nome);
            continue;
        }
        if (c == 13) break;
    }
}

/* ═══════════════ SAIDA ═══════════════ */

static double g_saida_t = 0.0;

static void render_saida(void) {
    float a = (float)(g_saida_t / 0.6);
    if (a > 1.0f) a = 1.0f;
    DrawText("Ate logo!", LARG / 2 - MeasureText("Ate logo!", 48) / 2, ALT / 2 - 24, 48, Fade(COR_AZUL, a));
}

void tela_saida(void) {
    double ini = GetTime();
    audio_parar();
    g_render = render_saida;
    do { g_saida_t = GetTime() - ini; frame(); } while (g_saida_t < 1.0);
}
