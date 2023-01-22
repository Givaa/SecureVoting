from web3 import Web3
import json
from flask import Flask, request, jsonify
import sys
import mysql.connector as sqlconnect
from flask_sslify import SSLify

DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!",
    database="Seggio"
)

Cursor = DataBase.cursor()

SecureToken = "R3,u=t?_~LRrPycS"

app = Flask(__name__)
sslify = SSLify(app)

# Inizializzo la connessione a Ganache
provider = Web3.HTTPProvider('http://127.0.0.1:7545')
w3 = Web3(provider)

# Carica il json e prende l'ABI
with open("SecureVoting.json") as f:
    info_json = json.load(f)
abi = info_json["abi"]

# Indirizzo dello smart contract e inizializzazione dell'oggetto
secureVotingAddress = "0x511c95B2cf0d2b24b7eD948dAC1f9b27A2b1cE1d"
contract = w3.eth.contract(address=secureVotingAddress, abi=abi)

print(contract.functions.getNumOfCandidates().call())

@app.before_request
def verify_token():
    auth_header = request.headers.get("Authorization")
    if auth_header:
        if auth_header != SecureToken:
            return jsonify({"error": "You're not authorized"}), 401
    else:
        return jsonify({"error": "Missing Authorization header"}), 401


@app.route('/isConnectedGanache', methods=['GET'])
def isConnected():
    return str(w3.isConnected())

@app.route('/vote', methods=['POST'])
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
    print(element, sys.stderr)
    if result == 1:
        return "Already Voted!"
    else:
        contract.functions.vote(str(uid), int(candidateid)).transact({"from": actualAccount})
        Cursor.execute("UPDATE Votante SET alreadyVoted = 1 WHERE RFID = (%s)", (RFID,))
        DataBase.commit()
        return str(contract.functions.totalVotes(int(candidateid)).call())


@app.route('/authenticate', methods=['POST'])
def authenticate():
    payload = request.get_data().decode('utf-8')
    RFID = payload.split('=')[1]
    Cursor.execute("SELECT BlockchainID FROM Votante WHERE RFID = (%s)", (RFID,))
    for element in Cursor.fetchone():
        actualBlockchainID = element
    return str(actualBlockchainID)


if __name__ == '__main__':
    app.run(ssl_context=('cert/certificate.pem', 'cert/privatekey.pem'), host='0.0.0.0', port=443)
