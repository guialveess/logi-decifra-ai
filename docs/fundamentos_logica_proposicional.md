# Fundamentos da Lógica Proposicional Clássica

## 1. Introdução

A **lógica proposicional clássica** é um sistema formal utilizado para representar e analisar proposições e as relações estabelecidas entre elas. Nesse sistema, cada proposição possui exatamente um de dois valores lógicos possíveis:

- **V** — Verdadeiro;
- **F** — Falso.

O material de exercícios fornecido trabalha progressivamente com proposições individuais e, em seguida, com expressões formadas por conectivos como **AND**, **OR**, **NOT**, **IMPLICA** e **BICONDICIONAL**. Este documento sistematiza esses conceitos utilizando a notação tradicional da lógica proposicional.

---

## 2. Proposição lógica

Uma **proposição lógica** é uma sentença declarativa à qual pode ser atribuído um valor lógico bem definido: **Verdadeiro (V)** ou **Falso (F)**.

Exemplos:

- `P: 10 > 5.` — proposição verdadeira.
- `Q: 2 + 2 = 5.` — proposição falsa.
- `R: Um quadrado possui quatro lados.` — proposição verdadeira.

Sentenças interrogativas, ordens e expressões cujo valor lógico não possa ser determinado não são, em geral, tratadas como proposições na lógica proposicional clássica.

### 2.1. Proposição simples

Uma **proposição simples**, também chamada de **proposição atômica**, é uma proposição considerada como uma unidade lógica e que **não é formada pela combinação de outras proposições por meio de conectivos lógicos**.

Normalmente, proposições simples são representadas por letras maiúsculas, como `P`, `Q` e `R`.

Exemplos:

- `P: O Brasil está localizado na América do Sul.`
- `Q: 3 é igual a 8.`
- `R: Rio Branco é a capital do Acre.`

É importante observar que uma frase pode conter palavras como “e” em seu português cotidiano sem necessariamente ser decomposta, no exercício em questão, em duas proposições formais. O que determina a estrutura lógica é a maneira como a sentença é formalizada.

### 2.2. Proposição composta

Uma **proposição composta**, também chamada de **proposição molecular**, é formada a partir de uma ou mais proposições mediante o uso de **conectivos lógicos**.

Exemplos:

- `P ∧ Q` — “P e Q”;
- `P ∨ Q` — “P ou Q”;
- `¬P` — “não P”;
- `P → Q` — “se P, então Q”;
- `P ↔ Q` — “P se, e somente se, Q”.

O valor lógico de uma proposição composta depende dos valores lógicos de suas proposições componentes e das regras do conectivo empregado.

---

## 3. Operadores da lógica proposicional clássica

Os cinco operadores abordados neste documento são:

| Operador | Símbolo | Leitura usual |
|---|:---:|---|
| Negação | `¬P` | não P |
| Conjunção | `P ∧ Q` | P e Q |
| Disjunção | `P ∨ Q` | P ou Q |
| Condicional | `P → Q` | se P, então Q |
| Bicondicional | `P ↔ Q` | P se, e somente se, Q |

> **Observação:** em materiais introdutórios e em linguagens computacionais podem aparecer outras representações, como `~P` ou `NOT P` para negação, `AND` para conjunção e `OR` para disjunção. Neste documento será priorizada a notação matemática `¬`, `∧`, `∨`, `→` e `↔`.

---

## 4. Negação lógica

A **negação** é um operador **unário**, pois atua sobre uma única proposição.

Dada uma proposição `P`, sua negação é representada por:

`¬P`

e pode ser lida como **“não P”** ou **“é falso que P”**.

A negação **inverte o valor lógico** da proposição original:

- se `P` é verdadeira, `¬P` é falsa;
- se `P` é falsa, `¬P` é verdadeira.

### Tabela-verdade da negação

| P | ¬P |
|:---:|:---:|
| V | F |
| F | V |

### Exemplo

Considere:

`P: Um quadrado possui quatro lados.`

Como `P` é verdadeira, sua negação:

`¬P: Um quadrado não possui quatro lados.`

é falsa.

---

## 5. Conjunção lógica

A **conjunção** combina duas proposições e é representada pelo símbolo:

`P ∧ Q`

Sua leitura usual é **“P e Q”**.

A conjunção é verdadeira **somente quando as duas proposições são verdadeiras simultaneamente**. Se pelo menos uma delas for falsa, a conjunção será falsa.

### Tabela-verdade da conjunção

| P | Q | P ∧ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | F |
| F | F | F |

### Exemplo

Considere:

- `P: 10 > 5.` — V
- `Q: 8 < 12.` — V

Logo:

`P ∧ Q`

é **Verdadeiro**, pois ambas as proposições são verdadeiras.

---

## 6. Disjunção lógica

A **disjunção** combina duas proposições e é representada pelo símbolo:

`P ∨ Q`

Sua leitura usual é **“P ou Q”**.

Na lógica proposicional clássica, o símbolo `∨` representa a **disjunção inclusiva**. Isso significa que a expressão é verdadeira quando **pelo menos uma** das proposições é verdadeira, inclusive quando ambas são verdadeiras.

A disjunção será falsa somente quando `P` e `Q` forem simultaneamente falsas.

### Tabela-verdade da disjunção

| P | Q | P ∨ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | V |
| F | V | V |
| F | F | F |

### Exemplo

Considere:

- `P: 10 > 5.` — V
- `Q: 3 = 8.` — F

Então:

`P ∨ Q`

é **Verdadeiro**, pois pelo menos uma das proposições é verdadeira.

---

## 7. Condicional lógica

A **condicional** é representada por:

`P → Q`

e pode ser lida como:

**“Se P, então Q.”**

Nessa estrutura:

- `P` é denominado **antecedente**;
- `Q` é denominado **consequente**.

Na lógica proposicional clássica, a condicional é falsa **somente quando o antecedente é verdadeiro e o consequente é falso**. Em todas as demais combinações, a condicional é verdadeira.

### Tabela-verdade da condicional

| P | Q | P → Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | V |
| F | F | V |

### Exemplo

Considere:

- `P = V`
- `Q = F`

Logo:

`P → Q = F`

Esse é o único caso em que uma condicional clássica possui valor lógico falso.

> **Atenção:** a condicional lógica é definida por sua tabela-verdade. Portanto, ela não exige, por si só, uma relação de causa e efeito entre `P` e `Q`.

---

## 8. Bicondicional lógica

A **bicondicional** é representada por:

`P ↔ Q`

e pode ser lida como:

**“P se, e somente se, Q.”**

A bicondicional é verdadeira quando `P` e `Q` possuem **o mesmo valor lógico**. Consequentemente:

- se ambas forem verdadeiras, a bicondicional será verdadeira;
- se ambas forem falsas, a bicondicional também será verdadeira;
- se apresentarem valores lógicos diferentes, a bicondicional será falsa.

### Tabela-verdade da bicondicional

| P | Q | P ↔ Q |
|:---:|:---:|:---:|
| V | V | V |
| V | F | F |
| F | V | F |
| F | F | V |

### Exemplo

Considere:

- `P = F`
- `Q = F`

Então:

`P ↔ Q = V`

pois as duas proposições possuem o mesmo valor lógico.

---

## 9. Quadro-resumo dos operadores

| Operador | Forma | Resultado verdadeiro quando... |
|---|---|---|
| Negação | `¬P` | `P` é falsa. |
| Conjunção | `P ∧ Q` | `P` e `Q` são verdadeiras. |
| Disjunção | `P ∨ Q` | pelo menos uma entre `P` e `Q` é verdadeira. |
| Condicional | `P → Q` | ocorre qualquer caso, exceto `P = V` e `Q = F`. |
| Bicondicional | `P ↔ Q` | `P` e `Q` possuem o mesmo valor lógico. |

---

## 10. Considerações finais

A distinção entre **proposições simples** e **proposições compostas** constitui uma base essencial para o estudo da lógica proposicional. Proposições simples funcionam como unidades fundamentais de análise, enquanto proposições compostas são construídas por meio de conectivos lógicos.

O domínio das tabelas-verdade permite determinar de maneira sistemática o valor lógico de expressões cada vez mais complexas. Assim, compreender as regras da negação, conjunção, disjunção, condicional e bicondicional prepara o estudante para tópicos posteriores, como equivalências lógicas, tautologias, contradições e simplificação de fórmulas proposicionais.
