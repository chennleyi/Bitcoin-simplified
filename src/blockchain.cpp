//
// Created by Chenleyi on 2024/01/19
//

#include "blockchain.h"
#include "block.h"
#include "proofofwork.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include <iostream>
#include <string>


void storeDataInBucket(leveldb::DB* db, const std::string& bucketName, const std::string& key, const std::string& value) {
    std::string prefixedKey = bucketName + key;
    leveldb::Status status = db->Put(leveldb::WriteOptions(), prefixedKey, value);
}

bool hasKeyWithPrefix(leveldb::DB* db, const std::string& prefix) {
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    it->Seek(prefix);
    bool found = it->Valid() && it->key().ToString().compare(0, prefix.size(), prefix) == 0;
    delete it;
    return found;
}


// Mining the block containing "Send 1 BTC to Ivan"
// 000000d7b0c76e1001cdc1fc866b95a481d23f3027d86901eaeb77ae6d002b13

// Success!
void Blockchain::addBlock(std::string data){
    std::string x;
    if(hasKeyWithPrefix(db, blocksBucket) == true){
        leveldb::ReadOptions readOptions;
        leveldb::Status readStatus = db->Get(readOptions, blocksBucket+"1", &x);
        if(false == readStatus.ok()){
            throw std::runtime_error("read error");
        }
        spdlog::info("Mining the block containing ''{}'",data);
        Block nBlock = newBlock(data, x);
        spdlog::info("{}\n\nSUCCESS!!!\n\n", nBlock.getHash());
        storeDataInBucket(db, blocksBucket, nBlock.getHash(), cerealBlock(nBlock));
        storeDataInBucket(db, blocksBucket, "1", nBlock.getHash());  
        tip = nBlock.getHash();
    } 

}

Blockchain:: Blockchain(){
    leveldb::Options options;  
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/learn/cpp/Bitcoin-simplified/data/testdb", &db);
    if(false == status.ok()){
        spdlog::warn("open failure");
        throw std::runtime_error("g");
    }
    if(hasKeyWithPrefix(db, blocksBucket) == false){
        spdlog::info("No blockchain existed, create a new one");
        auto genesis = newGenesisBlock();
        spdlog::info("Mining the Genesis block\n\nSUCESS!!!\n\n");
        storeDataInBucket(db, blocksBucket, genesis.getHash(), cerealBlock(genesis));
        storeDataInBucket(db, blocksBucket, "1", genesis.getHash());
        tip = genesis.getHash();
    }else{
        leveldb::ReadOptions readOptions;
        status = db->Get(readOptions, blocksBucket+"1", &tip);
    }
}

void Blockchain:: printChain(){
    std::string currentHash = tip;
    while(true){
        std::string value;
        db->Get(leveldb::ReadOptions(), blocksBucket+currentHash, &value);
        Block b = decerealBlock(value);
        spdlog::info("Prev hash: {}", b.getPrevBlockHash());
        spdlog::info("Hash: {}", b.getHash());
        spdlog::info("Data: {}", b.getData());
        spdlog::info("Timestamp: {}\n\n\n", b.getTimestamp());
        currentHash = b.getPrevBlockHash();

        if(b.getPrevBlockHash() == "0")
            break;
    }
}

Blockchain::~Blockchain(){
    delete db;
}




