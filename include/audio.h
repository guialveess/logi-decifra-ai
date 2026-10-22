#ifndef AUDIO_H
#define AUDIO_H

#define AUDIO_NENHUM   -1
#define AUDIO_MENU      0
#define AUDIO_GAMEPLAY  1
#define AUDIO_VITORIA   2
#define AUDIO_DERROTA   3

void audio_init(void);
void audio_tocar(int cenario);
void audio_parar(void);

#endif