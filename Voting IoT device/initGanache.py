from web3 import Web3
import json

# Inizialize the connection with Ganache
provider = Web3.HTTPProvider('http://127.0.0.1:7545')
w3 = Web3(provider)

# Opens the JSON and takes the ABI of the Smart Contract SecureVoting.sol
with open("SecureVoting.json") as f:
    info_json = json.load(f)
abi = info_json["abi"]

# Inizialize the smart contract based on its address
secureVotingAddress = "0x42d8E580c1b85b22cAF716b794cAccA036254533"
contract = w3.eth.contract(address=secureVotingAddress, abi=abi)

# Takes the first account, the one who deployed the Smart Contract and the only one
# who can add candidates
ownerAccount = w3.eth._get_accounts()[0]

# Addes 4 candidates to permit the voting
contract.functions.addCandidate("Gerardo", "Lega Nord").transact({"from": ownerAccount})
contract.functions.addCandidate("Giovanni", "Partito Democratico").transact({"from": ownerAccount})
contract.functions.addCandidate("Mario", "Europa+").transact({"from": ownerAccount})
contract.functions.addCandidate("Antonio", "Movimento 5 Stelle").transact({"from": ownerAccount})

# Prints the number of candidates
numberOfCandidates = contract.functions.getNumOfCandidates().call()
print("Number of Candidates: " + str(numberOfCandidates))

# Print the candidates informations
i = 0
while i < numberOfCandidates:
    print(contract.functions.getCandidate(i).call())
    i = i + 1
