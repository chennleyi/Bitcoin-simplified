#include "block.h"
#include "proofofwork.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include <memory>
#include <string>

Block::Block(time_t time, std::string data, std::string prevBlockHash):timestamp(time), data(data), prevBlockHash(prevBlockHash) {}

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

Block newBlock(std::string data, std::string prevBlockHash) {
    Block b(getUnixTime(), data, prevBlockHash);
    Proofofwork p(b);
    b.setHash(p.getHash());
    b.setNonce(p.getNonce());
    spdlog::info("data: {}, prev hash: {}, hash: {}", data, prevBlockHash, b.getHash());
    return b;
}

Block newGenesisBlock() {
    Block b(getUnixTime(), "first block generated", "59f7a1c5a50e20b59d772242729577c715ab6900712f70f0434b44141fffa456");
    b.setHash("59f7a1c5a50e20b59d772242729577c715ab6900712f70f0434b44141fffa456");
    return b;
}