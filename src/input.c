#include <stdio.h>
#include <string.h>
#include "input.h"

int ler_opcao(void) {
    int opcao = 0;
    scanf("%d", &opcao);
    while (getchar() != '\n');
    return opcao;
}

char ler_resposta(void) {
    char r;
    scanf(" %c", &r);
    while (getchar() != '\n');
    return r;
}

void ler_nome(char *nome, int tamanho) {
    fgets(nome, tamanho, stdin);
    int len = (int)strlen(nome);
    if (len > 0 && nome[len - 1] == '\n') {
        nome[len - 1] = '\0';
    }
}

void aguardar_enter(void) {
    printf("\n    Pressione ENTER para continuar...");
    getchar();
}
