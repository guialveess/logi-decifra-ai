# LOGI - Decifra.IA

<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/edbe883b-94cc-4641-b752-19e4452feeec" />

Jogo educacional de terminal que une logica proposicional e conscientizacao sobre Inteligencia Artificial.

Desenvolvido por Squad 05 como Projeto Integrador 2P - E5 no CESAR School.

Implementado em **C** na Unidade 1 e **Haskell** na Unidade 2.

---

## Sobre o Jogo

Voce esta em 2031. A IA ja permeia tudo: contrata, diagnostica, julga, governa. A maioria das pessoas apenas aceita as respostas que ela da.

Mas voce conheceu a **LOGI**.

LOGI nao e um chatbot qualquer. Ela e uma IA tutora criada para ensinar o que poucos ainda dominam: **pensar com rigor logico**. Ela acredita que a melhor defesa contra uma IA opaca e um humano que sabe raciocinar.

A cada sessao, LOGI te apresenta desafios de logica proposicional — os mesmos fundamentos matematicos que estao por tras de qualquer sistema de IA. Ela explica operadores como AND, OR, NOT, IMPLICA e BICONDICIONAL nao como curiosidades academicas, mas como ferramentas de pensamento critico para o mundo que vivemos.

Entre cada desafio, ela conta um pouco sobre como a IA funciona, onde ela erra, o que ela nao consegue fazer sozinha — e por que voce, humano, ainda importa.

**Decifra as proposicoes. Questione as maquinas. Pense por conta propria.**

---

## O que o jogo ensina

### Logica Proposicional

| Operador | Regra |
|---|---|
| `AND` | verdadeiro apenas quando ambos os lados sao V |
| `OR` | verdadeiro quando pelo menos um lado e V |
| `NOT` | inverte o valor logico |
| `IMPLICA` | falso apenas quando P = V e Q = F |
| `BICONDICIONAL` | verdadeiro quando os dois lados tem o mesmo valor |

### Conscientizacao em IA

O jogo aborda de forma narrativa temas como:

- Viesamento algoritmico — quando a IA reflete os preconceitos dos dados com que foi treinada
- Opacidade de modelos — o problema da caixa preta e a dificuldade de auditar decisoes automaticas
- Falsas certezas — por que alta acuracia em treino nao garante seguranca no mundo real
- Papel humano — o lugar do pensamento critico e da responsabilidade em sistemas hibridos

---

## Fluxo do jogo

```
Menu principal
  ├─ Jogar
  │     └─ Loading (LOGI inicializa)
  │           └─ Informe seu nome
  │                 └─ Desafio 1 → [V/F] ou [H] pedir dica ao LOGI
  │                 └─ Feedback (acerto/erro + explicacao)
  │                 └─ Desafio 2 ... Desafio 5
  │                 └─ Resultado final (pontuacao + classificacao)
  │                 └─ Volta ao menu
  ├─ Como Jogar (regras e operadores)
  └─ Sair (animacao "Ate logo!")
```

---

## Funcionalidades da UI

- Alternate screen buffer — o jogo roda como uma CLI isolada, sem rolagem do terminal
- Fade-in simultaneo em tela cheia — todas as linhas aparecem juntas com brilho crescente via ANSI true-color RGB
- Spinner braille animado (⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏ → ✔) nas telas de loading e saida
- Paleta azul derivada da identidade visual LOGI (#12122A em variantes mais claras)
- Respostas V/F sem diferenciacao de maiusculas/minusculas
- Sinal SIGINT capturado — Ctrl+C restaura o terminal corretamente

---

## Equipe

| Integrante | Frente |
|---|---|
| Guilherme Alves de Souza | Gestao Agil + Engenharia de Software |
| Hilton Resende | Gestao Agil |
| Joao Guilherme Azevedo | Gestao Agil |
| Joao Bezerra | Engenharia de Software |
| Joao Antonio Calazans | Design de Interacao (IHC) |
| Pedro Feitosa | Logica Matematica (LMC) |
| Ademir Pedro da Silva Filho | Motor em C (PIF) |
| Arthur Vieira Neiva Souza | Haskell (PIF) |
| Augusto Freitas Wanderley | Engenharia de Software |

---

## Estrutura do projeto

```
decifra-ai-e5/
├── src/
│   ├── main.c      # Ponto de entrada, loop principal, sinais
│   ├── game.c      # Logica do jogo, desafios mockados, pontuacao
│   ├── ui.c        # Todas as telas, animacoes, fade-in, paleta de cores
│   └── input.c     # Leitura de opcao, nome e resposta V/F
├── include/
│   ├── game.h      # Structs Desafio e Jogador, constantes, prototipos
│   ├── ui.h        # Prototipos das telas
│   ├── input.h     # Prototipos de leitura
│   └── colors.h    # Constantes de cor ANSI (referencia)
├── docs/           # Documentacao tecnica e decisoes de arquitetura
├── tests/          # Testes de unidade
└── Makefile        # Compilacao do projeto em C
```

---

## Como compilar e rodar

### Pre-requisitos

- gcc
- make

### Instalacao das dependencias

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential
```

**macOS:**
```bash
xcode-select --install
```

**Windows (recomendado usar WSL):**
```powershell
wsl --install
# Apos reiniciar:
sudo apt update
sudo apt install build-essential
```

### Build e execucao

```bash
make          # Compila
make run      # Compila e executa
make clean    # Limpa arquivos compilados
```

---

## Roadmap

- [x] Telas mockadas com navegacao completa (UI Unidade 1)
- [x] Animacoes fade-in simultaneo com ANSI true-color
- [x] Spinner braille e alternate screen buffer
- [ ] Integracao com modelo LOGI (bridge C e Python)
- [ ] Motor de logica proposicional em Haskell (Unidade 2)
- [ ] Banco de questoes expandido com geracao via IA
- [ ] Modo narrativo com falas contextuais da LOGI
- [ ] Historico de partidas e evolucao do jogador

---

## Convencao de branches

| Branch | Uso |
|---|---|
| `main` | Codigo estavel, entregas |
| `develop` | Integracao continua |
| `feat/nome` | Desenvolvimento de funcionalidades |

---

## Historias de Usuario

https://docs.google.com/document/d/14-Qs1IytAIkQkP5Q3xof79yqZfvHkl9T4z30iwl_f30/edit?tab=t.0

## Backlog

<img width="1920" height="968" alt="Captura de Tela (262)" src="https://github.com/user-attachments/assets/bafc4976-c9a4-4516-8d1f-60b9643f258e" />

---

## Board

<img width="1920" height="1031" alt="Captura de Tela (257)" src="https://github.com/user-attachments/assets/8975c89e-ebdf-49e8-8a69-3ed899bfeb8b" />

---

## Licenca

Projeto academico - CESAR School 2026.
