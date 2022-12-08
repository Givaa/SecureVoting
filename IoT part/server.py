# bisogna aggiungere le eccezioni, la crittografia e dei controlli esterni e aggiungere dei ritorni di errori http
from web3 import Web3
import json
from flask import Flask, render_template

app = Flask(__name__, template_folder='html')

# Inizializzo la connessione a Ganache
provider = Web3.HTTPProvider('http://127.0.0.1:7545')
w3 = Web3(provider)

# Controlli principali per Ganache
print(w3.isConnected())
print(w3.clientVersion)

firstAccount = w3.eth._get_accounts()[0]
print(w3.eth.get_balance(firstAccount))

secondAccount = w3.eth._get_accounts()[1]
print(w3.eth.get_balance(secondAccount))
print(len(w3.eth._get_accounts()))

# Carica il json e prende l'ABI
with open("SecureVoting.json") as f:
    info_json = json.load(f)
abi = info_json["abi"]

# Indirizzo dello smart contract e inizializzazione dell'oggetto
secureVotingAddress = "0x511c95B2cf0d2b24b7eD948dAC1f9b27A2b1cE1d"
contract = w3.eth.contract(address=secureVotingAddress, abi=abi)

# contract.functions.addCandidate('Gerardo', 'Lega').transact({"from": firstAccount})

print(contract.functions.getNumOfCandidates().call())

@app.route('/isConnected')
def isConnected():
    return str(w3.isConnected())

@app.route('/voting/numberOfCandidates')
def getNumOfCandidates():
    numofcandidates = contract.functions.getNumOfCandidates().call()
    return "Numero di candidati: " + str(numofcandidates)

@app.route('/voting/addCandidate/<name>/<party>')
def addCandidate(name, party):
    try:
        contract.functions.addCandidate(name, party).transact({"from": firstAccount})
        return "Candidato " + name + " per il partito " + party + " aggiunto!"
    except:
        return "Errore nell'aggiunta del candidato!"

@app.route('/voting/getCandidate/<int:candidateID>')
def getCandidate(candidateID):
    candid, candname, candparty = contract.functions.getCandidate(candidateID).call()
    return "Il nome del candidato è: " + str(candname)

@app.route('/vote/<int:uid>/<int:candidateID>')
def vote(uid, candidateID):
    # da modificare quando arriva l'RFID
    try:
        contract.functions.vote(str(uid), candidateID).transact({"from": firstAccount})
        return str(contract.functions.totalVotes(candidateID).call())
    except:
        return "Votazione fallita"

if __name__ == '__main__':
    app.run(host = '0.0.0.0', port=222)




















