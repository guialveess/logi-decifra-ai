# Casos de Testes Lógicos

## 1. Objetivo

Este documento apresenta casos de teste lógico elaborados a partir dos quatro níveis de dificuldade do arquivo `proposicoes-logicas.md`.

Para cada nível, são documentados casos contendo:

- **entrada**: valores lógicos atribuídos às proposições `P`, `Q` e, quando necessário, `R`;
- **expressão lógica**;
- **resultado esperado**;
- **justificativa**;
- **classificação do caso**, quando pertinente.

Além dos exercícios diretamente inspirados no arquivo-base, foram incluídos alguns **casos derivados** para contemplar tautologias, contradições e uma falácia de inferência.

> **Convenção:** `V` representa Verdadeiro e `F` representa Falso.

> **Precedência adotada:** quando uma expressão não apresenta parênteses, considera-se a ordem `¬`, `∧`, `∨`, `→`, `↔`.

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

A expressão original é:

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

A fórmula do exercício é:

`(P ∧ Q) ↔ (Q ∨ R)`

Utilizando os valores indicados no próprio arquivo:

`P = V`, `Q = F`, `R = V`.

| Campo | Descrição |
|---|---|
| **Entrada** | `P = V`, `Q = F`, `R = V` |
| **Expressão lógica** | `(P ∧ Q) ↔ (Q ∨ R)` |
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

A presença de um único caso falso é suficiente para demonstrar que uma fórmula **não é uma tautologia**. Da mesma forma, para demonstrar que uma forma de argumento é inválida, basta encontrar um **contraexemplo** no qual todas as premissas sejam verdadeiras e a conclusão seja falsa.

Esses testes podem ser utilizados tanto como documentação didática quanto como base para validação de um programa que processe expressões da lógica proposicional clássica.
