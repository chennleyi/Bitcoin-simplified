#ifndef BLOCK_H_
#define BLOCK_H_
#include "transaction.h"
#include <vector>
#include <string>
#include <cstdint>


class Block{
private:
    time_t timestamp{0};
    std::string prevBlockHash{""};
    std::string hash{""};
    int64_t nonce{0};
public:
    std::vector<Transaction> transactions{};
public:
    Block(time_t time, std::vector<Transaction> trans, std::string prevblockhash);
    Block() = default;
    
    void setHash(std::string);
    void setNonce(int64_t);

    std::string HashTransaction() const;
    time_t getTimestamp() const;
    std::string getPrevBlockHash() const;
    std::string getHash() const;
    std::int64_t getNonce() const;

    
    template<class Archive>
    void serialize(Archive& archive){
        archive(timestamp, prevBlockHash, hash, nonce, transactions);
    }


};

Block newGenesisBlock(Transaction& coinbase);
Block newBlock(std::vector<Transaction>& trans, std::string prevBlockHash);
std::string cerealBlock(Block b);
Block decerealBlock(std::string info);


#endif