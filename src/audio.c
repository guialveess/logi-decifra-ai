#include <stdio.h>
#include <stdlib.h>
#include "audio.h"

static int audio_ativo = 0;
static int faixa_atual = AUDIO_NENHUM;

void audio_init(void) {
    /* Verifica se o ffplay esta instalado */
    if (system("which ffplay > /dev/null 2>&1") == 0) {
        audio_ativo = 1;
    } else {
        audio_ativo = 0; /* Roda silenciosamente se nao tiver audio */
    }
}

void audio_parar(void) {
    if (!audio_ativo) return;
    /* Mata apenas os processos do ffplay que estao tocando nossa pasta */
    system("pkill -f 'ffplay.*assets/audio' > /dev/null 2>&1");
    faixa_atual = AUDIO_NENHUM;
}

void audio_tocar(int cenario) {
    if (!audio_ativo || faixa_atual == cenario) return;
    
    audio_parar();
    
    char cmd[256];
    const char *arquivo = "";
    
    switch (cenario) {
        case AUDIO_MENU:     arquivo = "assets/audio/menu.mp3"; break;
        case AUDIO_GAMEPLAY: arquivo = "assets/audio/gameplay.mp3"; break;
        case AUDIO_VITORIA:  arquivo = "assets/audio/win.mp3"; break;
        case AUDIO_DERROTA:  arquivo = "assets/audio/lose.mp3"; break;
        default: return;
    }
    
    if (arquivo[0] != '\0') {
        /* -nodisp (sem video) | -autoexit (fecha ao fim) | -loop 0 (loop infinito) */
        snprintf(cmd, sizeof(cmd), 
                 "SDL_AUDIODRIVER=pulseaudio ffplay -nodisp -autoexit -loglevel quiet -loop 0 %s > /dev/null 2>&1 &", 
                 arquivo);
        system(cmd);
        faixa_atual = cenario;
    }
}