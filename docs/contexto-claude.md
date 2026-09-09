# Contexto de Desenvolvimento — LOGI Decifra.IA

> Documento para novas sessões do Claude Code. Leia antes de qualquer task.

---

## O que é o projeto

Jogo educacional de terminal em C (Unidade 1) e Haskell (Unidade 2) para o Projeto Integrador 2P — E5 do CESAR School, Squad 05. O jogo ensina lógica proposicional e conscientização sobre IA, com uma tutora chamada LOGI que responde perguntas em tempo real.

Repositório: `guialveess/decifra-ai-e5`
Local: `~/Development/cesar/decifra-ai-e5`

---

## Arquitetura geral

```
┌─────────────────────────────────┐
│  jogo (C)                       │
│  main.c → game.c → ui.c         │
│                                 │
│  quando jogador pressiona H:    │
│    ai_consultar(pergunta, ...)   │ ← include/ai_client.h
│        │                        │
│        └─ popen("python3 ...") ─┼──► scripts/ai_client.py
└─────────────────────────────────┘         │
                                            ├─ tenta local (127.0.0.1:8787)
                                            │      └─ scripts/local_server.py
                                            │             (modelo carregado em MPS/CPU)
                                            │
                                            └─ fallback remoto
                                                   (HuggingFace Space Gradio)
                                                   https://guiiwfz-decifra-ia.hf.space
```

---

## Estado atual de cada frente

### 1. UI em C — CONCLUÍDA

Todas as telas implementadas com dados mockados (sem lógica real, sem IA). Fluxo completo navegável.

**Abordagem TUI:**
- Alternate screen buffer (`\033[?1049h` / `\033[?1049l`) — o jogo não suja o terminal
- Posicionamento absoluto de cursor via `\033[row;colH` para TODA saída — sem `\n` livre, zero scroll em qualquer terminal (macOS Terminal.app, iTerm2, etc.)
- Dimensões do terminal lidas em runtime com `TIOCGWINSZ` (`sys/ioctl.h`)
- Conteúdo centralizado verticalmente baseado em `g_rows`
- Fade-in simultâneo: `fade_tela()` faz 7 passos de brilho com `\033[2J\033[H` antes de cada passo
- Spinner braille animado (⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏ → ✔)
- `ler_enter()` com `termios`: desabilita ECHO+ICANON antes de aguardar ENTER (scroll do trackpad não aparece como `^[[A`)
- `tcflush(STDIN_FILENO, TCIFLUSH)` antes de cada leitura de input
- SIGINT/SIGTERM restauram o terminal corretamente

**Paleta de cores (ANSI true-color RGB):**
- Azul principal LOGI: `(100, 140, 255)`
- Azul dim/divisórias: `(60, 90, 180)`
- Cinza labels: `(120, 125, 160)`
- Branco azulado corpo: `(200, 210, 230)`
- Verde acerto: `(80, 200, 120)`
- Vermelho erro: `(220, 70, 70)`

**Telas implementadas:**
- `tela_menu()` — menu principal com opções 1/2/3
- `tela_como_jogar()` — regras e operadores lógicos
- `tela_loading()` — 4 itens animados + caixa de sistema
- `tela_nome()` — captura nome do jogador
- `tela_desafio(int)` — enunciado + prompt V/F/H
- `tela_painel_logi(int)` — dicas contextuais da LOGI
- `tela_feedback(int, int)` — resultado + explicação
- `tela_resultado_final()` — pontuação + classificação
- `tela_saida()` — animação "Até logo!" com braille fade

**Para compilar e rodar:**
```bash
cd ~/Development/cesar/decifra-ai-e5
make clean && make && ./jogo
```

---

### 2. Modelo LOGI — EM TREINAMENTO

**Base:** `Qwen/Qwen2.5-3B-Instruct`
**Adapter LoRA:** `guiiwfz/logi` (publicado no Hugging Face)
**Remote Space:** `https://guiiwfz-decifra-ia.hf.space` (Gradio)

**Sistema prompt do modelo:**
> "Você é um tutor especializado em lógica proposicional. Responda passo a passo, usando os operadores: NOT, AND, OR, IMPLICA, BICONDICIONAL. Seja claro e didático. Responda SEMPRE em português brasileiro."

**Dataset:** exercícios de lógica proposicional extraídos de `docs/proposicoes-logicas.md` e `docs/tabelas-verdade.md`. Em setembro/2026 o dataset foi expandido com novos exemplos e o modelo foi posto para retraining.

**Treinamento:** feito fora do repo (Colab / GPU externa). Quando concluído, o adapter LoRA é publicado em `guiiwfz/logi` no HuggingFace.

---

### 3. Servidor local — PRONTO, NÃO INTEGRADO AO JOGO

`scripts/local_server.py` — sobe o modelo localmente na porta 8787:

```bash
python3 scripts/local_server.py
```

- Carrega `Qwen/Qwen2.5-3B-Instruct` + LoRA `guiiwfz/logi`
- Detecta MPS (Apple Silicon) automaticamente, fallback CPU
- Endpoint: `POST http://127.0.0.1:8787/ask` com body `{"pergunta": "..."}`
- Retorna `{"resposta": "..."}`

`scripts/ai_client.py` — cliente que o jogo usa via `popen`:

```bash
python3 scripts/ai_client.py "O que é o operador AND?"
```

- Tenta local (127.0.0.1:8787) primeiro
- Fallback para o HuggingFace Space remoto (SSE / Gradio API)
- Retorna a resposta como texto puro no stdout

---

### 4. Bridge C → Python — PENDENTE

`include/ai_client.h` define a interface:
```c
#define AI_RESP_MAX 2048
int ai_consultar(const char *pergunta, char *resposta, int max_len);
```

`src/ai_client.c` (ainda não existe no repo / estava no stash local) deve implementar via `popen`:
```c
int ai_consultar(const char *pergunta, char *resposta, int max_len) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "python3 scripts/ai_client.py \"%s\" 2>/dev/null", pergunta);
    FILE *p = popen(cmd, "r");
    if (!p) { strncpy(resposta, "IA indisponivel.", max_len); return -1; }
    int n = fread(resposta, 1, max_len - 1, p);
    resposta[n > 0 ? n : 0] = '\0';
    pclose(p);
    return 0;
}
```

Quando o jogador pressiona `H` em `tela_desafio`, o `game.c` chama `tela_painel_logi()`. A intenção futura é que esse painel chame `ai_consultar()` com o enunciado do desafio, em vez de retornar dicas estáticas.

---

## Próximos passos (em ordem)

1. **Aguardar treinamento do modelo** — nova versão do adapter `guiiwfz/logi` sendo publicada no HuggingFace.

2. **Testar o servidor local:**
   ```bash
   python3 scripts/local_server.py &
   python3 scripts/ai_client.py "Dado P=V e Q=F, qual o valor de P AND Q?"
   ```

3. **Criar `src/ai_client.c`** — branch `feat/bridge-python-c` — implementar `ai_consultar()` com `popen`.

4. **Integrar no `game.c`** — quando o jogador pressiona H, chamar `ai_consultar(desafios[indice].enunciado, resp, AI_RESP_MAX)` e exibir a resposta real da LOGI na `tela_painel_logi`.

5. **Adicionar `src/ai_client.c` ao Makefile** — linha `SOURCES`:
   ```makefile
   SOURCES = src/main.c src/game.c src/ui.c src/input.c src/ai_client.c
   ```

6. **Unidade 2 (Haskell)** — motor de lógica proposicional separado, ainda não iniciado.

---

## Estrutura de arquivos

```
decifra-ai-e5/
├── src/
│   ├── main.c          # loop principal, alternate screen, signals
│   ├── game.c          # desafios mockados, pontuação, toupper()
│   ├── ui.c            # TODAS as telas, fade, TIOCGWINSZ, termios
│   └── input.c         # ler_opcao, ler_resposta, ler_nome (tcflush)
├── include/
│   ├── game.h          # structs Desafio + Jogador, NUM_DESAFIOS=5, NOME_MAX=64
│   ├── ui.h            # protótipos das telas
│   ├── input.h         # protótipos de leitura
│   ├── ai_client.h     # ai_consultar() — bridge C→Python
│   └── colors.h        # constantes ANSI (referência, não mais usado em ui.c)
├── scripts/
│   ├── ai_client.py    # cliente: tenta local, fallback remoto HF Space
│   └── local_server.py # servidor HTTP local na porta 8787 com o modelo
├── docs/
│   ├── proposicoes-logicas.md   # exercícios usados no dataset
│   ├── tabelas-verdade.md       # tabelas verdade usadas no dataset
│   └── contexto-claude.md      # ESTE ARQUIVO
├── tests/
├── Makefile            # gcc -Wall -Wextra -std=c11 -g -Iinclude
└── README.md           # visão geral pública do projeto
```

---

## Convenção de branches

| Branch | Finalidade |
|---|---|
| `main` | código estável, sempre compilável |
| `feat/ui-fade` | (mergeado) telas com fade-in e alternate screen |
| `feat/ui-fullscreen` | (mergeado) posicionamento absoluto + TIOCGWINSZ |
| `feat/bridge-python-c` | PRÓXIMO — implementar `src/ai_client.c` |

---

## Observações técnicas importantes

- Os erros do LSP (clangd "file not found") são falsos positivos — clangd não sabe do `-Iinclude`. `make` compila sem erros.
- O modelo MPS usa `float16` e `low_cpu_mem_usage=True` — não usar `device_map="auto"` no MPS.
- O HuggingFace Space usa SSE (Server-Sent Events) com `event:` e `data:` lines — o cliente já trata isso.
- A cota de GPU do Space é limitada por dia — o fallback local é a solução de longo prazo.
- Os desafios têm `\n` no campo `enunciado` — o `render_desafio()` usa posicionamento absoluto para o bloco inteiro, então o `\n` interno do enunciado ocupa r+4 e r+5 naturalmente sem causar scroll.
