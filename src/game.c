#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "ui.h"
#include "input.h"

/* Banco de desafios: cada entrada define o enunciado exibido ao jogador,
 * a resposta correta (V ou F), a explicação exibida no feedback e
 * o nível de dificuldade (usado no cálculo de pontuação: nivel * 10). */
Desafio desafios[NUM_DESAFIOS] = {
    {
        /* Nível 1 — operador AND: verdadeiro apenas quando ambos são V */
        "Dado P = V e Q = F,\n    qual o valor de: P AND Q?",
        'F',
        "P AND Q so e verdadeiro quando ambos sao V.\n    Como Q = F, o resultado e F.",
        1
    },
    {
        /* Nível 1 — operador OR: verdadeiro quando ao menos um lado é V */
        "Dado P = V e Q = V,\n    qual o valor de: P OR Q?",
        'V',
        "P OR Q e verdadeiro quando pelo menos um e V.\n    Aqui ambos sao V, entao o resultado e V.",
        1
    },
    {
        /* Nível 2 — operador NOT: inverte o valor lógico da proposição */
        "Dado P = F,\n    qual o valor de: NOT P?",
        'V',
        "NOT inverte o valor logico.\n    NOT F = V.",
        2
    },
    {
        /* Nível 3 — operador IMPLICA: falso apenas quando P = V e Q = F */
        "Dado P = V e Q = F,\n    qual o valor de: P IMPLICA Q?",
        'F',
        "P IMPLICA Q e falso apenas quando P = V e Q = F.\n    Aqui P = V e Q = F, entao o resultado e F.",
        3
    },
    {
        /* Nível 3 — operador BICONDICIONAL: verdadeiro quando P e Q têm o mesmo valor */
        "Dado P = V e Q = V,\n    qual o valor de: P BICONDICIONAL Q?",
        'V',
        "BICONDICIONAL e V quando P e Q tem o mesmo valor.\n    Ambos sao V, entao o resultado e V.",
        3
    }
};

/* Estado global do jogador: nome, pontuação acumulada e total de acertos */
Jogador jogador;

/* Zera pontuação e acertos para iniciar uma nova partida */
void inicializar_jogo(void) {
    jogador.pontuacao = 0;
    jogador.acertos   = 0;
}

/* Executa um desafio pelo índice:
 * 1. Exibe o enunciado e aguarda V, F ou H (ajuda).
 * 2. Se H, abre o painel da tutora LOGI e relê a resposta.
 * 3. Compara com a resposta correta, atualiza pontuação e exibe feedback. */
void executar_desafio(int indice) {
    char resposta;

    tela_desafio(indice);
    resposta = ler_resposta();

    /* Jogador pediu ajuda: exibe dica da LOGI e aguarda nova resposta */
    if (resposta == 'H' || resposta == 'h') {
        tela_painel_logi(indice);
        tela_desafio(indice);
        resposta = ler_resposta();
    }

    /* Verifica acerto comparando com a resposta correta (case-insensitive) */
    int acertou = (toupper((unsigned char)resposta) == desafios[indice].resposta_correta);
    if (acertou) {
        jogador.acertos++;
        jogador.pontuacao += desafios[indice].nivel * 10; /* pontos = nível × 10 */
    }

    tela_feedback(acertou, indice); /* exibe CORRETO/INCORRETO e explicação */
}

/* Exibe a tela de resultado final e encerra a partida */
void finalizar_jogo(void) {
    tela_resultado_final();
}
