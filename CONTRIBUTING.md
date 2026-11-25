# Contribuindo com o LOGI — Decifra.IA

## Pré-requisitos

- `gcc` e `make`
- `ffplay` (ffmpeg) para áudio — opcional
- `raylib` para interface gráfica — opcional

```bash
# macOS
brew install ffmpeg raylib

# Ubuntu/Debian
sudo apt install gcc make ffmpeg libraylib-dev
```

## Rodando o projeto

```bash
git clone https://github.com/guialveess/logi-decifra-ai.git
cd logi-decifra-ai

make          # terminal
make run-gui  # interface gráfica (requer raylib)
```

## Estrutura do projeto

```
src/          # código-fonte C
include/      # headers
assets/audio/ # trilhas sonoras (.mp3)
tests/        # testes automatizados
docs/         # documentação e roteiros
.github/      # workflows CI/CD
```

## Fluxo de contribuição

1. Crie uma branch a partir de `main`:
   ```bash
   git checkout -b feat/nome-da-feature
   ```
2. Faça commits semânticos: `feat:`, `fix:`, `docs:`, `refactor:`
3. Rode os testes antes de abrir PR:
   ```bash
   make test
   ```
4. Abra um Pull Request para `main` descrevendo o que foi feito

## Análise estática

O CI roda `cppcheck` automaticamente em todo PR. Para rodar localmente:

```bash
cppcheck --error-exitcode=1 --enable=warning,portability --suppress=missingIncludeSystem src/
```

## Time

Projeto desenvolvido pelo Squad 05 — CESAR School, Projeto Integrador 2P E5.
