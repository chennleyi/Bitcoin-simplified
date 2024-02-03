#include "block.h"
#include "proofofwork.h"
#include "utils.h"
#include <sstream>
#include <memory>
#include <string>
#include <iostream>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>


Block::Block(time_t time, std::vector<Transaction> trans, std::string prevblockhash):timestamp(time), transactions(trans), prevBlockHash(prevblockhash) {}

void Block::setHash(std::string _hash){
    this->hash = _hash;
}

void Block::setNonce(int64_t _nonce){
    this->nonce = _nonce;
}

time_t Block::getTimestamp() const {
    return timestamp;
}

std::string Block::getPrevBlockHash() const {
    return prevBlockHash;
}

std::string Block::getHash() const {
    return hash;
}

std::int64_t Block::getNonce() const {
    return nonce;
}


std::string Block::HashTransaction() const {
    std::string total = "";
    for(auto& transaction: transactions){
        total += transaction.id;
    }
    return getSha2(total);
}

std::string cerealBlock(Block b) {
    std::stringstream ss; 
    {
        cereal::BinaryOutputArchive oarchive(ss); 
        oarchive(b); // Write the data to the archive
    }
    return ss.str();
}

Block decerealBlock(std::string info) {
    Block b;
    std::stringstream dd;
    dd << info;
    cereal::BinaryInputArchive oarchive(dd); 
    oarchive(b); // Write the data to the archive 
    return b;
}

Block newBlock(std::vector<Transaction>& trans, std::string prevBlockHash) {
    Block b(getUnixTime(), trans, prevBlockHash);
    Proofofwork p(b);
    b.setHash(p.getHash());
    b.setNonce(p.getNonce());
    return b;
}

Block newGenesisBlock(Transaction& coinbase) {
    Block b(getUnixTime(), std::vector<Transaction>{coinbase}, "0");
    Proofofwork p(b);
    b.setHash(p.getHash());
    b.setNonce(p.getNonce());
    return b;
}