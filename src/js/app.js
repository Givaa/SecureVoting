import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.15.0/firebase-app.js'
import { getAuth, signInWithEmailAndPassword, onAuthStateChanged } from 'https://www.gstatic.com/firebasejs/9.15.0/firebase-auth.js'

const firebaseConfig = {
  apiKey: "AIzaSyCkGV1K6geqgdi3eYEGaDlKmKoVsK8IGAo",
  authDomain: "voting-68398.firebaseapp.com",
  projectId: "voting-68398",
  storageBucket: "voting-68398.appspot.com",
  messagingSenderId: "299619990480",
  appId: "1:299619990480:web:0faa7193a3ab1860f857d5"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth(app);

const App = {

  signedInUser: null,
  web3Provider: null,
  contracts: {},

  init: async function () {

    // to be deleted
    signInWithEmailAndPassword(auth, 'test@login.it', 'ciao1234').then(userCredential => {
      // Signed in 
      const user = userCredential.user;
      // ...
    }).catch((error) => {
      console.log(error);
    });

    onAuthStateChanged(auth, async (user) => {
      if (user) {
        App.signedInUser = user;
        console.log('signed in!');
        return await App.initWeb3();
      } else {
        // User is signed out
        console.log('nope');
      }
    });


  },

  initWeb3: async function () {

    // Modern dapp browsers...
    if (window.ethereum) {
      App.web3Provider = window.ethereum;
      try {
        // Request account access
        await window.ethereum.enable();
      } catch (error) {
        // User denied account access...
        console.error("User denied account access")
      }
    }
    // Legacy dapp browsers...
    else if (window.web3) {
      App.web3Provider = window.web3.currentProvider;
    }
    // If no injected web3 instance is detected, fall back to Ganache
    else {
      App.web3Provider = new Web3.providers.HttpProvider('http://localhost:7545');
    }
    web3 = new Web3(App.web3Provider);

    return App.initContract();
  },

  initContract: function () {

    $.getJSON('SecureVoting.json', function (data) {
      // Get the necessary contract artifact file and instantiate it with @truffle/contract
      var SecureVotingArtifact = data;
      App.contracts.SecureVoting = TruffleContract(SecureVotingArtifact);

      // Set the provider for our contract
      App.contracts.SecureVoting.setProvider(App.web3Provider);

      // Use our contract to retrieve and mark the adopted pets
      // DO SOMETHING
    });

    console.log(App.signedInUser);

    return App.bindEvents();
  },

  bindEvents: function () {
    $(document).on('click', '.btn-adopt', App.handleVote);
  },

  handleVote: function (event) {
    event.preventDefault();

    var candidateId = parseInt($(event.target).data('id'));

    console.log('oh yeah');
  }

};

$(function () {
  $(window).on('load', function () {
    App.init();
  });
});
