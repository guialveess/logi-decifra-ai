#include <stdio.h>
#include "game.h"
#include "ui.h"
#include "input.h"

int main(void) {
    int opcao;
    int i;

    while (1) {
        tela_menu();
        opcao = ler_opcao();

        if (opcao == 3) {
            limpar_tela();
            printf("\n    Ate logo!\n\n");
            break;
        }

        if (opcao == 2) {
            tela_como_jogar();
            continue;
        }

        if (opcao != 1) {
            continue;
        }

        tela_loading();

        tela_nome();
        ler_nome(jogador.nome, NOME_MAX);

        inicializar_jogo();

        for (i = 0; i < NUM_DESAFIOS; i++) {
            executar_desafio(i);
        }

        finalizar_jogo();
    }

    return 0;
}
