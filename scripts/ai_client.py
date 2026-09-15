#!/usr/bin/env python3
import sys
import json
import urllib.request
import urllib.error

LOCAL  = 'http://127.0.0.1:8787/ask'
REMOTE = 'https://guiiwfz-decifra-ia.hf.space'

def consultar_local(pergunta):
    payload = json.dumps({'pergunta': pergunta}).encode('utf-8')
    req = urllib.request.Request(
        LOCAL, data=payload,
        headers={'Content-Type': 'application/json'})
    try:
        with urllib.request.urlopen(req, timeout=3) as resp:
            return json.loads(resp.read()).get('resposta', '')
    except Exception:
        return None

def consultar_remoto(pergunta):
    payload = json.dumps({'pergunta': pergunta}).encode('utf-8')
    req = urllib.request.Request(
        REMOTE + '/gradio_api/call/v2/responder',
        data=payload,
        headers={'Content-Type': 'application/json'}
    )
    try:
        with urllib.request.urlopen(req, timeout=5) as resp:
            event_id = json.loads(resp.read()).get('event_id')
            if not event_id:
                return ''
    except Exception:
        return ''

    try:
        url = REMOTE + f'/gradio_api/call/responder/{event_id}'
        event_type = ''
        with urllib.request.urlopen(url, timeout=15) as stream:
            for raw in stream:
                line = raw.decode('utf-8').strip()
                if line.startswith('event:'):
                    event_type = line[6:].strip()
                    continue
                if not line.startswith('data:'):
                    continue
                try:
                    obj = json.loads(line[5:].strip())
                except json.JSONDecodeError:
                    continue
                if event_type == 'error':
                    return ''
                if isinstance(obj, dict) and 'output' in obj:
                    data = obj['output'].get('data', [])
                    return data[0] if data else ''
                if isinstance(obj, list) and obj:
                    return obj[0]
    except Exception:
        return ''
    return ''

def consultar(pergunta):
    local = consultar_local(pergunta)
    if local:
        return local
    return consultar_remoto(pergunta)

if __name__ == '__main__':
    pergunta = ' '.join(sys.argv[1:])
    if not pergunta:
        sys.exit(1)
    resultado = consultar(pergunta)
    if resultado:
        print(resultado, end='', flush=True)
