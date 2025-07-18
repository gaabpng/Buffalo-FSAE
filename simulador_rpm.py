import time
import requests
import random

url = 'http://127.0.0.1:8000/api/dados/'  # Endpoint do Django

while True:
    rpm = random.uniform(800, 9000)  # Simula valores entre 800 e 9000 RPM
    rpm = int(rpm)
    print(f"Enviando RPM: {rpm}")

    response = requests.post(url, json={'RPM': rpm})  # <-- Maiúsculo aqui
    if response.status_code == 201:
        print("→ Enviado com sucesso")
    else:
        print(f"Erro: {response.status_code} - {response.text}")

    time.sleep(0.1)  # 50ms
