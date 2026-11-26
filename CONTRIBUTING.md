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

# Windows: instale o WSL (no PowerShell) e siga os passos do Ubuntu
wsl --install
```

Se aparecer `Unable to locate package libraylib-dev` (acontece no Ubuntu 26.04) ou se você estiver no WSL, siga a seção abaixo.

## Instalação no Linux / WSL (Ubuntu)

```bash
# No PowerShell: entrar no WSL com o seu usuário (não use `su`)
wsl -u <usuario>

# Dependências
sudo apt update
sudo apt install -y git wget build-essential ffmpeg libasound2-dev libx11-dev libxrandr-dev \
  libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
  libwayland-dev libxkbcommon-dev libpulse0 libpulse-dev pulseaudio-utils libasound2-plugins

# Áudio no WSL (o som sai pelo Windows via WSLg)
echo 'export PULSE_SERVER=unix:/mnt/wslg/PulseServer' >> ~/.bashrc && source ~/.bashrc
printf 'pcm.!default { type pulse }\nctl.!default { type pulse }\n' > ~/.asoundrc

# Raylib pelo código-fonte, como biblioteca compartilhada
git clone --depth 1 https://github.com/raysan5/raylib.git /tmp/raylib
cd /tmp/raylib/src
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED
sudo make install RAYLIB_LIBTYPE=SHARED && sudo ldconfig
```

No WSL, a janela gráfica abre pelo WSLg (já incluso no Windows 11; se necessário, rode `wsl --update` no PowerShell).

## Rodando o projeto

```bash
# Clonar o repositório
cd ~
git clone https://github.com/guialveess/logi-decifra-ai.git
cd logi-decifra-ai

make run      # compila e roda no terminal
make run-gui  # interface gráfica (requer raylib)
make test     # testes automatizados
make clean    # remove binários e objetos
```

### Usando o binário da release (sem compilar)

O áudio toca os arquivos de `assets/audio/` com o `ffplay`, então baixe o binário **dentro da pasta do projeto** para ter som:

```bash
cd ~/logi-decifra-ai
wget https://github.com/guialveess/logi-decifra-ai/releases/download/v2.0/jogo-linux
chmod +x jogo-linux && ./jogo-linux
```

## Problemas comuns

- **Janela gráfica não abre:** entre com `wsl -u <usuario>` (não use `su` nem root).
- **`undefined reference to 'roundf'`:** instale a raylib com `RAYLIB_LIBTYPE=SHARED`.
- **Sem som:** instale o `ffmpeg` e rode o jogo dentro da pasta do projeto.

## Estrutura do projeto

```
src/          # código-fonte C
include/      # headers
assets/audio/ # trilhas sonoras (.mp3)
scripts/      # integração Python com o modelo LOGI (Qwen2.5)
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
