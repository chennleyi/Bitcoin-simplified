#include "proofofwork.h"
#include "utils.h"
#include "block.h"
#include "utils.h"
#include <cstdint>
#include <string>
#include <limits>
#include <gmpxx.h>

Proofofwork:: Proofofwork(Block& b){
    
    std::string data = std::to_string(b.getTimestamp()) + b.HashTransaction() + b.getPrevBlockHash();
    
    mpz_init(number);
    mpz_set_ui(number, 1);
    mpz_mul_2exp(number, number, 250);

    while(true){
        mpz_t hashNumber;
        std::string t = getSha2(data+std::to_string(nonce));
        mpz_init_set_str(hashNumber, t.c_str(), 16);  // 将256位哈希值转换为数字
        if(mpz_cmp(hashNumber, number) < 0){
            break;
        }
        if(nonce < std::numeric_limits<uint64_t>::max())
            nonce++;
    }
    hash = getSha2(data+std::to_string(nonce));
}

bool Proofofwork:: validate() const{
    mpz_t hashNumber;
    mpz_init_set_str(hashNumber, hash.c_str(), 16);  // 将256位哈希值转换为数字
    return mpz_cmp(hashNumber, number) > 0;
}

int64_t Proofofwork::getNonce() const{
    return nonce;
}

std::string Proofofwork::getHash() const{
    return hash;
}