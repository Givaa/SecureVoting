// SPDX-License-Identifier: MIT
pragma solidity ^0.7.0;

contract SecureVoting {
    // Questo evento serve per loggare quando viene aggiunto un nuovo candidato
    // Serve per il frontend per avere il riferimento alla votazione.
    event AddedCandidate(uint256 candidateID);

    // Indirizzo della persona che vota.
    address owner;

    // Costruttore.
    constructor() {
        owner = msg.sender;
    }

    // Modifier che si assicura che effettivamente ad eseguire il resto del codice
    // sia la persona che sta votando.
    modifier onlyOwner() {
        require(msg.sender == owner);

        // Questo carattere si usa in Solidity, solo in una funzione modifier, per far si
        // che si possa eseguire il resto del codice.
        _;
    }

    // Struct che descrive il Votante.
    struct Voter {
        string uid; // Possibile ottimizzazione con bytes32
        uint256 candidateIDVote;
    }

    // Struct che descrive un Candidato.
    struct Candidate {
        string name;
        string party;
        // Ci assicuriamo che esista davvero.
        bool doesExist;
    }

    // Variabili di stato che vengono salvate sulla Blockchain per il numero di voti totali e il numero di candidati totale.
    uint256 numCandidates;
    uint256 numVoters;

    // Hash-table che ci permettono di tenere conto di candidati e voti.
    mapping(uint256 => Candidate) candidates;
    mapping(uint256 => Voter) voters;

    function addCandidate(string calldata name, string calldata party) public onlyOwner {
        // Aggiorna il numero di Candidati totale.
        uint256 candidateID = numCandidates++;
        // Crea un nuovo candidato e lo aggiunge al mapping.
        candidates[candidateID] = Candidate(name, party, true);
        // Logghiamo che è stato aggiunto un candidato.
        emit AddedCandidate(candidateID);
    }

    function vote(string calldata uid, uint256 candidateID) public {
        // Controlla se esiste il candidato per cui votiamo.
        if (candidates[candidateID].doesExist == true) {
            uint256 voterID = numVoters++; // Numero di voti totale viene aggiornato
            voters[voterID] = Voter(uid, candidateID);
        }
    }

    // Conta il numero di voti per un Candidato tramite un loop di tutte le votazioni effettuate.
    function totalVotes(uint256 candidateID) public view returns (uint256) {
        uint256 numOfVotes = 0;
        for (uint256 i = 0; i < numVoters; i++) {
            // Incrementa il numero di voti se l'ID corrisponde.
            if (voters[i].candidateIDVote == candidateID) {
                numOfVotes++;
            }
        }
        return numOfVotes;
    }

    // Ritorna il numero di candidati totale.
    function getNumOfCandidates() public view returns (uint256) {
        return numCandidates;
    }

    // Ritorna il numero di voti totale.
    function getNumOfVoters() public view returns (uint256) {
        return numVoters;
    }

    // Ritorna tutte le informazioni su un Candidato.
    function getCandidate(uint256 candidateID)
        public
        view
        returns (
            uint256,
            string memory,
            string memory
        )
    {
        return (
            candidateID,
            candidates[candidateID].name,
            candidates[candidateID].party
        );
    }
}
