import requests

url = 'https://192.168.1.153:443/vote'

data = {
    'uid': '0x4c72fD9A313E8C50bcE0E513b55D35f1568481da',
    'RFID': '8C35D337',
    'candidateID': '0'
}

response = requests.post(url, data=data, verify=False)

print("Status Code", response.status_code)





