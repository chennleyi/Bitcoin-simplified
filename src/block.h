//
// Created by Chenleyi on 2024/01/16
//

#ifndef BLOCK_H_
#define BLOCK_H_
#include <string>
#include <cstdint>
class Block{
public:
    Block(time_t, std::string, std::string);
    Block() = default;
    Block(const Block& );
    Block& operator=(const Block& other);
    void setHash(std::string);
    void setNonce(int64_t);
    time_t getTimestamp() const;
    std::string getData() const;
    std::string getPrevBlockHash() const;
    std::string getHash() const;
    std::int64_t getNonce() const;

    template<class Archive>
    void serialize(Archive& archive){
        archive(timestamp, data, prevBlockHash, hash, nonce);
    }
private:
    time_t timestamp;
    std::string data;
    std::string prevBlockHash;
    std::string hash;
    int64_t nonce;
};

Block newGenesisBlock();
Block newBlock(std::string data, std::string prevBlockHash);
std::string cerealBlock(Block b);
Block decerealBlock(std::string info);
#endif