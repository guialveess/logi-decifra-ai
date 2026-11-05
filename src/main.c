#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "game.h"
#include "ui.h"
#include "input.h"
#include "audio.h"
#include "score.h"

#ifndef _WIN32
#include <unistd.h>
#include <termios.h>

static struct termios g_orig_term;

static void configurar_terminal(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &g_orig_term);
    raw = g_orig_term;
    raw.c_lflag &= ~(unsigned)(ECHO | ICANON);
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void restaurar_termios(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig_term);
}
#else
static void configurar_terminal(void) {}
static void restaurar_termios(void)   {}
#endif

static void restaurar_terminal(int sig) {
    (void)sig;
    audio_parar();
    restaurar_termios();
    printf("\033[?25h\033[?1049l");
    fflush(stdout);
    exit(0);
}

int main(void) {
    int opcao;
    int i;

    signal(SIGINT,  restaurar_terminal);
    signal(SIGTERM, restaurar_terminal);

    printf("\033[?1049h\033[H\033[3J\033[2J\033[H\033[?25l");
    fflush(stdout);
    configurar_terminal();
    audio_init();

    while (1) {
        tela_menu();
        opcao = ler_opcao();

        if (opcao == 8) { tela_saida(); break; }

        if (opcao == 7) { 
            score_definir_modo(SCORE_MODO_BASICO); /* Sobrevivência usa o score básico */
            jogador.recorde_anterior = score_melhor();
            jogador.posicao_ranking = -1;
            
            tela_nome();
            ler_nome(jogador.nome, NOME_MAX);
            executar_nivel_sobrevivencia();
            continue;
        }
        
        if (opcao == 6) { executar_loja(); continue; } 

        if (opcao == 5) {
            score_definir_modo(SCORE_MODO_TEMPO);
            jogador.recorde_anterior = score_melhor();
            jogador.posicao_ranking = -1;

            tela_nome();
            ler_nome(jogador.nome, NOME_MAX);
            executar_nivel_contra_tempo();
            continue;
        }

        if (opcao == 4) {
            score_definir_modo(SCORE_MODO_AVANCADO);
            jogador.recorde_anterior = score_melhor();
            jogador.posicao_ranking = -1;

            tela_nome();
            ler_nome(jogador.nome, NOME_MAX);
            executar_nivel_avancado();
            continue;
        }

        if (opcao == 3) { tela_exemplos_escopo(); continue; }
        if (opcao == 2) { tela_como_jogar();      continue; }
        
        if (opcao == 1) {
            score_definir_modo(SCORE_MODO_BASICO);
            jogador.recorde_anterior = score_melhor();
            jogador.posicao_ranking = -1;

            tela_loading();
            tela_nome();
            ler_nome(jogador.nome, NOME_MAX);
            
            int moedas_antigas = jogador.moedas;
            int dicas_antigas = jogador.dicas_gratis;
            int pulos_antigos = jogador.pulos_gratis; 
            int tempo_antigo  = jogador.botoes_tempo;

            inicializar_jogo();
            
            jogador.moedas = moedas_antigas;
            jogador.dicas_gratis = dicas_antigas;
            jogador.pulos_gratis = pulos_antigos;
            jogador.botoes_tempo = tempo_antigo; 

            for (i = 0; i < NUM_DESAFIOS_BASE; i++)
                executar_desafio(i);
            finalizar_jogo(NUM_DESAFIOS_BASE);
        }
    }

    restaurar_termios();
    audio_parar();
    printf("\033[?25h\033[?1049l");
    fflush(stdout);
    return 0;
}