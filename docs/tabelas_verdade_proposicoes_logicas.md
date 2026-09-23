# Tabelas-Verdade — Exercícios de Proposições Lógicas

Este documento reúne uma tabela-verdade para **cada questão** do arquivo fornecido.

> **Convenções:** V = Verdadeiro; F = Falso. Nas questões sem conectivo, a tabela apenas explicita os valores possíveis das proposições. Nas questões com três proposições, são mostradas as oito combinações possíveis.

> **Precedência adotada nas expressões sem parênteses do Nível 04:** ¬, ∧, ∨, →, ↔. Assim, por exemplo, `P AND Q OR R` é lido como `(P ∧ Q) ∨ R`.

## Nível 01

### Questão 1.1

**Expressão analisada:** `P`

| P | P |
|:---:|:---:|
| V | V |
| F | F |

### Questão 1.2

**Expressão analisada:** `Q`

| Q | Q |
|:---:|:---:|
| V | V |
| F | F |

### Questão 1.3

**Expressão analisada:** `P`

| P | Q | P |
|:---:|:---:|:---:|
| V | V | V |
| V | F | V |
| F | V | F |
| F | F | F |

### Questão 1.4

**Expressão analisada:** `P e Q (valores individuais)`

| P | Q |
|:---:|:---:|
| V | V |
| V | F |
| F | V |
| F | F |

### Questão 1.5

**Expressão analisada:** `¬P e ¬Q`

| P | Q | ¬P | ¬Q |
|:---:|:---:|:---:|:---:|
| V | V | F | F |
| V | F | F | V |
| F | V | V | F |
| F | F | V | V |

### Questão 1.6

**Expressão analisada:** `P, Q e R (valores individuais)`

| P | Q | R |
|:---:|:---:|:---:|
| V | V | V |
| V | V | F |
| V | F | V |
| V | F | F |
| F | V | V |
| F | V | F |
| F | F | V |
| F | F | F |

### Questão 1.7

**Expressão analisada:** `P, Q e R (valores individuais)`

| P | Q | R |
|:---:|:---:|:---:|
| V | V | V |
| V | V | F |
| V | F | V |
| V | F | F |
| F | V | V |
| F | V | F |
| F | F | V |
| F | F | F |

### Questão 1.8

**Expressão analisada:** `P e Q (valores individuais)`

| P | Q |
|:---:|:---:|
| V | V |
| V | F |
| F | V |
| F | F |

### Questão 1.9

**Expressão analisada:** `P, Q e R (valores individuais)`

| P | Q | R |
|:---:|:---:|:---:|
| V | V | V |
| V | V | F |
| V | F | V |
| V | F | F |
| F | V | V |
| F | V | F |
| F | F | V |
| F | F | F |

### Questão 1.10

**Expressão analisada:** `P, Q e R (valores individuais)`

| P | Q | R |
|:---:|:---:|:---:|
| V | V | V |
| V | V | F |
| V | F | V |
| V | F | F |
| F | V | V |
| F | V | F |
| F | F | V |
| F | F | F |

## Nível 02

### Questão 2.1

**Expressão analisada:** `P ∧ Q`

| P | Q | P ∧ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | F |
| F | F | F |

### Questão 2.2

**Expressão analisada:** `P ∨ Q`

| P | Q | P ∨ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | V |
| F | V | V |
| F | F | F |

### Questão 2.3

**Expressão analisada:** `P → Q`

| P | Q | P → Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | V |
| F | F | V |

### Questão 2.4

**Expressão analisada:** `P ↔ Q`

| P | Q | P ↔ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | F |
| F | F | V |

### Questão 2.5

**Expressão analisada:** `P ∧ Q`

| P | Q | P ∧ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | F |
| F | F | F |

### Questão 2.6

**Expressão analisada:** `P ∨ Q`

| P | Q | P ∨ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | V |
| F | V | V |
| F | F | F |

### Questão 2.7

**Expressão analisada:** `P → Q`

| P | Q | P → Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | V |
| F | F | V |

### Questão 2.8

**Expressão analisada:** `P ↔ Q`

| P | Q | P ↔ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | F |
| F | F | V |

## Nível 03

### Questão 3.1

**Expressão analisada:** `(P ∧ Q) ∨ R`

| P | Q | R | (P ∧ Q) ∨ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | F |
| F | F | V | V |
| F | F | F | F |

### Questão 3.2

**Expressão analisada:** `(P ∨ Q) ∧ R`

| P | Q | R | (P ∨ Q) ∧ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | F |
| F | F | V | F |
| F | F | F | F |

### Questão 3.3

**Expressão analisada:** `(Q ∧ R) ∨ P`

| P | Q | R | (Q ∧ R) ∨ P |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | V |
| F | V | V | V |
| F | V | F | F |
| F | F | V | F |
| F | F | F | F |

### Questão 3.4

**Expressão analisada:** `(P ∨ Q) ∧ ¬R`

| P | Q | R | (P ∨ Q) ∧ ¬R |
|:---:|:---:|:---:|:---:|
| V | V | V | F |
| V | V | F | V |
| V | F | V | F |
| V | F | F | V |
| F | V | V | F |
| F | V | F | V |
| F | F | V | F |
| F | F | F | F |

### Questão 3.5

**Expressão analisada:** `(¬P ∧ Q) ∨ R`

| P | Q | R | (¬P ∧ Q) ∨ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | V |
| F | F | V | V |
| F | F | F | F |

### Questão 3.6

**Expressão analisada:** `(P ∧ ¬Q) ∨ R`

| P | Q | R | (P ∧ ¬Q) ∨ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | V |
| V | F | F | V |
| F | V | V | V |
| F | V | F | F |
| F | F | V | V |
| F | F | F | F |

### Questão 3.7

**Expressão analisada:** `P ∨ (Q ∧ ¬R)`

| P | Q | R | P ∨ (Q ∧ ¬R) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | V |
| F | V | V | F |
| F | V | F | V |
| F | F | V | F |
| F | F | F | F |

### Questão 3.8

**Expressão analisada:** `Q ∨ (¬R ∧ P)`

| P | Q | R | Q ∨ (¬R ∧ P) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | F |
| V | F | F | V |
| F | V | V | V |
| F | V | F | V |
| F | F | V | F |
| F | F | F | F |

## Nível 04

### Questão 4.1

**Expressão analisada:** `(P ∧ Q) ∨ R`

| P | Q | R | (P ∧ Q) ∨ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | F |
| F | F | V | V |
| F | F | F | F |

### Questão 4.2

**Expressão analisada:** `P ∨ (Q ∧ R)`

| P | Q | R | P ∨ (Q ∧ R) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | V |
| F | V | V | V |
| F | V | F | F |
| F | F | V | F |
| F | F | F | F |

### Questão 4.3

**Expressão analisada:** `(P ∨ Q) → R`

| P | Q | R | (P ∨ Q) → R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | F |
| F | F | V | V |
| F | F | F | V |

### Questão 4.4

**Expressão analisada:** `(P ∧ Q) ↔ R`

| P | Q | R | (P ∧ Q) ↔ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | F |
| V | F | F | V |
| F | V | V | F |
| F | V | F | V |
| F | F | V | F |
| F | F | F | V |

### Questão 4.5

**Expressão analisada:** `(P ∧ Q) ∨ R`

| P | Q | R | (P ∧ Q) ∨ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | F |
| F | F | V | V |
| F | F | F | F |

### Questão 4.6

**Expressão analisada:** `P ∨ (Q ∧ R)`

| P | Q | R | P ∨ (Q ∧ R) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | V |
| F | V | V | V |
| F | V | F | F |
| F | F | V | F |
| F | F | F | F |

### Questão 4.7

**Expressão analisada:** `(P → Q) ∨ R`

| P | Q | R | (P → Q) ∨ R |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | V |
| V | F | F | F |
| F | V | V | V |
| F | V | F | V |
| F | F | V | V |
| F | F | F | V |

### Questão 4.8

**Expressão analisada:** `(P ∧ Q) ↔ (Q ∨ R)`

| P | Q | R | (P ∧ Q) ↔ (Q ∨ R) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | F |
| V | F | F | V |
| F | V | V | F |
| F | V | F | F |
| F | F | V | F |
| F | F | F | V |

### Questão 4.9

**Expressão analisada:** `(P ∧ Q) ↔ (R ∨ Q)`

| P | Q | R | (P ∧ Q) ↔ (R ∨ Q) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | F |
| V | F | F | V |
| F | V | V | F |
| F | V | F | F |
| F | F | V | F |
| F | F | F | V |

### Questão 4.10

**Expressão analisada:** `P → (Q ↔ R)`

| P | Q | R | P → (Q ↔ R) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | F |
| V | F | F | V |
| F | V | V | V |
| F | V | F | V |
| F | F | V | V |
| F | F | F | V |

### Questão 4.11

**Expressão analisada:** `(P ↔ Q) → (R ↔ Q)`

| P | Q | R | (P ↔ Q) → (R ↔ Q) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | F |
| V | F | V | V |
| V | F | F | V |
| F | V | V | V |
| F | V | F | V |
| F | F | V | F |
| F | F | F | V |

### Questão 4.12

**Expressão analisada:** `(P ∧ Q) ↔ (Q ∨ R)`

| P | Q | R | (P ∧ Q) ↔ (Q ∨ R) |
|:---:|:---:|:---:|:---:|
| V | V | V | V |
| V | V | F | V |
| V | F | V | F |
| V | F | F | V |
| F | V | V | F |
| F | V | F | F |
| F | F | V | F |
| F | F | F | V |

## Observação sobre a questão 4.8

A coluna final da tabela da questão **4.8** contém resultados falsos em algumas combinações. Portanto, a fórmula `(P ∧ Q) ↔ (Q ∨ R)` **não é uma tautologia**.
