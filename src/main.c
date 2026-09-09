#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "game.h"
#include "ui.h"
#include "input.h"

static void restaurar_terminal(int sig) {
    (void)sig;
    printf("\033[?25h\033[?1049l");
    fflush(stdout);
    exit(0);
}

int main(void) {
    int opcao;
    int i;

    signal(SIGINT,  restaurar_terminal);
    signal(SIGTERM, restaurar_terminal);

    printf("\033[?1049h\033[2J\033[H\033[?25l");
    fflush(stdout);

    while (1) {
        tela_menu();
        opcao = ler_opcao();

        if (opcao == 3) {
            tela_saida();
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

    printf("\033[?25h\033[?1049l");
    fflush(stdout);

    return 0;
}
