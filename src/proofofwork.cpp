#include "proofofwork.h"
#include "utils.h"
#include "block.h"
#include <cstdint>
#include <string>
#include "utils.h"

Proofofwork:: Proofofwork(Block& b){
    std::string data = std::to_string(b.getTimestamp()) + b.getData() + b.getPrevBlockHash();
    this->nonce = 1;
    int64_t x = 1;
    x = x << target;
    while(std::stoi(data+std::to_string(this->nonce)) > x){
        this->nonce++;
    }
    this->hash = getSha2(data+std::to_string(this->nonce));
}

int64_t Proofofwork::getNonce() const{
    return nonce;
}

std::string Proofofwork::getHash() const{
    return hash;
}