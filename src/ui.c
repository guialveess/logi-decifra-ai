#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "game.h"
#include "input.h"
#include "colors.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void pausa_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

static void typar(const char *s, int ms) {
    for (; *s; s++) {
        putchar((unsigned char)*s);
        fflush(stdout);
        pausa_ms(ms);
    }
}

static void linha_divisoria(void) {
    printf(COR_AZUL_DIM
        "    \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        COR_RESET "\n");
}

static void loading_item(const char *texto) {
    const char *frames[] = {
        "\xe2\xa0\x8b", "\xe2\xa0\x99", "\xe2\xa0\xb9", "\xe2\xa0\xb8",
        "\xe2\xa0\xbc", "\xe2\xa0\xb4", "\xe2\xa0\xa6", "\xe2\xa0\xa7",
        "\xe2\xa0\x87", "\xe2\xa0\x8f"
    };
    int i;
    for (i = 0; i < 12; i++) {
        printf("\r" COR_CINZA "    %s  %s" COR_RESET, frames[i % 10], texto);
        fflush(stdout);
        pausa_ms(70);
    }
    printf("\r" COR_AZUL "    \u2714   " COR_RESET "%s\n", texto);
    fflush(stdout);
}

void tela_menu(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL);
    typar("LOGI - Decifra.IA", 28);
    printf(COR_RESET COR_CINZA);
    typar("  Logica e Seguranca em IA", 12);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    " COR_AZUL "1" COR_RESET "   "); typar("Jogar\n", 18);
    printf("    " COR_AZUL "2" COR_RESET "   "); typar("Como Jogar\n", 18);
    printf("    " COR_AZUL "3" COR_RESET "   "); typar("Sair\n", 18);
    printf("\n");
    linha_divisoria();
    printf("\n");
    printf("    opcao: ");
}

void tela_como_jogar(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL);
    typar("Como Jogar", 28);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    typar("    Voce recebera desafios de logica proposicional e seguranca em IA.\n", 12);
    typar("    Para os desafios de logica, responda V (Verdadeiro) ou F (Falso).\n\n", 12);
    printf("    Durante o desafio, pressione " COR_AZUL "H" COR_RESET " ");
    typar("para pedir ajuda ao LOGI,\n", 12);
    typar("    a IA tutora do jogo.\n\n", 12);
    typar("    Operadores logicos:\n\n", 14);
    printf("      " COR_AZUL "AND" COR_RESET "            "); typar("verdadeiro quando ambos os lados sao V\n", 10);
    printf("      " COR_AZUL "OR" COR_RESET "             "); typar("verdadeiro quando pelo menos um lado e V\n", 10);
    printf("      " COR_AZUL "NOT" COR_RESET "            "); typar("inverte o valor logico\n", 10);
    printf("      " COR_AZUL "IMPLICA" COR_RESET "        "); typar("falso apenas quando P = V e Q = F\n", 10);
    printf("      " COR_AZUL "BICONDICIONAL" COR_RESET "  "); typar("verdadeiro quando os dois lados tem o mesmo valor\n\n", 10);
    typar("    Cada acerto vale pontos de acordo com o nivel do desafio.\n", 12);
    typar("    Boa sorte!\n", 14);
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_loading(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL);
    typar("LOGI - Decifra.IA", 28);
    printf(COR_RESET "\n\n");

    loading_item("Inicializando sistema de logica e IA");
    loading_item("Carregando banco de questoes");
    loading_item("Conectando a IA tutora");
    loading_item("Pronto");
    printf("\n");
    pausa_ms(200);

    printf("    " COR_AZUL_DIM
           "\u250c\u2500 sistema "
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2510" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET
           "  " COR_CINZA "Jogo    " COR_RESET "    "
           COR_AZUL); typar("LOGI - Decifra.IA", 14);
    printf(COR_RESET "                       " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET
           "  " COR_CINZA "Versao  " COR_RESET "    "
           COR_AZUL); typar("1.0  PI2", 14);
    printf(COR_RESET "                                " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET
           "  " COR_CINZA "Modo    " COR_RESET "    "
           COR_AZUL); typar("Logica Proposicional e Seguranca em IA", 14);
    printf(COR_RESET "  " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET
           "  " COR_CINZA "Status  " COR_RESET "    "
           COR_AZUL); typar("PRONTO", 14);
    printf(COR_RESET "                                  " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM
           "\u2514"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2500\u2500\u2500\u2500\u2500\u2500"
           "\u2518" COR_RESET "\n");

    aguardar_enter();
}

void tela_nome(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL);
    typar("LOGI - Decifra.IA", 28);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    typar("    Como voce se chama?\n\n", 16);
    printf("    Nome: ");
}

void tela_desafio(int indice) {
    char buf[128];
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET COR_CINZA);
    snprintf(buf, sizeof(buf), "  |  Desafio %d de %d  |  Nivel %d  |  Pontos: %d",
        indice + 1, NUM_DESAFIOS, desafios[indice].nivel, jogador.pontuacao);
    typar(buf, 8);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    " COR_AZUL);
    typar(desafios[indice].enunciado, 16);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    [" COR_AZUL "V" COR_RESET "] Verdadeiro"
           "     [" COR_AZUL "F" COR_RESET "] Falso"
           "     [" COR_AZUL "H" COR_RESET "] Pedir ajuda ao LOGI\n\n");
    printf("    resposta: ");
}

void tela_painel_logi(int indice) {
    const char *dicas[NUM_DESAFIOS] = {
        "AND so e verdadeiro quando os dois lados sao V.\n    Se qualquer lado for F, o resultado e F.",
        "OR e verdadeiro quando pelo menos um lado e V.\n    So e falso quando os dois lados sao F.",
        "NOT inverte o valor logico.\n    NOT V = F  e  NOT F = V.",
        "P IMPLICA Q e falso apenas quando P = V e Q = F.\n    Em todos os outros casos o resultado e V.",
        "BICONDICIONAL e V quando os dois lados tem o mesmo valor.\n    V BICON V = V  e  F BICON F = V."
    };

    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL);
    typar("LOGI", 28);
    printf(COR_RESET COR_CINZA);
    typar("  Tutora de Logica e Seguranca em IA", 12);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    char buf[32];
    snprintf(buf, sizeof(buf), "    Dica para o desafio %d:\n\n", indice + 1);
    typar(buf, 14);
    printf("    " COR_AZUL);
    typar(dicas[indice], 14);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_feedback(int acertou, int indice) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET COR_CINZA);
    char buf[32];
    snprintf(buf, sizeof(buf), "  |  Desafio %d de %d", indice + 1, NUM_DESAFIOS);
    typar(buf, 10);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    if (acertou) {
        printf("    " COR_VERDE);
        typar("CORRETO", 22);
        printf(COR_RESET);
        char pts[24];
        snprintf(pts, sizeof(pts), "  +%d pontos\n\n", desafios[indice].nivel * 10);
        typar(pts, 14);
    } else {
        printf("    " COR_VERMELHO);
        typar("INCORRETO", 22);
        printf(COR_RESET "  resposta correta: " COR_AZUL);
        char r[4];
        snprintf(r, sizeof(r), "%c", desafios[indice].resposta_correta);
        typar(r, 20);
        printf(COR_RESET "\n\n");
    }
    typar("    Explicacao:\n    ", 12);
    printf(COR_CINZA);
    typar(desafios[indice].explicacao, 12);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_resultado_final(void) {
    char buf[64];
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL);
    typar("LOGI - Decifra.IA", 28);
    printf(COR_RESET COR_CINZA);
    typar("  Resultado Final", 14);
    printf(COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    " COR_CINZA "Jogador" COR_RESET "    ");
    typar(jogador.nome, 14);
    printf("\n");
    printf("    " COR_CINZA "Acertos" COR_RESET "    " COR_AZUL);
    snprintf(buf, sizeof(buf), "%d", jogador.acertos);
    typar(buf, 20);
    printf(COR_RESET " de %d\n", NUM_DESAFIOS);
    printf("    " COR_CINZA "Pontos " COR_RESET "    " COR_AZUL);
    snprintf(buf, sizeof(buf), "%d", jogador.pontuacao);
    typar(buf, 20);
    printf(COR_RESET "\n\n");

    if (jogador.acertos == NUM_DESAFIOS) {
        printf("    " COR_VERDE);
        typar("PERFEITO", 22);
        printf(COR_RESET "  ");
        typar("Voce domina a logica proposicional!\n", 14);
    } else if (jogador.acertos >= 3) {
        printf("    " COR_AZUL);
        typar("BOM", 22);
        printf(COR_RESET "  ");
        typar("Continue praticando!\n", 14);
    } else {
        printf("    " COR_CINZA);
        typar("INICIANTE", 22);
        printf(COR_RESET "  ");
        typar("Nao desista, a logica leva tempo.\n", 14);
    }

    printf("\n");
    linha_divisoria();
    aguardar_enter();
}
