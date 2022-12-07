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
print(w3.eth.get_balance(w3.eth._get_accounts()[0]))

# Carica il json e prende l'ABI
with open("SecureVoting.json") as f:
    info_json = json.load(f)
abi = info_json["abi"]

# Indirizzo dello smart contract e inizializzazione dell'oggetto
secureVotingAddress = "0x336C1bedF196990E4F479b72b7576A69487c182e"
contract = w3.eth.contract(address=secureVotingAddress, abi=abi)

numberOfVotes = contract.functions.getNumOfVoters().call()


tx_hash = contract.functions.addCandidate('Gerardo', 'Lega').buildTransaction({'gasPrice': w3.eth.gas_price})
{
    'to': '0x6Bc272FCFcf89C14cebFC57B8f1543F5137F97dE',
    'data': '0x81752d63',
    'value': 0,
    'gas': 43242,
    'gasPrice': w3.eth.gas_price
}

print(contract.functions.getNumOfCandidates().call())

@app.route("/")
def starting():
    return render_template('index.html', variable=numberOfVotes)

@app.route('/voting/numberOfCandidates')
def getNumOfCandidates():
    numofcandidates = contract.functions.getNumOfVoters().call()
    return "Numero di candidati: " + str(numofcandidates)

@app.route('/voting/addCandidate/<name>/<party>')
def addCandidate(name, party):
    try:
        contract.functions.addCandidate(name, party).transact()
        return "Candidato " + name + " per il partito " + party + " aggiunto!"
    except:
        return "Errore nell'aggiunta del candidato!"

@app.route('/voting/getCandidate/<int:candidateID>')
def getCandidate(candidateID):
    candid, candname, candparty = contract.functions.getCandidate(candidateID).call()
    return "Il nome del candidato è: " + str(candname)




















