# Roteiro de Apresentação — LOGI Decifra.AI — Banca U2

**Squad 05 | CESAR School 2026.2**

---

## Estrutura (10 min)

| Bloco | Responsável | Tempo |
|-------|-------------|-------|
| Abertura + contexto | Guilherme | 1 min |
| Demo do jogo | Guilherme | 3 min |
| Arquitetura técnica | Ademir | 2 min |
| Testes de usabilidade | João | 2 min |
| Gestão de projeto | Guilherme | 1 min |
| Encerramento | Todos | 1 min |

---

## Bloco 1 — Abertura (Guilherme)

> "Bom dia. Somos o Squad 05 e apresentamos o LOGI Decifra.AI — um jogo educacional de terminal que ensina lógica proposicional de forma interativa. Na Unidade 2 expandimos o jogo de 3 para 8 modos, integramos persistência de rankings e adicionamos a tutora LOGI como sistema de dicas."

---

## Bloco 2 — Demo do jogo (Guilherme)

**Roteiro de demonstração:**

1. Executar `make run`
2. Mostrar o menu com as 8 opções:
   - `1` Jogar (modo básico)
   - `2` Como jogar
   - `3` Exemplos de escopo
   - `4` Nível avançado
   - `5` Contra-tempo
   - `6` Loja
   - `7` Modo Sobrevivência
   - `8` Sair

3. Demonstrar **Modo Básico** — responder uma questão com `V`/`F`, pressionar `H` para ver dica da LOGI
4. Demonstrar **Modo Sobrevivência** — mostrar eliminação por erro
5. Mostrar **tela de ranking** após a partida

> "O jogo roda inteiramente no terminal, em C puro, sem dependências externas. A tutora LOGI detecta o operador lógico da questão e retorna a regra correspondente — tudo determinístico, sem chamada de rede."

---

## Bloco 3 — Arquitetura técnica (Ademir)

> "O sistema é dividido em três camadas. O núcleo em C gerencia o loop do jogo, os 5 operadores lógicos e os 4 modos. O módulo score.c persiste o top-5 de cada modo em arquivo local. E o ai_client.c entrega dicas contextuais sem popen nem servidor Python — detecção por strstr direto no binário."

**Destacar:**
- `src/score.c` — persistência independente por modo (`~/.decifra_score`, `~/.decifra_score_avancado`, `~/.decifra_score_tempo`)
- `src/ai_client.c` — dicas determinísticas baseadas nos operadores
- CI/CD — `cppcheck` integrado ao GitHub Actions

---

## Bloco 4 — Testes de usabilidade (João)

> "Realizamos testes com usuários reais aplicando as heurísticas de Nielsen. Os principais achados foram [X, Y, Z]. Os ajustes aplicados foram as mensagens de feedback de nível e a tela de ranking por modo."

---

## Bloco 5 — Gestão de projeto (Guilherme)

> "Gerenciamos o projeto em sprints quinzenais no Jira. Entregamos as US13 a US20 na Unidade 2 — oito histórias de usuário. A sprint 15e16 finaliza em 20/11 com a release tag v2.0 e a documentação completa. Para a Unidade 3 temos duas histórias planejadas no backlog: narração contextual da LOGI e rankings consolidados."

---

## Bloco 6 — Encerramento (Todos)

> "O projeto está disponível em github.com/guialveess/decifra-ai-e5. Ficamos à disposição para perguntas."
