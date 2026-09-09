#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "ui.h"
#include "input.h"

Desafio desafios[NUM_DESAFIOS] = {
    {
        "Dado P = V e Q = F,\n    qual o valor de: P AND Q?",
        'F',
        "P AND Q so e verdadeiro quando ambos sao V.\n    Como Q = F, o resultado e F.",
        1
    },
    {
        "Dado P = V e Q = V,\n    qual o valor de: P OR Q?",
        'V',
        "P OR Q e verdadeiro quando pelo menos um e V.\n    Aqui ambos sao V, entao o resultado e V.",
        1
    },
    {
        "Dado P = F,\n    qual o valor de: NOT P?",
        'V',
        "NOT inverte o valor logico.\n    NOT F = V.",
        2
    },
    {
        "Dado P = V e Q = F,\n    qual o valor de: P IMPLICA Q?",
        'F',
        "P IMPLICA Q e falso apenas quando P = V e Q = F.\n    Aqui P = V e Q = F, entao o resultado e F.",
        3
    },
    {
        "Dado P = V e Q = V,\n    qual o valor de: P BICONDICIONAL Q?",
        'V',
        "BICONDICIONAL e V quando P e Q tem o mesmo valor.\n    Ambos sao V, entao o resultado e V.",
        3
    }
};

Jogador jogador;

void inicializar_jogo(void) {
    jogador.pontuacao = 0;
    jogador.acertos = 0;
}

void executar_desafio(int indice) {
    char resposta;

    tela_desafio(indice);
    resposta = ler_resposta();

    if (resposta == 'H' || resposta == 'h') {
        tela_painel_logi(indice);
        tela_desafio(indice);
        resposta = ler_resposta();
    }

    int acertou = (toupper((unsigned char)resposta) == desafios[indice].resposta_correta);
    if (acertou) {
        jogador.acertos++;
        jogador.pontuacao += desafios[indice].nivel * 10;
    }

    tela_feedback(acertou, indice);
}

void finalizar_jogo(void) {
    tela_resultado_final();
}
