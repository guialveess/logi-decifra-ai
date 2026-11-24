#ifndef INPUT_H
#define INPUT_H

int  ler_opcao(void);
int  ler_opcao_menu(void);   /* digitos 1-9 ou R (devolve OPCAO_RANKINGS) */
#define OPCAO_RANKINGS 100
char ler_resposta(void);
void ler_nome(char *nome, int tamanho);
void aguardar_enter(void);
void limpar_stdin(void);

#endif
