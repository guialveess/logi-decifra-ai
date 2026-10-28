# Casos de Testes Lógicos

## 1. Objetivo

Este documento apresenta casos de teste lógico elaborados a partir dos quatro níveis de dificuldade do arquivo `proposicoes-logicas.md` e dos desafios 6 a 10 do nível avançado do jogo LOGI — Decifra.IA, definidos em `src/game.c`.

Para cada nível, são documentados casos contendo:

- **entrada**: valores lógicos atribuídos às proposições `P`, `Q` e, quando necessário, `R`;
- **expressão lógica**;
- **resultado esperado**;
- **justificativa**;
- **classificação do caso**, quando pertinente;
- **origem do exercício ou desafio**, quando identificável;
- **evidência e alcance da verificação**, quando houver execução do avaliador lógico.

Além dos exercícios diretamente inspirados no arquivo-base, foram incluídos alguns **casos derivados** para contemplar tautologias, contradições e uma falácia de inferência.

> **Convenção:** `V` representa Verdadeiro e `F` representa Falso.

> **Precedência adotada:** quando uma expressão não apresenta parênteses, considera-se a ordem `¬`, `∧`, `∨`, `→`, `↔`, coerente com as rotinas `eval_not`, `eval_and`, `eval_or`, `eval_impl` e `eval_expr` de `src/game.c`. Os parênteses determinam a avaliação prioritária de suas subexpressões.

## 2. Escopo e rastreabilidade

Este documento reúne dois conjuntos de casos com finalidades complementares:

| Conjunto | Origem | Finalidade |
|---|---|---|
| **Casos didáticos (Níveis 01 a 04)** | `proposicoes-logicas.md` e casos derivados, quando indicados | Demonstrar a avaliação das proposições, dos conectivos e das formas de argumento. Não pressupõem que cada fórmula esteja cadastrada no jogo. |
| **Casos implementados (AV.6 a AV.10)** | Vetor `templates` de `src/game.c`, desafios 6 a 10 | Verificar as oito valorações de cada expressão efetivamente cadastrada, confrontando a lógica clássica com uma reprodução isolada do parser recursivo. |

> **Critério de evidência:** a correção matemática das tabelas-verdade não deve ser confundida com a comprovação de execução no jogo completo. Os casos avançados possuem evidência de **teste isolado das rotinas do avaliador**, descrita nesta documentação; não foram apresentados como testes de integração ou como validação de entradas malformadas.

---

# NÍVEL 01 — Proposições Simples

Neste nível, os testes concentram-se na identificação direta do valor lógico de proposições simples e no uso básico da negação.

## Caso 1.1 — Proposição verdadeira

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V` |
| **Expressão lógica** | `P` |
| **Resultado esperado** | `V` |
| **Justificativa** | A própria entrada informa que `P` possui valor lógico verdadeiro. Portanto, a avaliação de `P` resulta diretamente em `V`. |
| **Tipo** | Proposição simples |

---

## Caso 1.2 — Proposição falsa

| Campo | Descrição |
|---|---|
| **Entrada** | `Q = F` |
| **Expressão lógica** | `Q` |
| **Resultado esperado** | `F` |
| **Justificativa** | A entrada define `Q` como falsa. Como não há conectivos, o resultado da expressão é o próprio valor atribuído a `Q`. |
| **Tipo** | Proposição simples |

---

## Caso 1.3 — Identificação de valores binários

| Campo | Descrição |
|---|---|
| **Entrada** | `P = 0`, `Q = 1` |
| **Expressão lógica** | `P`, `Q` |
| **Resultado esperado** | `P = F`, `Q = V` |
| **Justificativa** | Considerando a convenção utilizada no material, `0` representa Falso e `1` representa Verdadeiro. |
| **Tipo** | Proposições simples / representação binária |

---

## Caso 1.4 — Negação de duas proposições

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F` |
| **Expressão lógica** | `¬P`, `¬Q` |
| **Resultado esperado** | `¬P = F`, `¬Q = V` |
| **Justificativa** | A negação inverte o valor lógico da proposição. Assim, a negação de uma proposição verdadeira é falsa, enquanto a negação de uma proposição falsa é verdadeira. |
| **Tipo** | Negação |

---

## Caso 1.5 — Avaliação de proposições matemáticas simples

Considere:

- `P: 10 > 5`
- `Q: 3 = 7`
- `R: 8 < 12`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = V` |
| **Expressão lógica** | `P`, `Q`, `R` |
| **Resultado esperado** | `P = V`, `Q = F`, `R = V` |
| **Justificativa** | `10 > 5` é verdadeiro; `3 = 7` é falso; e `8 < 12` é verdadeiro. |
| **Tipo** | Proposições simples |

---

## Caso 1.6 — Avaliação de sentenças declarativas

Considere:

- `P: A água ferve a 100 °C em condições próximas ao nível do mar.`
- `Q: 2 + 2 = 5.`
- `R: Um quadrado possui quatro lados.`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = V` |
| **Expressão lógica** | `P`, `Q`, `R` |
| **Resultado esperado** | `P = V`, `Q = F`, `R = V` |
| **Justificativa** | Nas condições indicadas, `P` é considerada verdadeira; `2 + 2 = 5` é falso; e a definição de quadrado torna `R` verdadeira. |
| **Tipo** | Proposições simples |

### Observação sobre o Nível 01

Tautologias, contradições e falácias de inferência ainda não são naturais neste nível, pois os exercícios trabalham principalmente com proposições isoladas. Esses conceitos passam a ser apropriados nos níveis seguintes, quando os conectivos são introduzidos.

---

# NÍVEL 02 — Proposições com Conectivo

Neste nível, os testes utilizam os conectivos fundamentais da lógica proposicional.

## Caso 2.1 — Conjunção com uma proposição falsa

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F` |
| **Expressão lógica** | `P ∧ Q` |
| **Resultado esperado** | `F` |
| **Justificativa** | Uma conjunção somente é verdadeira quando todas as proposições envolvidas são verdadeiras. Como `Q = F`, o resultado é falso. |
| **Tipo** | Conjunção |

---

## Caso 2.2 — Disjunção com uma proposição verdadeira

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F` |
| **Expressão lógica** | `P ∨ Q` |
| **Resultado esperado** | `V` |
| **Justificativa** | A disjunção inclusiva é verdadeira quando pelo menos uma das proposições é verdadeira. Como `P = V`, o resultado é verdadeiro. |
| **Tipo** | Disjunção |

---

## Caso 2.3 — Condicional com antecedente falso

| Campo | Descrição |
|---|---|
| **Entrada** | `P = F`, `Q = V` |
| **Expressão lógica** | `P → Q` |
| **Resultado esperado** | `V` |
| **Justificativa** | Na lógica proposicional clássica, a condicional somente é falsa quando o antecedente é verdadeiro e o consequente é falso. Como `P = F`, a expressão é verdadeira. |
| **Tipo** | Condicional |

---

## Caso 2.4 — Bicondicional entre duas proposições falsas

| Campo | Descrição |
|---|---|
| **Entrada** | `P = F`, `Q = F` |
| **Expressão lógica** | `P ↔ Q` |
| **Resultado esperado** | `V` |
| **Justificativa** | A bicondicional é verdadeira quando as duas proposições possuem o mesmo valor lógico. Como ambas são falsas, o resultado é verdadeiro. |
| **Tipo** | Bicondicional |

---

## Caso 2.5 — Tautologia pela lei do terceiro excluído

> **Caso derivado a partir dos operadores estudados no arquivo.**

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V` ou `P = F` |
| **Expressão lógica** | `P ∨ ¬P` |
| **Resultado esperado** | `V` para qualquer valor de `P` |
| **Justificativa** | Se `P` for verdadeira, a primeira parcela da disjunção já torna a expressão verdadeira. Se `P` for falsa, então `¬P` será verdadeira. Logo, a expressão nunca é falsa. |
| **Tipo** | **Tautologia** |

---

## Caso 2.6 — Contradição

> **Caso derivado a partir dos operadores estudados no arquivo.**

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V` ou `P = F` |
| **Expressão lógica** | `P ∧ ¬P` |
| **Resultado esperado** | `F` para qualquer valor de `P` |
| **Justificativa** | `P` e `¬P` nunca podem ser verdadeiras simultaneamente. Portanto, a conjunção será falsa em todas as possíveis valorações de `P`. |
| **Tipo** | **Contradição** |

---

# NÍVEL 03 — Proposições com Conectivos Combinados

Neste nível, mais de um conectivo participa da mesma expressão, exigindo avaliação por etapas.

## Caso 3.1 — Conjunção seguida de disjunção

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = V` |
| **Expressão lógica** | `(P ∧ Q) ∨ R` |
| **Resultado esperado** | `V` |
| **Justificativa** | Primeiro, `P ∧ Q = V ∧ F = F`. Em seguida, `F ∨ R = F ∨ V = V`. |
| **Tipo** | Expressão composta |

---

## Caso 3.2 — Disjunção seguida de conjunção

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = F` |
| **Expressão lógica** | `(P ∨ Q) ∧ R` |
| **Resultado esperado** | `F` |
| **Justificativa** | `P ∨ Q = V ∨ F = V`. Depois, `V ∧ R = V ∧ F = F`. |
| **Tipo** | Expressão composta |

---

## Caso 3.3 — Disjunção com negação

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = V`, `R = V` |
| **Expressão lógica** | `(P ∨ Q) ∧ ¬R` |
| **Resultado esperado** | `F` |
| **Justificativa** | `P ∨ Q = V`. Como `R = V`, então `¬R = F`. Portanto, `V ∧ F = F`. |
| **Tipo** | Expressão composta com negação |

---

## Caso 3.4 — Negação dentro de uma conjunção

| Campo | Descrição |
|---|---|
| **Entrada** | `P = F`, `Q = V`, `R = F` |
| **Expressão lógica** | `(¬P ∧ Q) ∨ R` |
| **Resultado esperado** | `V` |
| **Justificativa** | Como `P = F`, temos `¬P = V`. Assim, `¬P ∧ Q = V ∧ V = V`. Por fim, `V ∨ F = V`. |
| **Tipo** | Expressão composta com negação |

---

## Caso 3.5 — Expressão baseada nas proposições textuais do arquivo

Considere:

- `P: O Brasil está localizado na América do Sul.` → `V`
- `Q: Atenas é a lendária cidade que afundou no oceano.` → `F`
- `R: Alexandre, o Grande, foi imperador de Roma.` → `F`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = F` |
| **Expressão lógica** | `(P ∧ ¬Q) ∨ R` |
| **Resultado esperado** | `V` |
| **Justificativa** | `¬Q = V`; portanto, `P ∧ ¬Q = V ∧ V = V`. Em seguida, `V ∨ F = V`. |
| **Tipo** | Expressão composta |

---

## Caso 3.6 — Falácia da negação do antecedente

> **Caso derivado.** Neste documento, a expressão “falácia do antecedente” é interpretada como **falácia da negação do antecedente**.

A forma do argumento é:

1. `P → Q`
2. `¬P`
3. Portanto, `¬Q`

Para testar a validade do argumento, pode-se representar a inferência pela fórmula:

`((P → Q) ∧ ¬P) → ¬Q`

Utilize o contraexemplo:

`P = F` e `Q = V`.

| Campo | Descrição |
|---|---|
| **Entrada** | `P = F`, `Q = V` |
| **Expressão lógica** | `((P → Q) ∧ ¬P) → ¬Q` |
| **Resultado esperado** | `F` |
| **Justificativa** | `P → Q = V`, pois o antecedente é falso. Além disso, `¬P = V`, então as duas premissas são verdadeiras. Porém, como `Q = V`, temos `¬Q = F`. Assim, existem premissas verdadeiras e conclusão falsa. Isso demonstra que a forma de argumento é inválida. |
| **Tipo** | **Falácia da negação do antecedente** |

---

## Caso 3.7 — Tautologia por simplificação da conjunção

> **Caso derivado.**

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as combinações possíveis de `P` e `Q` |
| **Expressão lógica** | `(P ∧ Q) → P` |
| **Resultado esperado** | `V` em todas as combinações |
| **Justificativa** | Sempre que `P ∧ Q` for verdadeira, necessariamente `P` também será verdadeira. Quando `P ∧ Q` for falsa, a condicional permanece verdadeira. |
| **Tipo** | **Tautologia** |

---

# NÍVEL 04 — Proposições com Conectivos Avançados

Neste nível, os casos combinam diferentes operadores, prioridades e relações mais complexas.

## Caso 4.1 — Conjunção e disjunção

A expressão original, conforme o exercício 4.1 de `proposicoes-logicas.md`, é:

`P AND Q OR R`

Pela precedência adotada:

`(P ∧ Q) ∨ R`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = V` |
| **Expressão lógica** | `(P ∧ Q) ∨ R` |
| **Resultado esperado** | `V` |
| **Justificativa** | `P ∧ Q = V ∧ F = F`. Em seguida, `F ∨ V = V`. |
| **Tipo** | Expressão composta |

---

## Caso 4.2 — Disjunção e conjunção

A expressão original é:

`P OR Q AND R`

Pela precedência adotada:

`P ∨ (Q ∧ R)`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = V`, `R = F` |
| **Expressão lógica** | `P ∨ (Q ∧ R)` |
| **Resultado esperado** | `V` |
| **Justificativa** | `Q ∧ R = V ∧ F = F`. Logo, `P ∨ F = V ∨ F = V`. |
| **Tipo** | Expressão composta |

---

## Caso 4.3 — Condicional envolvendo disjunção

A expressão original é:

`P OR Q IMPLICA R`

Pela precedência adotada:

`(P ∨ Q) → R`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = F`, `Q = F`, `R = V` |
| **Expressão lógica** | `(P ∨ Q) → R` |
| **Resultado esperado** | `V` |
| **Justificativa** | `P ∨ Q = F`. Uma condicional cujo antecedente é falso é verdadeira na lógica proposicional clássica. |
| **Tipo** | Expressão composta com condicional |

---

## Caso 4.4 — Bicondicional envolvendo conjunção

A expressão original é:

`P AND Q BICONDICIONAL R`

Pela precedência adotada:

`(P ∧ Q) ↔ R`

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = F` |
| **Expressão lógica** | `(P ∧ Q) ↔ R` |
| **Resultado esperado** | `V` |
| **Justificativa** | `P ∧ Q = F`. Como `R = F`, temos `F ↔ F`, que é verdadeiro porque os dois lados possuem o mesmo valor lógico. |
| **Tipo** | Expressão composta com bicondicional |

---

## Caso 4.5 — Teste de possível tautologia

> **Rastreabilidade:** este caso documental corresponde ao **exercício 4.8** de `proposicoes-logicas.md`, e não ao exercício 4.5 do material-base. A numeração `Caso 4.5` identifica sua posição neste documento e foi preservada para evitar alterar referências existentes.

A fórmula do exercício é:

`(P ∧ Q) ↔ (Q ∨ R)`

Utilizando os valores indicados no próprio arquivo:

`P = V`, `Q = F`, `R = V`.

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = V` |
| **Expressão lógica** | `(P ∧ Q) ↔ (Q ∨ R)` |
| **Exercício de origem** | `4.8` de `proposicoes-logicas.md` |
| **Resultado esperado** | `F` |
| **Justificativa** | `P ∧ Q = V ∧ F = F`. Já `Q ∨ R = F ∨ V = V`. Portanto, `F ↔ V = F`. Como foi encontrada uma valoração em que a fórmula é falsa, ela **não é uma tautologia**. |
| **Tipo** | Teste de tautologia / contraexemplo |

---

## Caso 4.6 — Tautologia da equivalência da condicional

> **Caso derivado.**

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as combinações possíveis de `P` e `Q` |
| **Expressão lógica** | `(P → Q) ↔ (¬P ∨ Q)` |
| **Resultado esperado** | `V` em todas as combinações |
| **Justificativa** | A condicional `P → Q` é logicamente equivalente à expressão `¬P ∨ Q`. Como os dois lados sempre apresentam o mesmo valor lógico, a bicondicional é sempre verdadeira. |
| **Tipo** | **Tautologia** |

---

## Caso 4.7 — Contradição formada por uma fórmula e sua negação

> **Caso derivado.**

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as combinações possíveis de `P` e `Q` |
| **Expressão lógica** | `(P ↔ Q) ∧ ¬(P ↔ Q)` |
| **Resultado esperado** | `F` em todas as combinações |
| **Justificativa** | Seja qual for o resultado de `P ↔ Q`, a segunda parcela será exatamente sua negação. Uma proposição e sua negação nunca podem ser verdadeiras simultaneamente. |
| **Tipo** | **Contradição** |

---

## Caso 4.8 — Falácia da negação do antecedente em forma de teste

> **Caso derivado.**

Considere novamente a inferência:

`P → Q`

`¬P`

∴ `¬Q`

Um contraexemplo suficiente é:

`P = F`, `Q = V`.

| Campo | Descrição |
|---|---|
| **Entrada** | `P = F`, `Q = V` |
| **Expressão lógica** | `((P → Q) ∧ ¬P) → ¬Q` |
| **Resultado esperado** | `F` |
| **Justificativa** | As premissas `P → Q` e `¬P` são verdadeiras, mas `¬Q` é falsa. Portanto, a conclusão não decorre necessariamente das premissas. |
| **Tipo** | **Falácia da negação do antecedente** |

---

# NÍVEL AVANÇADO DO JOGO — Desafios 6 a 10

Esta seção complementa os quatro níveis documentados anteriormente com a validação dos **desafios 6 a 10 do jogo LOGI — Decifra.IA**, definidos no vetor `templates` de [`src/game.c`](../src/game.c). São cinco expressões compostas que utilizam `P`, `Q` e `R`, com parênteses e diferentes conectivos.

> **Distinção de numeração:** os desafios 6 a 10 do jogo não correspondem aos exercícios 4.6 a 4.10 do arquivo `proposicoes-logicas.md`. A identificação `AV.6` a `AV.10` evita sobreposição com os casos já registrados no Nível 04 deste documento.

> **Critério de avaliação:** cada expressão possui três variáveis proposicionais, totalizando `2³ = 8` valorações possíveis. O resultado esperado decorre de um cálculo independente, conforme a semântica da lógica proposicional clássica; o resultado observado provém de um **teste isolado em C**, que reproduz as rotinas do avaliador presentes na versão consultada de `src/game.c`.

> **Correspondência com o jogo:** em `gerar_desafio_da_sessao`, o índice `linha_tabela` é sorteado entre `0` e `7`, e os valores das variáveis são obtidos por deslocamentos de bits. Neste documento, `linha_tabela = 4P + 2Q + R`, considerando `V = 1` e `F = 0`. Assim, `0 = FFF` e `7 = VVV`. As tabelas estão organizadas de `VVV` a `FFF`, de modo que o índice aparece em **ordem decrescente (7 a 0)**.

> **Escopo da verificação:** o teste isolado em C reproduziu `Parser`, `eval_prim`, `eval_not`, `eval_and`, `eval_or`, `eval_impl`, `eval_expr` e `avaliar_expressao` de `src/game.c` e avaliou as 40 entradas. A comparação independente foi feita com as definições clássicas dos conectivos. Como o código de produção não foi instrumentado nem executado em um teste de integração, a correspondência observada aplica-se à reprodução isolada dessas rotinas, e não equivale à aprovação de todo o jogo.

### Rastreabilidade dos desafios cadastrados

| Desafio do jogo | Identificação documental | Origem no vetor `templates` | Expressão cadastrada |
|:---:|:---:|:---:|---|
| 6 | `AV.6` | `templates[5]` | `(P OR Q) AND NOT R` |
| 7 | `AV.7` | `templates[6]` | `NOT (P AND Q) OR R` |
| 8 | `AV.8` | `templates[7]` | `P AND (Q OR NOT R)` |
| 9 | `AV.9` | `templates[8]` | `(P IMPLICA Q) AND (Q OR R)` |
| 10 | `AV.10` | `templates[9]` | `NOT (P BICONDICIONAL Q) OR R` |

---

## Caso AV.6 — Disjunção entre parênteses seguida de conjunção com negação

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as oito combinações possíveis de `P`, `Q` e `R` |
| **Origem no jogo** | `src/game.c`, `templates[5]` (desafio 6) |
| **Expressão lógica** | `(P ∨ Q) ∧ ¬R` |
| **Expressão no parser** | `(P OR Q) AND NOT R` |
| **Resultado esperado** | Conforme a coluna **Resultado esperado** da tabela-verdade, para cada valoração |
| **Justificativa** | Primeiro, avalia-se `P ∨ Q`. Em seguida, calcula-se `¬R`. A conjunção só é verdadeira quando pelo menos uma das proposições `P` ou `Q` é verdadeira e `R` é falsa. |
| **Tipo** | Expressão composta com parênteses e negação |

### Tabela-verdade completa — Desafio 6

| **`linha_tabela`** | `P` | `Q` | `R` | `P ∨ Q` | `¬R` | **Resultado esperado** | **Resultado do parser (teste isolado)** |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 7 | V | V | V | V | F | **F** | F |
| 6 | V | V | F | V | V | **V** | V |
| 5 | V | F | V | V | F | **F** | F |
| 4 | V | F | F | V | V | **V** | V |
| 3 | F | V | V | V | F | **F** | F |
| 2 | F | V | F | V | V | **V** | V |
| 1 | F | F | V | F | F | **F** | F |
| 0 | F | F | F | F | V | **F** | F |

**Verificação:** as oito valorações (`linha_tabela` de `0` a `7`) apresentam concordância entre o resultado matemático esperado e a reprodução isolada do parser (**8/8 coincidências; nenhuma divergência**).

---

## Caso AV.7 — Negação de uma conjunção seguida de disjunção

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as oito combinações possíveis de `P`, `Q` e `R` |
| **Origem no jogo** | `src/game.c`, `templates[6]` (desafio 7) |
| **Expressão lógica** | `¬(P ∧ Q) ∨ R` |
| **Expressão no parser** | `NOT (P AND Q) OR R` |
| **Resultado esperado** | Conforme a coluna **Resultado esperado** da tabela-verdade, para cada valoração |
| **Justificativa** | Primeiro, calcula-se `P ∧ Q` e nega-se seu resultado. Em seguida, aplica-se a disjunção com `R`. A fórmula é falsa apenas quando `P = V`, `Q = V` e `R = F`. |
| **Tipo** | Expressão composta com negação de parênteses |

### Tabela-verdade completa — Desafio 7

| **`linha_tabela`** | `P` | `Q` | `R` | `P ∧ Q` | `¬(P ∧ Q)` | **Resultado esperado** | **Resultado do parser (teste isolado)** |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 7 | V | V | V | V | F | **V** | V |
| 6 | V | V | F | V | F | **F** | F |
| 5 | V | F | V | F | V | **V** | V |
| 4 | V | F | F | F | V | **V** | V |
| 3 | F | V | V | F | V | **V** | V |
| 2 | F | V | F | F | V | **V** | V |
| 1 | F | F | V | F | V | **V** | V |
| 0 | F | F | F | F | V | **V** | V |

**Verificação:** as oito valorações (`linha_tabela` de `0` a `7`) apresentam concordância entre o resultado matemático esperado e a reprodução isolada do parser (**8/8 coincidências; nenhuma divergência**).

---

## Caso AV.8 — Conjunção com disjunção e negação internas

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as oito combinações possíveis de `P`, `Q` e `R` |
| **Origem no jogo** | `src/game.c`, `templates[7]` (desafio 8) |
| **Expressão lógica** | `P ∧ (Q ∨ ¬R)` |
| **Expressão no parser** | `P AND (Q OR NOT R)` |
| **Resultado esperado** | Conforme a coluna **Resultado esperado** da tabela-verdade, para cada valoração |
| **Justificativa** | Dentro dos parênteses, calcula-se `¬R` e depois `Q ∨ ¬R`. O resultado somente é verdadeiro quando `P = V` e pelo menos uma das condições `Q = V` ou `R = F` se verifica. |
| **Tipo** | Expressão composta com operadores aninhados |

### Tabela-verdade completa — Desafio 8

| **`linha_tabela`** | `P` | `Q` | `R` | `¬R` | `Q ∨ ¬R` | **Resultado esperado** | **Resultado do parser (teste isolado)** |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 7 | V | V | V | F | V | **V** | V |
| 6 | V | V | F | V | V | **V** | V |
| 5 | V | F | V | F | F | **F** | F |
| 4 | V | F | F | V | V | **V** | V |
| 3 | F | V | V | F | V | **F** | F |
| 2 | F | V | F | V | V | **F** | F |
| 1 | F | F | V | F | F | **F** | F |
| 0 | F | F | F | V | V | **F** | F |

**Verificação:** as oito valorações (`linha_tabela` de `0` a `7`) apresentam concordância entre o resultado matemático esperado e a reprodução isolada do parser (**8/8 coincidências; nenhuma divergência**).

---

## Caso AV.9 — Condicional e disjunção ligadas por conjunção

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as oito combinações possíveis de `P`, `Q` e `R` |
| **Origem no jogo** | `src/game.c`, `templates[8]` (desafio 9) |
| **Expressão lógica** | `(P → Q) ∧ (Q ∨ R)` |
| **Expressão no parser** | `(P IMPLICA Q) AND (Q OR R)` |
| **Resultado esperado** | Conforme a coluna **Resultado esperado** da tabela-verdade, para cada valoração |
| **Justificativa** | A condicional `P → Q` é falsa somente se `P = V` e `Q = F`. A expressão entre os outros parênteses, `Q ∨ R`, é verdadeira se pelo menos uma das duas proposições for verdadeira. A conjunção exige ambos os resultados verdadeiros. |
| **Tipo** | Expressão composta com condicional |

### Tabela-verdade completa — Desafio 9

| **`linha_tabela`** | `P` | `Q` | `R` | `P → Q` | `Q ∨ R` | **Resultado esperado** | **Resultado do parser (teste isolado)** |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 7 | V | V | V | V | V | **V** | V |
| 6 | V | V | F | V | V | **V** | V |
| 5 | V | F | V | F | V | **F** | F |
| 4 | V | F | F | F | F | **F** | F |
| 3 | F | V | V | V | V | **V** | V |
| 2 | F | V | F | V | V | **V** | V |
| 1 | F | F | V | V | V | **V** | V |
| 0 | F | F | F | V | F | **F** | F |

**Verificação:** as oito valorações (`linha_tabela` de `0` a `7`) apresentam concordância entre o resultado matemático esperado e a reprodução isolada do parser (**8/8 coincidências; nenhuma divergência**).

---

## Caso AV.10 — Negação da bicondicional seguida de disjunção

| Campo | Descrição |
|---|---|
| **Entrada** | Todas as oito combinações possíveis de `P`, `Q` e `R` |
| **Origem no jogo** | `src/game.c`, `templates[9]` (desafio 10) |
| **Expressão lógica** | `¬(P ↔ Q) ∨ R` |
| **Expressão no parser** | `NOT (P BICONDICIONAL Q) OR R` |
| **Resultado esperado** | Conforme a coluna **Resultado esperado** da tabela-verdade, para cada valoração |
| **Justificativa** | A bicondicional `P ↔ Q` é verdadeira quando `P` e `Q` têm o mesmo valor. Após a negação, o resultado é verdadeiro quando `P` e `Q` diferem. A disjunção com `R` também torna a expressão verdadeira sempre que `R = V`. |
| **Tipo** | Expressão composta com bicondicional e negação |

### Tabela-verdade completa — Desafio 10

| **`linha_tabela`** | `P` | `Q` | `R` | `P ↔ Q` | `¬(P ↔ Q)` | **Resultado esperado** | **Resultado do parser (teste isolado)** |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 7 | V | V | V | V | F | **V** | V |
| 6 | V | V | F | V | F | **F** | F |
| 5 | V | F | V | F | V | **V** | V |
| 4 | V | F | F | F | V | **V** | V |
| 3 | F | V | V | F | V | **V** | V |
| 2 | F | V | F | F | V | **V** | V |
| 1 | F | F | V | V | F | **V** | V |
| 0 | F | F | F | V | F | **F** | F |

**Verificação:** as oito valorações (`linha_tabela` de `0` a `7`) apresentam concordância entre o resultado matemático esperado e a reprodução isolada do parser (**8/8 coincidências; nenhuma divergência**).

---

# Verificação do Parser Recursivo — Síntese dos Resultados

O parser processa as expressões por precedência de operadores e avalia recursivamente o conteúdo dos parênteses por meio de `eval_expr`. A função `avaliar_expressao` utiliza as valorações de `P`, `Q` e `R` para calcular a resposta correta de cada desafio.

**Procedimento de validação:** para cada uma das cinco expressões cadastradas no nível avançado, foram percorridas as oito valorações possíveis de `P`, `Q` e `R` (ordem de apresentação: `VVV`, `VVF`, `VFV`, `VFF`, `FVV`, `FVF`, `FFV`, `FFF`). Foi compilado e executado um programa C de teste isolado, contendo a reprodução das rotinas do avaliador e as cinco expressões cadastradas. As saídas foram confrontadas com cálculos lógicos independentes para as 40 combinações.

A correspondência entre o índice interno do jogo e as variáveis segue o mesmo critério de `gerar_desafio_da_sessao`:

```text
linha_tabela = (P << 2) | (Q << 1) | R
P = (linha_tabela >> 2) & 1
Q = (linha_tabela >> 1) & 1
R = linha_tabela & 1
```

Os operadores `AND`, `OR`, `NOT`, `IMPLICA` e `BICONDICIONAL` foram conferidos segundo a lógica proposicional clássica. Para a condicional, adotou-se `P → Q ≡ ¬P ∨ Q`; para a bicondicional, `P ↔ Q` é verdadeira se, e somente se, os valores de `P` e `Q` forem iguais.

**Tipo de evidência:** execução do avaliador reproduzido em isolamento; não foi empregado o executável final do jogo nem a invocação direta da função estática do módulo de produção. Dessa forma, o resultado é uma confirmação da consistência lógica dos casos e das rotinas reproduzidas, não um relatório de testes automatizados integrados ao repositório.

| Desafio | Expressão lógica | Linhas verificadas | Resultados `V` | Resultados `F` | Divergências |
|---|---|:---:|:---:|:---:|:---:|
| 6 | `(P ∨ Q) ∧ ¬R` | 8/8 | 3 | 5 | 0 |
| 7 | `¬(P ∧ Q) ∨ R` | 8/8 | 7 | 1 | 0 |
| 8 | `P ∧ (Q ∨ ¬R)` | 8/8 | 3 | 5 | 0 |
| 9 | `(P → Q) ∧ (Q ∨ R)` | 8/8 | 5 | 3 | 0 |
| 10 | `¬(P ↔ Q) ∨ R` | 8/8 | 6 | 2 | 0 |

**Resultado consolidado:** **40/40 coincidências**, sem divergências entre os resultados esperados e as saídas da reprodução isolada do parser. As cinco expressões são **contingências**: apresentam ao menos uma linha verdadeira e uma linha falsa.

## Conclusão Formal sobre a Corretude

Considerando a semântica da lógica proposicional clássica e o conjunto de expressões selecionado, a análise exaustiva das oito valorações de cada desafio demonstra que os **resultados esperados estão corretos**. A execução isolada das rotinas do parser apresentou correspondência integral com esses valores.

Portanto, **as cinco tabelas-verdade estão corretas para todas as oito valorações de cada expressão**, e as **40 saídas do avaliador reproduzido em isolamento coincidem com esses resultados**. Essa correspondência oferece evidência favorável à implementação dos conectivos e ao processamento dos parênteses nas expressões dos desafios 6 a 10.

A conclusão limita-se às expressões válidas analisadas e à reprodução isolada do código consultado. **Não constitui prova de corretude geral do parser, nem comprova a execução integrada do jogo**; não foram avaliadas, nesse ensaio, expressões malformadas, outros encadeamentos de operadores ou possíveis diferenças entre versões do código.

### Limitações e verificações complementares

Como evolução futura, recomenda-se verificar diretamente as rotinas compiladas a partir de `src/game.c` e registrar testes automatizados, se houver ampliação do escopo acadêmico. Também podem ser definidos requisitos de tratamento de erros sintáticos, considerando exemplos como `P AND` (operando ausente), `(P OR Q` (parêntese não fechado), `P XOR Q` (operador não reconhecido) e `P Q` (conteúdo excedente). **Esses exemplos não foram executados nem classificados como testes aprovados neste documento.**

> **Referência técnica:** [`src/game.c`](../src/game.c), vetor `templates`, função `avaliar_expressao` e rotinas `eval_prim`, `eval_not`, `eval_and`, `eval_or`, `eval_impl` e `eval_expr`. A fonte de referência pública utilizada foi o repositório [`guialveess/logi-decifra-ai`](https://github.com/guialveess/logi-decifra-ai/blob/main/src/game.c), consultado em 28/10/2026.

---

# Resumo dos Casos Especiais

| Conceito | Exemplo utilizado | Classificação |
|---|---|---|
| Lei do terceiro excluído | `P ∨ ¬P` | Tautologia |
| Contradição direta | `P ∧ ¬P` | Contradição |
| Simplificação da conjunção | `(P ∧ Q) → P` | Tautologia |
| Equivalência da condicional | `(P → Q) ↔ (¬P ∨ Q)` | Tautologia |
| Fórmula e sua negação | `(P ↔ Q) ∧ ¬(P ↔ Q)` | Contradição |
| Negação do antecedente | `P → Q`, `¬P` ∴ `¬Q` | Falácia de inferência |

---

# Considerações Finais

Os casos de teste apresentados seguem a progressão proposta no arquivo-base:

1. **Nível 01:** identificação direta de valores lógicos e negação;
2. **Nível 02:** aplicação individual dos principais conectivos;
3. **Nível 03:** combinação de operadores e avaliação em etapas;
4. **Nível 04:** expressões mais complexas, equivalências, testes de tautologia, contradição e validade de inferências.
5. **Nível avançado do jogo:** verificação das oito valorações dos desafios 6 a 10, com tabelas-verdade completas, índices `linha_tabela` e comparação com uma reprodução isolada do parser recursivo `eval_expr`.

A presença de um único caso falso é suficiente para demonstrar que uma fórmula **não é uma tautologia**. Da mesma forma, para demonstrar que uma forma de argumento é inválida, basta encontrar um **contraexemplo** no qual todas as premissas sejam verdadeiras e a conclusão seja falsa.

Esses testes podem ser utilizados tanto como documentação didática quanto como base para validação de um programa que processe expressões da lógica proposicional clássica. A análise dos desafios 6 a 10 acrescenta evidência técnica de correspondência entre as tabelas-verdade e o avaliador reproduzido em isolamento, limitada às expressões efetivamente verificadas. Para avaliação integrada e validação de erros sintáticos, são necessários testes adicionais no código do jogo.
