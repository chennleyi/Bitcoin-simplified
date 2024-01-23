//
// Created by Chenleyi on 2024/01/16
//

#ifndef BLOCKCHAIN_H_
#define BLOCKCHAIN_H_

#include <vector>
#include <string>
#include <leveldb/db.h>

const std::string blocksBucket = "blocksBucket";


class Block;
class Blockchain{
public:
    Blockchain();
    ~Blockchain();
    void addBlock(std::string data);
    void printChain();
    std::string tip;
    leveldb::DB* db = nullptr;
};

Blockchain newBlockchain();
#endif