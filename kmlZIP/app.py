import zipfile
import os
import requests

def extrair_arquivos(caminho_zip, pasta_destino):
    with zipfile.ZipFile(caminho_zip, 'r') as zip_ref:
        zip_ref.extractall(pasta_destino)  # Extrai todos os arquivos para a pasta
    return [os.path.join(pasta_destino, f) for f in os.listdir(pasta_destino)]  # Retorna a lista de arquivos extraídos


def enviar_para_api(url_api, arquivos, empresa):
    """ Envia os arquivos extraídos para a API com o parâmetro 'contrato_request'. """
    for arquivo in arquivos:
        nome_arquivo_sem_extensao = os.path.splitext(os.path.basename(arquivo))[0]  # Remove a extensão .kml
        with open(arquivo, 'rb') as f:
            files = {'file': (os.path.basename(arquivo), f)}
            data = {'contrato_request': f"{empresa}:{nome_arquivo_sem_extensao}"}  # ✅ Nome sem extensão

            resposta = requests.post(url_api, files=files, data=data)

            if resposta.status_code == 200:
                print(f"✅ Enviado com sucesso: {arquivo}")
            else:
                print(f"❌ Erro ao enviar {arquivo}: {resposta.status_code}, {resposta.text}")




caminho_zip = "kml.zip"
pasta_destino = "KML's"
url_api = "https://gcp-kml-import-1014371172316.southamerica-east1.run.app/upload_kml"  # Substituir pelo seu endpoint
empresa = "Saneago";


# 1. Extrair os arquivos
arquivos_extraidos = extrair_arquivos(caminho_zip, pasta_destino)


# 2. Enviar os arquivos extraídos para a API
enviar_para_api(url_api, arquivos_extraidos, empresa)
