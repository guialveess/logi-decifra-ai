#ifndef NARRATIVA_H
#define NARRATIVA_H

/* US21: Narracao contextual da LOGI entre desafios. */

typedef struct {
    const char *tema;
    const char *fala;
} FalaLogi;

/* Quantidade de falas no banco. */
int narrativa_total(void);

/* Devolve a proxima fala sorteada SEM reposicao dentro da sessao
 * (execucao do programa). Quando o banco inteiro ja foi exibido,
 * ele e reembaralhado, evitando repetir a ultima fala mostrada. */
const FalaLogi *narrativa_proxima(void);

/* Quantas falas ja foram exibidas nesta sessao. */
int narrativa_exibidas(void);

#endif
