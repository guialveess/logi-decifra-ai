#include <stdio.h>
#include <string.h>
#include "input.h"

#ifndef _WIN32
#include <unistd.h>
#include <termios.h>
static void flush_stdin(void) { tcflush(STDIN_FILENO, TCIFLUSH); }
#else
static void flush_stdin(void) {}
#endif

int ler_opcao(void) {
    int opcao = 0;
    flush_stdin();
    scanf("%d", &opcao);
    while (getchar() != '\n');
    return opcao;
}

char ler_resposta(void) {
    char r;
    flush_stdin();
    scanf(" %c", &r);
    while (getchar() != '\n');
    return r;
}

void ler_nome(char *nome, int tamanho) {
    flush_stdin();
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
