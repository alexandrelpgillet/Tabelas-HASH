import zipfile
import os
import requests
import json
import uuid
import time


def extrair_arquivos(caminho_zip, pasta_destino):
    with zipfile.ZipFile(caminho_zip, 'r') as zip_ref:
        zip_ref.extractall(pasta_destino)  # Extrai todos os arquivos para a pasta
    return [os.path.join(pasta_destino, f) for f in os.listdir(pasta_destino)]  # Retorna a lista de arquivos extraídos


def enviar_para_api(url_api, arquivos, empresa, palavra_chave):


    couchdb_url = "http://35.199.94.204:5984"  # URL do CouchDB
    db = "contratos-apolo"  # Nome do banco de dados
    db_automated = "contratos-apolo-automatizado"  # Nome do banco de dados
    username = "admin"  # Usuário do CouchDB
    password = "admin"  # Senha do CouchDB







    """ Envia os arquivos extraídos para a API com o parâmetro 'contrato_request'. """
    for arquivo in arquivos:
        nome_arquivo_sem_extensao = os.path.splitext(os.path.basename(arquivo))[0]  # Remove a extensão .kml
        with open(arquivo, 'rb') as f:
            files = {'file': (os.path.basename(arquivo), f)}
            data = {'contrato_request': f"{empresa}:{nome_arquivo_sem_extensao}"}  # ✅ Nome sem extensão

            unique_id_city = uuid.uuid4().hex[:32]  # Mantém 32 caracteres hexadecimais
            unique_id_empresa = uuid.uuid4().hex[:32]  # Mantém 32 caracteres hexadecimais



            resposta = requests.post(url_api, files=files, data=data)

            if resposta.status_code == 201:
                print(f"✅ Enviado com sucesso: {arquivo}")
            else:
                print(f"❌ Erro ao enviar {arquivo}: {resposta.status_code}, {resposta.text}")



            particionEmpresa = {
                "_id": f"{empresa}:{nome_arquivo_sem_extensao}:{unique_id_empresa}",
                "cidade": f"{nome_arquivo_sem_extensao}",
                "empresa": f"{empresa}",
                "key": f"{palavra_chave}"

            }

            url = f"{couchdb_url}/{db}"

            time.sleep(0.5)  # Espera 500ms entre as requisições

            particion_main = requests.post(url, data=json.dumps(particionEmpresa), headers={"Content-Type": "application/json"}, auth=(username, password))

            if(particion_main.status_code ==201):

                print(f"✅ Endereço salvo da no CouchDB contrato-apolo com sucesso = {empresa}:{nome_arquivo_sem_extensao}");

            else:
                print(f"❌ Erro ao salvar no CouchDB contrato-apolo  = {empresa}:{nome_arquivo_sem_extensao}");
                print("Código de status:", particion_main.status_code)
                print("Resposta do servidor:", particion_main.text)


            particionCity = {
                    "_id": f"{nome_arquivo_sem_extensao}:{empresa}:{unique_id_city}",
                    "cidade": f"{nome_arquivo_sem_extensao}",
                    "empresa": f"{empresa}",
                    "key": f"{palavra_chave}"
            }


            url = f"{couchdb_url}/{db_automated}"

            time.sleep(0.5)  # Espera 500ms entre as requisições

            particion_automated = requests.post(url, data=json.dumps(particionCity), headers={"Content-Type": "application/json"}, auth=(username, password))



            if(particion_automated.status_code ==200):

                print(f"✅ Endereço salvo da no CouchDB contrato-apolo-automatizado com sucesso = {nome_arquivo_sem_extensao}:{empresa}");

            else:
                print(f"❌ Erro ao salvar no CouchDB contrato-apolo-automatizado  = {nome_arquivo_sem_extensao}:{empresa}");







caminho_zip = "kml.zip"
pasta_destino = "KML's"
url_api = "https://gcp-kml-import-1014371172316.southamerica-east1.run.app/upload_kml"  # Substituir pelo seu endpoint
empresa = "Saneago";
palavra_chave = "SANEAMENTO BÁSICO";

# 1. Extrair os arquivos
arquivos_extraidos = extrair_arquivos(caminho_zip, pasta_destino)


# 2. Enviar os arquivos extraídos para a API
enviar_para_api(url_api, arquivos_extraidos, empresa, palavra_chave);
