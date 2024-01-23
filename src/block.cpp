#include "block.h"
#include "proofofwork.h"
#include "utils.h"
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <sstream>
#include <memory>
#include <string>
#include <iostream>

Block::Block(time_t time, std::string data, std::string prevBlockHash):timestamp(time), data(data), prevBlockHash(prevBlockHash) {}

Block:: Block(const Block& other){
    this->data = other.getData();
    this->hash = other.getHash();
    this->nonce = other.getNonce();
    this->timestamp = other.getTimestamp();
    this->prevBlockHash = other.getPrevBlockHash();
}

Block& Block::operator=(const Block& other)
{
    if (this != &other) {
        this->data = other.getData();
        this->hash = other.getHash();
        this->nonce = other.getNonce();
        this->timestamp = other.getTimestamp();
        this->prevBlockHash = other.getPrevBlockHash();      
    }
    return *this;
}

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

std::string Block::getData() const {
    return data;
}

std::string Block::getHash() const {
    return hash;
}

std::int64_t Block::getNonce() const {
    return nonce;
}

std::string cerealBlock(Block b) {
    std::stringstream ss; 
    {
        cereal::BinaryOutputArchive oarchive(ss); 
        oarchive(b); // Write the data to the archive
    }
    // std::cout<<ss.str()<<std::endl;
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

Block newBlock(std::string data, std::string prevBlockHash) {
    Block b(getUnixTime(), data, prevBlockHash);
    Proofofwork p(b);
    b.setHash(p.getHash());
    b.setNonce(p.getNonce());
    // spdlog::info("data: {}, prev hash: {}, hash: {}", data, prevBlockHash, b.getHash());
    return b;
}

Block newGenesisBlock() {
    Block b(getUnixTime(), "first block generated", "0");
    b.setHash("59f7a1c5a50e20b59d772242729577c715ab6900712f70f0434b44141fffa456");
    return b;
}