from web3 import Web3
import json
from flask import Flask, request, jsonify
import mysql.connector as sqlconnect
from flask_sslify import SSLify

# Inizialize Database connection
DataBase = sqlconnect.connect(
    host="localhost",
    user="root",
    password="Votazioneseggio2022!",
    database="Seggio"
)
Cursor = DataBase.cursor()

# Shared secret between the Arduino and the server
SecureToken = "R3,u=t?_~LRrPycS"

# Force all the connection on HTTPS
app = Flask(__name__)
sslify = SSLify(app)

# Inizialize the connection with Ganache
provider = Web3.HTTPProvider('http://127.0.0.1:7545')
w3 = Web3(provider)

# Opens the JSON and takes the ABI of the Smart Contract SecureVoting.sol
with open("SecureVoting.json") as f:
    info_json = json.load(f)
abi = info_json["abi"]

# Inizialize the smart contract based on its address
# secureVotingAddress = "0x511c95B2cf0d2b24b7eD948dAC1f9b27A2b1cE1d"
secureVotingAddress = "0x42d8E580c1b85b22cAF716b794cAccA036254533"
contract = w3.eth.contract(address=secureVotingAddress, abi=abi)

# This function controls if the request has the secret shared token and it's performed before each function
@app.before_request
def verify_token():
    auth_header = request.headers.get("Authorization")
    if auth_header:
        if auth_header != SecureToken:
            return jsonify({"error": "You're not authorized"}), 401
    else:
        return jsonify({"error": "Missing Authorization header"}), 401

# This function checks if the server can comunicate with Ganache
@app.route('/isConnectedGanache', methods=['GET'])
def isConnected():
    try:
        return str(w3.isConnected())
    except:
        return jsonify({"error": "isConnect function does not work properly"}), 500

# This function returns a Blockchain ID given an RFID ID
@app.route('/authenticate', methods=['POST'])
def authenticate():
    try:
        payload = request.get_data().decode('utf-8')
        RFID = payload.split('=')[1]
        Cursor.execute("SELECT BlockchainID FROM Votante WHERE RFID = (%s)", (RFID,))
        for element in Cursor.fetchone():
            actualBlockchainID = element
        return str(actualBlockchainID)
    except:
        return jsonify({"error": "authenticate function does not work properly"}), 500

# This function allows to vote given a Blockchain account ID.
# You can't vote more than one time.
@app.route('/vote', methods=['POST'])
def vote():
    try:
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
        if result == 1:
            return "Already Voted!"
        else:
            contract.functions.vote(str(uid), int(candidateid)).transact({"from": actualAccount})
            Cursor.execute("UPDATE Votante SET alreadyVoted = 1 WHERE RFID = (%s)", (RFID,))
            DataBase.commit()
            return str(contract.functions.totalVotes(int(candidateid)).call())
    except:
        return jsonify({"error": "vote function does not work properly"}), 500

# Starts the server and loads the certificates for HTTPS
if __name__ == '__main__':
    app.run(ssl_context=('cert/certificate.pem', 'cert/privatekey.pem'), host='0.0.0.0', port=443)
