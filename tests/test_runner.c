#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "game.h"
#include "score.h"
#include "narrativa.h"
#include "conquistas.h"

static int total = 0, falhas = 0;

#define CHECK(desc, cond) do { total++; \
    if (cond) printf("  [ok]   %s\n", desc); \
    else { falhas++; printf("  [FAIL] %s\n", desc); } } while (0)

static int av(const char *e, const char *v, int a, int b, int c) {
    int vals[3]; vals[0] = a; vals[1] = b; vals[2] = c;
    return avaliar_expressao(e, v, vals, (int)strlen(v));
}

static void testes_logica(void) {
    puts("Parser logico:");
    CHECK("P AND Q (1,1) = V",              av("P AND Q", "PQ", 1, 1, 0) == 1);
    CHECK("P AND Q (1,0) = F",              av("P AND Q", "PQ", 1, 0, 0) == 0);
    CHECK("P OR Q (0,0) = F",               av("P OR Q", "PQ", 0, 0, 0) == 0);
    CHECK("NOT P (0) = V",                  av("NOT P", "P", 0, 0, 0) == 1);
    CHECK("NOT (P OR Q) (1,0) = F",         av("NOT (P OR Q)", "PQ", 1, 0, 0) == 0);
    CHECK("P IMPLICA Q (1,0) = F",          av("P IMPLICA Q", "PQ", 1, 0, 0) == 0);
    CHECK("P IMPLICA Q (0,1) = V",          av("P IMPLICA Q", "PQ", 0, 1, 0) == 1);
    CHECK("P BICONDICIONAL Q (0,0) = V",    av("P BICONDICIONAL Q", "PQ", 0, 0, 0) == 1);
    CHECK("P BICONDICIONAL Q (1,0) = F",    av("P BICONDICIONAL Q", "PQ", 1, 0, 0) == 0);
    CHECK("(P OR Q) AND NOT R (1,0,0) = V", av("(P OR Q) AND NOT R", "PQR", 1, 0, 0) == 1);
    CHECK("precedencia: V OR F AND F = V",  av("P OR Q AND R", "PQR", 1, 0, 0) == 1);
    CHECK("parenteses: (V OR F) AND F = F", av("(P OR Q) AND R", "PQR", 1, 0, 0) == 0);
}

static void testes_narrativa(void) {
    int n = narrativa_total(), i, j, ok = 1, rodada2 = 1;
    const FalaLogi *vistas[64];
    const FalaLogi *ultima;
    puts("Narracao da LOGI (US21):");
    CHECK("banco com 10+ falas", n >= 10);
    for (i = 0; i < n; i++) {
        vistas[i] = narrativa_proxima();
        for (j = 0; j < i; j++) if (vistas[j] == vistas[i]) ok = 0;
    }
    CHECK("sem repeticao ate esgotar o banco", ok);
    ultima = vistas[n - 1];
    CHECK("proxima rodada nao comeca repetindo a ultima fala", narrativa_proxima() != ultima);
    (void)rodada2;
    CHECK("falas nao vazias", vistas[0]->fala[0] && vistas[0]->tema[0]);
}

/* ── Loja (HU14, HU16, HU17, HU18) ──────────────────────────────────────
 * A loja le opcoes do teclado e mostra telas. O teste simula as teclas
 * por um arquivo (stdin) e descarta a saida de tela (stdout), depois
 * confere moedas e inventario. Opcoes: 2 dica, 3 pulo, 4 tempo, 5 sair.
 * Cada compra/aviso exibe uma tela que espera ENTER ("\n"). */
static void loja_com_teclas(const char *teclas) {
    char arq[] = "/tmp/decifra_lojaXXXXXX";
    int fd = mkstemp(arq), out, nul;
    if (fd < 0) return;
    if (write(fd, teclas, strlen(teclas)) < 0) { close(fd); return; }
    close(fd);

    fflush(stdout);
    out = dup(STDOUT_FILENO);
    nul = open("/dev/null", O_WRONLY);
    dup2(nul, STDOUT_FILENO);

    if (freopen(arq, "r", stdin)) executar_loja();

    fflush(stdout);
    dup2(out, STDOUT_FILENO);
    close(out); close(nul);
    unlink(arq);
}

static void zerar_inventario(int moedas) {
    jogador.moedas = moedas;
    jogador.dicas_gratis = 0;
    jogador.pulos_gratis = 0;
    jogador.botoes_tempo = 0;
}

static void testes_loja(void) {
    puts("Loja (HU14/HU16-HU18):");

    zerar_inventario(100);
    loja_com_teclas("2\n5");
    CHECK("dica: cobra 15 moedas", jogador.moedas == 100 - CUSTO_DICA);
    CHECK("dica: soma 1 dica gratis", jogador.dicas_gratis == 1);

    zerar_inventario(100);
    loja_com_teclas("3\n5");
    CHECK("pulo: cobra 20 moedas", jogador.moedas == 100 - CUSTO_PULO);
    CHECK("pulo: soma 1 pulo", jogador.pulos_gratis == 1);

    zerar_inventario(100);
    loja_com_teclas("4\n5");
    CHECK("tempo: cobra 15 moedas", jogador.moedas == 100 - CUSTO_TEMPO);
    CHECK("tempo: soma 1 botao de tempo", jogador.botoes_tempo == 1);

    zerar_inventario(100);
    loja_com_teclas("2\n3\n4\n5");
    CHECK("varias compras: saldo final correto",
          jogador.moedas == 100 - CUSTO_DICA - CUSTO_PULO - CUSTO_TEMPO);

    zerar_inventario(10);
    loja_com_teclas("2\n3\n4\n1\n5");
    CHECK("saldo insuficiente: moedas nao mudam", jogador.moedas == 10);
    CHECK("saldo insuficiente: nada entra no inventario",
          jogador.dicas_gratis == 0 && jogador.pulos_gratis == 0 && jogador.botoes_tempo == 0);

    zerar_inventario(CUSTO_DICA);
    loja_com_teclas("2\n2\n5");
    CHECK("saldo exato: compra 1 vez e recusa a 2a", jogador.dicas_gratis == 1 && jogador.moedas == 0);
}

/* ── Conquistas (US23) ─────────────────────────────────────────────── */
static void testes_conquistas(void) {
    char dir[] = "/tmp/decifra_cqXXXXXX";
    int novas[CQ_TOTAL], n;
    puts("Conquistas (US23):");
    if (!mkdtemp(dir)) { puts("  [FAIL] mkdtemp"); falhas++; total++; return; }
    setenv("HOME", dir, 1);          /* nao mexe nas conquistas reais */
    conquistas_recarregar();

    CHECK("comeca sem conquistas", !conquista_tem(CQ_PRIMEIRO_ACERTO) && !conquista_tem(CQ_TRIPLETE));

    n = conquistas_verificar(0, 0, 0, 0, novas);
    CHECK("sem acertos: nada desbloqueia", n == 0);

    n = conquistas_verificar(1, 1, 10, 2, novas);
    CHECK("primeiro acerto desbloqueia", n == 1 && novas[0] == CQ_PRIMEIRO_ACERTO);

    n = conquistas_verificar(2, 2, 20, 4, novas);
    CHECK("2 seguidos: ainda nao e Triplete", n == 0);

    n = conquistas_verificar(3, 3, 30, 6, novas);
    CHECK("3 seguidos: Triplete", n == 1 && novas[0] == CQ_TRIPLETE);

    n = conquistas_verificar(3, 3, 30, 6, novas);
    CHECK("conquista nao aparece duas vezes", n == 0);

    n = conquistas_verificar(10, 5, 120, 60, novas);
    CHECK("5 seguidos + 100 pts + 50 moedas: 3 novas de uma vez", n == 3);

    conquistas_recarregar();         /* simula fechar e abrir o jogo */
    CHECK("conquistas ficam salvas no arquivo",
          conquista_tem(CQ_PRIMEIRO_ACERTO) && conquista_tem(CQ_TRIPLETE) && conquista_tem(CQ_POUPADOR));
}

/* ── Resumo por operador (US24) ────────────────────────────────────── */
static void marca_desafio(int i, const char *expr, int acertou) {
    snprintf(desafios[i].expressao, sizeof(desafios[i].expressao), "%s", expr);
    jogador.resposta_jogador[i] = 'V';
    jogador.acertou[i] = acertou;
}

static void testes_resumo_operadores(void) {
    int ac[OP_TOTAL], tot[OP_TOTAL], i;
    puts("Resumo por operador (US24):");

    for (i = 0; i < NUM_DESAFIOS; i++) { jogador.resposta_jogador[i] = 0; jogador.acertou[i] = 0; }
    resumo_operadores(ac, tot);
    CHECK("sem respostas: nenhum operador contado", tot[OP_AND] == 0 && tot[OP_OR] == 0);
    CHECK("sem respostas: nenhum operador fraco", operador_mais_fraco(ac, tot) == -1);

    marca_desafio(0, "P AND Q", 1);
    marca_desafio(1, "P OR Q", 0);
    marca_desafio(2, "NOT (P OR Q)", 1);
    marca_desafio(3, "P IMPLICA Q", 1);
    marca_desafio(4, "P BICONDICIONAL Q", 0);
    resumo_operadores(ac, tot);

    CHECK("AND: 1 de 1", ac[OP_AND] == 1 && tot[OP_AND] == 1);
    CHECK("OR aparece em 2 desafios, 1 acerto", ac[OP_OR] == 1 && tot[OP_OR] == 2);
    CHECK("NOT: 1 de 1", ac[OP_NOT] == 1 && tot[OP_NOT] == 1);
    CHECK("BICONDICIONAL nao conta como OR nem como NOT", tot[OP_BICONDICIONAL] == 1);
    CHECK("pior operador: BICONDICIONAL (0%)", operador_mais_fraco(ac, tot) == OP_BICONDICIONAL);

    marca_desafio(4, "P BICONDICIONAL Q", 1);
    resumo_operadores(ac, tot);
    CHECK("depois de corrigir: pior passa a ser OR (50%)", operador_mais_fraco(ac, tot) == OP_OR);

    marca_desafio(1, "P OR Q", 1);
    resumo_operadores(ac, tot);
    CHECK("tudo certo: nenhum operador fraco", operador_mais_fraco(ac, tot) == -1);
}

/* ── Modo Hardcore (US25) ──────────────────────────────────────────── */
static void testes_hardcore(void) {
    puts("Modo Hardcore (US25):");
    CHECK("bonus de 50%: 100 -> 150", pontos_com_bonus_hardcore(100) == 150);
    CHECK("bonus de 50%: 70 -> 105",  pontos_com_bonus_hardcore(70) == 105);
    CHECK("sem pontos, sem bonus",    pontos_com_bonus_hardcore(0) == 0);
    jogador.hardcore = 1;
    inicializar_jogo();
    CHECK("nova partida comeca fora do Hardcore", jogador.hardcore == 0);
}

/* ── Rankings (US22) ──────────────────────────────────────────────── */
static void testes_score(void) {
    char dir[] = "/tmp/decifra_testeXXXXXX";
    char path[600];
    ScoreEntry e[SCORE_MAX];
    FILE *f;
    int m;
    puts("Rankings (US22):");
    if (!mkdtemp(dir)) { puts("  [FAIL] mkdtemp"); falhas++; total++; return; }
    setenv("HOME", dir, 1);

    for (m = 0; m < SCORE_NUM_MODOS; m++)
        CHECK("sem arquivo => 0 entradas", score_carregar_modo(m, e, SCORE_MAX) == 0);

    snprintf(path, sizeof(path), "%s/.decifra_score", dir);
    f = fopen(path, "w");
    fputs("Ana|40\nlinha sem separador\nBob|abc\nCaio|120\nBia|60\n", f);
    fclose(f);
    CHECK("ignora linhas corrompidas (3 validas)", score_carregar_modo(SCORE_MODO_BASICO, e, SCORE_MAX) == 3);
    CHECK("ordena por pontos (Caio primeiro)", strcmp(e[0].nome, "Caio") == 0 && e[0].pontos == 120);
    CHECK("outro modo ausente continua vazio", score_carregar_modo(SCORE_MODO_AVANCADO, e, SCORE_MAX) == 0);

    score_definir_modo(SCORE_MODO_SOBREVIVENCIA);
    CHECK("sobrevivencia grava no proprio arquivo", score_inserir("Ze", 70) == 0);
    CHECK("sobrevivencia lida por modo", score_carregar_modo(SCORE_MODO_SOBREVIVENCIA, e, SCORE_MAX) == 1);
    CHECK("basico nao foi alterado", score_carregar_modo(SCORE_MODO_BASICO, e, SCORE_MAX) == 3);
    CHECK("pontuacao 0 nao entra no ranking", score_inserir("Zero", 0) == -1);
}

int main(void) {
    testes_logica();
    testes_narrativa();
    testes_score();
    testes_loja();
    testes_conquistas();
    testes_resumo_operadores();
    testes_hardcore();
    printf("\n%d testes, %d falha(s)\n", total, falhas);
    return falhas ? 1 : 0;
}