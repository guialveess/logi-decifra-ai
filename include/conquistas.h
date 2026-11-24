#ifndef CONQUISTAS_H
#define CONQUISTAS_H

/* US23: Conquistas desbloqueadas durante as partidas.
 * Ficam salvas em ~/.decifra_conquistas (uma por linha) e cada uma
 * aparece so uma vez, mesmo em partidas futuras. */

enum {
    CQ_PRIMEIRO_ACERTO = 0,   /* acertar o primeiro desafio            */
    CQ_TRIPLETE,              /* 3 acertos seguidos na mesma partida   */
    CQ_SEQUENCIA_5,           /* 5 acertos seguidos na mesma partida   */
    CQ_CENTENA,               /* chegar a 100 pontos numa partida      */
    CQ_POUPADOR,              /* juntar 50 moedas                      */
    CQ_TOTAL
};

const char *conquista_nome(int id);
const char *conquista_descricao(int id);

/* 1 se a conquista ja foi desbloqueada (nesta ou em sessoes anteriores). */
int conquista_tem(int id);

/* Marca a conquista e salva no arquivo. Devolve 1 se era nova, 0 se ja existia. */
int conquista_desbloquear(int id);

/* Confere o estado atual do jogador e desbloqueia o que for novo.
 * Os ids novos vao em 'novas' (tamanho CQ_TOTAL). Devolve quantas foram novas. */
int conquistas_verificar(int acertos, int sequencia, int pontuacao, int moedas, int *novas);

/* Le o arquivo de novo (usado pelos testes). */
void conquistas_recarregar(void);

#endif
