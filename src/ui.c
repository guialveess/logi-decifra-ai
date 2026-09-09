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

static void linha_divisoria(void) {
    printf(COR_AZUL_DIM
        "    \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
        COR_RESET "\n");
}

static void loading_item(const char *texto) {
    printf(COR_CINZA "    ..  %s" COR_RESET, texto);
    fflush(stdout);
    pausa_ms(500);
    printf("\r" COR_AZUL "    \u2714   " COR_RESET "%s\n", texto);
    fflush(stdout);
}

void tela_menu(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET
           COR_CINZA "  Logica e Seguranca em IA" COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    " COR_AZUL "1" COR_RESET "   Jogar\n");
    printf("    " COR_AZUL "2" COR_RESET "   Como Jogar\n");
    printf("    " COR_AZUL "3" COR_RESET "   Sair\n");
    printf("\n");
    linha_divisoria();
    printf("\n");
    printf("    opcao: ");
}

void tela_como_jogar(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "Como Jogar" COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Voce recebera desafios de logica proposicional e seguranca em IA.\n");
    printf("    Para os desafios de logica, responda V (Verdadeiro) ou F (Falso).\n\n");
    printf("    Durante o desafio, pressione " COR_AZUL "H" COR_RESET " para pedir ajuda ao LOGI,\n");
    printf("    a IA tutora do jogo.\n\n");
    printf("    Operadores logicos:\n\n");
    printf("      " COR_AZUL "AND" COR_RESET "            verdadeiro quando ambos os lados sao V\n");
    printf("      " COR_AZUL "OR" COR_RESET "             verdadeiro quando pelo menos um lado e V\n");
    printf("      " COR_AZUL "NOT" COR_RESET "            inverte o valor logico\n");
    printf("      " COR_AZUL "IMPLICA" COR_RESET "        falso apenas quando P = V e Q = F\n");
    printf("      " COR_AZUL "BICONDICIONAL" COR_RESET "  verdadeiro quando os dois lados tem o mesmo valor\n\n");
    printf("    Cada acerto vale pontos de acordo com o nivel do desafio.\n");
    printf("    Boa sorte!\n");
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_loading(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET "\n\n");

    loading_item("Inicializando sistema de logica e IA");
    loading_item("Carregando banco de questoes");
    loading_item("Conectando a IA tutora");
    loading_item("Pronto");
    printf("\n");
    pausa_ms(200);

    printf("    " COR_AZUL_DIM "\u250c\u2500 sistema \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET "  " COR_CINZA "Jogo    " COR_RESET "        " COR_AZUL "LOGI - Decifra.IA" COR_RESET "               " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET "  " COR_CINZA "Versao  " COR_RESET "        " COR_AZUL "1.0  PI2" COR_RESET "                        " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET "  " COR_CINZA "Modo    " COR_RESET "        " COR_AZUL "Logica Proposicional e Seguranca em IA" COR_RESET "  " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2502" COR_RESET "  " COR_CINZA "Status  " COR_RESET "        " COR_AZUL "PRONTO" COR_RESET "                          " COR_AZUL_DIM "\u2502" COR_RESET "\n");
    printf("    " COR_AZUL_DIM "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518" COR_RESET "\n");

    aguardar_enter();
}

void tela_nome(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Como voce se chama?\n\n");
    printf("    Nome: ");
}

void tela_desafio(int indice) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET
           COR_CINZA "  |  Desafio %d de %d  |  Nivel %d  |  Pontos: %d" COR_RESET "\n\n",
        indice + 1, NUM_DESAFIOS, desafios[indice].nivel, jogador.pontuacao);
    linha_divisoria();
    printf("\n");
    printf("    %s\n\n", desafios[indice].enunciado);
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
    printf("    " COR_AZUL "LOGI" COR_RESET
           COR_CINZA "  Tutora de Logica e Seguranca em IA" COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Dica para o desafio %d:\n\n", indice + 1);
    printf("    " COR_AZUL "%s" COR_RESET "\n", dicas[indice]);
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_feedback(int acertou, int indice) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET
           COR_CINZA "  |  Desafio %d de %d" COR_RESET "\n\n",
           indice + 1, NUM_DESAFIOS);
    linha_divisoria();
    printf("\n");
    if (acertou) {
        printf("    " COR_VERDE "CORRETO" COR_RESET "  +%d pontos\n\n",
               desafios[indice].nivel * 10);
    } else {
        printf("    " COR_VERMELHO "INCORRETO" COR_RESET "  resposta correta: "
               COR_AZUL "%c" COR_RESET "\n\n",
               desafios[indice].resposta_correta);
    }
    printf("    Explicacao:\n    %s\n", desafios[indice].explicacao);
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_resultado_final(void) {
    limpar_tela();
    printf("\n");
    printf("    " COR_AZUL "LOGI - Decifra.IA" COR_RESET
           COR_CINZA "  Resultado Final" COR_RESET "\n\n");
    linha_divisoria();
    printf("\n");
    printf("    " COR_CINZA "Jogador" COR_RESET "    %s\n", jogador.nome);
    printf("    " COR_CINZA "Acertos" COR_RESET "    " COR_AZUL "%d" COR_RESET " de %d\n",
           jogador.acertos, NUM_DESAFIOS);
    printf("    " COR_CINZA "Pontos " COR_RESET "    " COR_AZUL "%d" COR_RESET "\n\n",
           jogador.pontuacao);

    if (jogador.acertos == NUM_DESAFIOS) {
        printf("    " COR_VERDE "PERFEITO" COR_RESET
               "  Voce domina a logica proposicional!\n");
    } else if (jogador.acertos >= 3) {
        printf("    " COR_AZUL "BOM" COR_RESET "  Continue praticando!\n");
    } else {
        printf("    " COR_CINZA "INICIANTE" COR_RESET
               "  Nao desista, a logica leva tempo.\n");
    }

    printf("\n");
    linha_divisoria();
    aguardar_enter();
}
