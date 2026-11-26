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

Se aparecer `Unable to locate package libraylib-dev` (acontece no Ubuntu 26.04), compile a raylib pelo código-fonte:

```bash
sudo apt install -y build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev \
  libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
git clone --depth 1 https://github.com/raysan5/raylib.git /tmp/raylib
cd /tmp/raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

No Windows, use o WSL (`wsl --install` no PowerShell) e siga os passos do Ubuntu. A janela gráfica abre pelo WSLg.

## Rodando o projeto

```bash
git clone https://github.com/guialveess/logi-decifra-ai.git
cd logi-decifra-ai

make run      # compila e roda no terminal
make run-gui  # interface gráfica (requer raylib)
make test     # testes automatizados
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