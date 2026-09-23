#!/usr/bin/env python3
import os
import sys
import json
import threading
import torch

# Desabilita protocolo XET (buggy em alguns ambientes) — usa HTTP padrao
os.environ.setdefault('HF_HUB_DISABLE_XET', '1')

from http.server import BaseHTTPRequestHandler, HTTPServer
from transformers import AutoModelForCausalLM, AutoTokenizer
from peft import PeftModel

MODEL_BASE = 'Qwen/Qwen2.5-3B-Instruct'
LORA_REPO  = 'guiiwfz/logi'
PORT       = 8787

SYSTEM = (
    'Voce e um tutor especializado em logica proposicional. '
    'Responda passo a passo, usando os operadores: NOT, AND, OR, IMPLICA, BICONDICIONAL. '
    'Seja claro e didatico. Responda SEMPRE em portugues brasileiro.'
)

print('Carregando modelo...', flush=True)
device = 'mps' if torch.backends.mps.is_available() else 'cpu'
tokenizer = AutoTokenizer.from_pretrained(MODEL_BASE)
# device_map nao e suportado no MPS — carrega na CPU e move para o device
base = AutoModelForCausalLM.from_pretrained(
    MODEL_BASE, dtype=torch.float16, low_cpu_mem_usage=True)
base = base.to(device)
modelo = PeftModel.from_pretrained(base, LORA_REPO)
modelo = modelo.to(device)
modelo.eval()
print(f'Modelo carregado em {device}. Servidor pronto na porta {PORT}.', flush=True)

lock = threading.Lock()

def gerar(pergunta):
    mensagens = [
        {'role': 'system', 'content': SYSTEM},
        {'role': 'user',   'content': pergunta},
    ]
    text = tokenizer.apply_chat_template(
        mensagens, tokenize=False, add_generation_prompt=True)
    inputs = tokenizer(text, return_tensors='pt').to(device)
    with lock:
        with torch.no_grad():
            outputs = modelo.generate(
                **inputs, max_new_tokens=500, temperature=0.3, do_sample=True)
    input_len = inputs['input_ids'].shape[-1]
    return tokenizer.decode(outputs[0][input_len:], skip_special_tokens=True)

class Handler(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        pass

    def do_POST(self):
        if self.path != '/ask':
            self.send_response(404)
            self.end_headers()
            return
        length = int(self.headers.get('Content-Length', 0))
        body   = json.loads(self.rfile.read(length))
        pergunta = body.get('pergunta', '')
        try:
            resposta = gerar(pergunta)
        except Exception as e:
            resposta = f'Erro na geracao: {e}'
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({'resposta': resposta}).encode())

if __name__ == '__main__':
    server = HTTPServer(('127.0.0.1', PORT), Handler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print('\nServidor encerrado.')
