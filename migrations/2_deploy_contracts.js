var SecureVoting = artifacts.require("./SecureVoting.sol")

module.exports = function(deployer) {
  deployer.deploy(SecureVoting)
}