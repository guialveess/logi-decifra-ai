#!/usr/bin/env python3
"""
Deploy do modelo LOGI no Modal (SPIKE PI2-125).

Serve o modelo GGUF (guiiwfz/logi-1.5b-gguf) como um endpoint HTTP no Modal.
A imagem baixa o modelo do HuggingFace durante o build para evitar cold-start
de download a cada requisicao.

Deploy:
    modal deploy scripts/modal_server.py

Testar localmente (sem deploy):
    modal run scripts/modal_server.py

O endpoint exposto aceita POST /ask com { "pergunta": "..." }
e retorna { "resposta": "...", "latencia_ms": ... }
"""

import time
import json

import modal

MODEL_REPO = "guiiwfz/logi-1.5b-gguf"
MODEL_FILE = "*Q4_K_M.gguf"
APP_NAME   = "logi-decifra-ai"

SYSTEM_PROMPT = (
    "Voce e LOGI, tutor do jogo educativo Decifra.IA. "
    "Seu escopo inclui: "
    "logica proposicional: AND, OR, NOT, IMPLICA, BICONDICIONAL, tabelas-verdade, "
    "equivalencias, falacias e regras de inferencia; "
    "alfabetizacao em IA: modelos, dados, treino, avaliacao, vies, alucinacao, RAG, "
    "privacidade, etica e uso responsavel; "
    "seguranca digital relacionada ao uso de IA: phishing, deepfakes, engenharia social "
    "e protecao de dados. "
    "Responda em portugues, com linguagem simples e correta. "
    "Em calculos de logica, mostre os passos. "
    "Nas outras perguntas, responda de forma direta e didatica. "
    "Nao invente fontes, leis, numeros ou capacidades. "
    "Quando o tema estiver fora do escopo, diga isso brevemente e redirecione "
    "para logica, IA ou seguranca no uso de IA."
)

image = (
    modal.Image.debian_slim(python_version="3.11")
    .pip_install("llama-cpp-python", "huggingface_hub", "fastapi[standard]")
    .run_commands(
        "python -c \""
        "from huggingface_hub import hf_hub_download; "
        "hf_hub_download(repo_id='guiiwfz/logi-1.5b-gguf', "
        "filename='qwen2.5-1.5b-instruct.Q4_K_M.gguf', "
        "local_dir='/model')"
        "\""
    )
)

app = modal.App(APP_NAME, image=image)


@app.cls(
    cpu=4,
    memory=4096,
    timeout=120,
)
@modal.concurrent(max_inputs=4)
class LoGI:
    @modal.enter()
    def carregar(self):
        from llama_cpp import Llama
        import glob
        arquivos = glob.glob("/model/*.gguf")
        if not arquivos:
            raise RuntimeError("Modelo GGUF nao encontrado em /model/")
        self.llm = Llama(
            model_path=arquivos[0],
            n_ctx=1024,
            n_gpu_layers=0,
            verbose=False,
        )

    @modal.method()
    def gerar(self, pergunta: str) -> dict:
        t0 = time.time()
        resultado = self.llm.create_chat_completion(
            messages=[
                {"role": "system", "content": SYSTEM_PROMPT},
                {"role": "user",   "content": pergunta},
            ],
            temperature=0.2,
            max_tokens=300,
        )
        latencia_ms = int((time.time() - t0) * 1000)
        resposta = resultado["choices"][0]["message"]["content"]
        return {"resposta": resposta, "latencia_ms": latencia_ms}

    @modal.fastapi_endpoint(method="POST")
    def ask(self, body: dict) -> dict:
        pergunta = body.get("pergunta", "")
        if not pergunta:
            return {"erro": "campo 'pergunta' obrigatorio"}
        return self.gerar(pergunta)


# Entrypoint para testes locais com `modal run scripts/modal_server.py`
@app.local_entrypoint()
def testar():
    perguntas = [
        "Qual e o resultado de V AND F?",
        "O que e uma tautologia?",
        "Explique o conectivo IMPLICA com um exemplo.",
    ]
    modelo = LoGI()
    print("\n=== Teste LOGI no Modal ===\n")
    for p in perguntas:
        print(f"Pergunta: {p}")
        resultado = modelo.gerar.remote(p)
        print(f"Resposta: {resultado['resposta'][:120]}...")
        print(f"Latencia: {resultado['latencia_ms']} ms\n")
