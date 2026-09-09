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
    printf("    \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\n");
}

void tela_menu(void) {
    limpar_tela();
    printf("\n");
    printf("    Decifra.IA  Logica Proposicional\n\n");
    linha_divisoria();
    printf("\n");
    printf("    1   Jogar\n");
    printf("    2   Como Jogar\n");
    printf("    3   Sair\n");
    printf("\n");
    linha_divisoria();
    printf("\n");
    printf("    opcao: ");
}

void tela_como_jogar(void) {
    limpar_tela();
    printf("\n");
    printf("    Como Jogar\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Voce recebera desafios de logica proposicional.\n");
    printf("    Para cada desafio, responda V (Verdadeiro) ou F (Falso).\n\n");
    printf("    Durante o desafio, pressione H para pedir ajuda ao LOGI,\n");
    printf("    a IA tutora do jogo.\n\n");
    printf("    Operadores logicos usados:\n\n");
    printf("      AND         verdadeiro quando ambos os lados sao V\n");
    printf("      OR          verdadeiro quando pelo menos um lado e V\n");
    printf("      NOT         inverte o valor logico\n");
    printf("      IMPLICA     falso apenas quando P = V e Q = F\n");
    printf("      BICONDICIONAL  verdadeiro quando os dois lados tem o mesmo valor\n\n");
    printf("    Cada acerto vale pontos de acordo com o nivel do desafio.\n");
    printf("    Boa sorte!\n");
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_loading(void) {
    limpar_tela();
    printf("\n");
    printf("    Decifra.IA\n\n");
    printf("    \u2714 Inicializando sistema de logica proposicional\n");
    pausa_ms(400);
    printf("    \u2714 Carregando banco de questoes\n");
    pausa_ms(400);
    printf("    \u2714 Conectando a IA tutora\n");
    pausa_ms(400);
    printf("    \u2714 Pronto\n\n");
    pausa_ms(300);

    printf("    \u250c\u2500 sistema \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n");
    printf("    \u2502  Jogo           Decifra.IA                       \u2502\n");
    printf("    \u2502  Versao         1.0  PI2                         \u2502\n");
    printf("    \u2502  Modo           Logica Proposicional             \u2502\n");
    printf("    \u2502  Status         PRONTO                           \u2502\n");
    printf("    \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n");
    aguardar_enter();
}

void tela_nome(void) {
    limpar_tela();
    printf("\n");
    printf("    Decifra.IA\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Como voce se chama?\n\n");
    printf("    Nome: ");
}

void tela_desafio(int indice) {
    limpar_tela();
    printf("\n");
    printf("    Decifra.IA  |  Desafio %d de %d  |  Nivel %d  |  Pontos: %d\n\n",
        indice + 1, NUM_DESAFIOS, desafios[indice].nivel, jogador.pontuacao);
    linha_divisoria();
    printf("\n");
    printf("    %s\n\n", desafios[indice].enunciado);
    linha_divisoria();
    printf("\n");
    printf("    [V] Verdadeiro     [F] Falso     [H] Pedir ajuda ao LOGI\n\n");
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
    printf("    LOGI  Tutora de Logica Proposicional\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Dica para o desafio %d:\n\n", indice + 1);
    printf("    %s\n", dicas[indice]);
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_feedback(int acertou, int indice) {
    limpar_tela();
    printf("\n");
    printf("    Decifra.IA  |  Desafio %d de %d\n\n", indice + 1, NUM_DESAFIOS);
    linha_divisoria();
    printf("\n");
    if (acertou) {
        printf("    CORRETO  +%d pontos\n\n", desafios[indice].nivel * 10);
    } else {
        printf("    INCORRETO  resposta correta: %c\n\n", desafios[indice].resposta_correta);
    }
    printf("    Explicacao:\n    %s\n", desafios[indice].explicacao);
    printf("\n");
    linha_divisoria();
    aguardar_enter();
}

void tela_resultado_final(void) {
    limpar_tela();
    printf("\n");
    printf("    Decifra.IA  Resultado Final\n\n");
    linha_divisoria();
    printf("\n");
    printf("    Jogador    %s\n", jogador.nome);
    printf("    Acertos    %d de %d\n", jogador.acertos, NUM_DESAFIOS);
    printf("    Pontos     %d\n\n", jogador.pontuacao);

    if (jogador.acertos == NUM_DESAFIOS) {
        printf("    Desempenho  PERFEITO  Voce domina a logica proposicional!\n");
    } else if (jogador.acertos >= 3) {
        printf("    Desempenho  BOM  Continue praticando!\n");
    } else {
        printf("    Desempenho  INICIANTE  Nao desista, a logica leva tempo.\n");
    }

    printf("\n");
    linha_divisoria();
    aguardar_enter();
}
