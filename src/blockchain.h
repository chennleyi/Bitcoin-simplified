//
// Created by Chenleyi on 2024/01/16
//

#ifndef BLOCKCHAIN_H_
#define BLOCKCHAIN_H_
#include <vector>
#include <string>
#include <leveldb/db.h>

class Block;
class Blockchain{
public:
    Blockchain(Block);
    Blockchain() = default;
    void addBlock(std::string data);
private:
    std::vector<Block> blocks;
};

Blockchain newBlockchain();

#endif