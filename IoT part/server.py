# bisogna aggiungere le eccezioni, la crittografia e dei controlli esterni e aggiungere dei ritorni di errori http
from web3 import Web3
import json
from flask import Flask, request
import sys
import mysql.connector as sqlconnect

DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!",
    database="Seggio"
)

Cursor = DataBase.cursor()

app = Flask(__name__)

# Inizializzo la connessione a Ganache
provider = Web3.HTTPProvider('http://127.0.0.1:7545')
w3 = Web3(provider)

# Controlli principali per Ganache
print(w3.isConnected())
print(w3.clientVersion)

# da cancellare
firstAccount = w3.eth._get_accounts()[0]
secondAccount = w3.eth._get_accounts()[1]

# Carica il json e prende l'ABI
with open("SecureVoting.json") as f:
    info_json = json.load(f)
abi = info_json["abi"]

# Indirizzo dello smart contract e inizializzazione dell'oggetto
# da fare meglio per prendere indirizzo smart contract dinamico
secureVotingAddress = "0x511c95B2cf0d2b24b7eD948dAC1f9b27A2b1cE1d"
contract = w3.eth.contract(address=secureVotingAddress, abi=abi)

print(contract.functions.getNumOfCandidates().call())

@app.route('/isConnected')
def isConnected():
    return str(w3.isConnected())

@app.route('/voting/numberOfCandidates', methods = ['GET'])
def getNumOfCandidates():
    numofcandidates = contract.functions.getNumOfCandidates().call()
    return "Numero di candidati: " + str(numofcandidates)

@app.route('/voting/addCandidate/<name>/<party>', methods = ['POST'])
def addCandidate(name, party):
    try:
        contract.functions.addCandidate(name, party).transact({"from": firstAccount})
        return "Candidato " + name + " per il partito " + party + " aggiunto!"
    except:
        return "Errore nell'aggiunta del candidato!"

@app.route('/voting/getCandidate/<int:candidateID>', methods = ['POST'])
def getCandidate(candidateID):
    candid, candname, candparty = contract.functions.getCandidate(candidateID).call()
    return "Il nome del candidato è: " + str(candname)

@app.route('/vote', methods = ['POST'])
def vote():
    payload = request.get_data().decode('utf-8').split('&')
    uid, RFID, candidateid = payload
    uid = uid.split('=')[1]
    candidateid = candidateid.split('=')[1]
    RFID = RFID.split('=')[1]
    for account in w3.eth._get_accounts():
        if account == uid:
            actualAccount = account
    Cursor.execute("SELECT alreadyVoted FROM Votante WHERE RFID = (%s)", (RFID,))
    for element in Cursor.fetchone():
        result = element
    if result == '0x01':
        return "Already Voted!"
    else:
        contract.functions.vote(str(uid), int(candidateid)).transact({"from": actualAccount})
        return str(contract.functions.totalVotes(int(candidateid)).call())
@app.route('/authenticate', methods = ['POST'])
def authenticate():
    payload = request.get_data().decode('utf-8')
    RFID = payload.split('=')[1]
    Cursor.execute("SELECT BlockchainID FROM Votante WHERE RFID = (%s)", (RFID,))
    for element in Cursor.fetchone():
        actualBlockchainID = element
    return str(actualBlockchainID)

if __name__ == '__main__':
    app.run(ssl_context= ('cert/certificate.pem', 'cert/privatekey.pem'), host= '0.0.0.0', port=222)




















