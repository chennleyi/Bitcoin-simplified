#include "blockchain.h"
#include "block.h"
#include "proofofwork.h"
#include "transaction.h"
#include "spdlog/spdlog.h"
#include <unordered_map>
#include <vector>
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
void Blockchain::mineBlock(std::vector<Transaction> transactions){
    std::string x;
    if(hasKeyWithPrefix(db, blocksBucket) == true){
        leveldb::ReadOptions readOptions;
        leveldb::Status readStatus = db->Get(readOptions, blocksBucket+"1", &x);
        if(false == readStatus.ok()){
            throw std::runtime_error("read error");
        }
        Block nBlock = newBlock(transactions, x);
        std::cout<<nBlock.getHash()<<"\n\nSUCCESS!!!\n\n";
        storeDataInBucket(db, blocksBucket, nBlock.getHash(), cerealBlock(nBlock));
        storeDataInBucket(db, blocksBucket, "1", nBlock.getHash());  
        tip = nBlock.getHash();
    } 

}


Blockchain:: Blockchain(std::string addr){
    leveldb::Options options;  
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/learn/cpp/Bitcoin-simplified/data/testdb", &db);
    if(false == status.ok()){
        spdlog::warn("open failure");
        throw std::runtime_error("g");
    }
    if(hasKeyWithPrefix(db, blocksBucket) == false){
        spdlog::info("No blockchain existed, create a new one");
        auto cbtx = NewCoinbaseTx(addr, std::string("The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"));
        auto genesis = newGenesisBlock(cbtx);
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
        spdlog::info("Timestamp: {}\n\n\n", b.getTimestamp());
        currentHash = b.getPrevBlockHash();
        if(b.getPrevBlockHash() == "0")
            break;
    }
}


SpendableOutput Blockchain::FindUnspentOutput(std::string address) const
{
    SpendableOutput s;
    std::string currentHash = tip;
    bool requirement = false;
    // std::cout<<"-----------------------------\n";
    while(true){
        std::string value;
        db->Get(leveldb::ReadOptions(), blocksBucket+currentHash, &value); 
        Block b = decerealBlock(value);
        // 遍历tx
        for(auto tx: b.transactions){
            // 遍历tx的outputs
            for(int i = 0; i < tx.txOutput.size(); ++i){
                // 这个output被引用
                if(s.smap.find(tx.id) != s.smap.end()){
                    for(auto j: s.smap[tx.id]){
                        if(i == j){
                            // std::cout<<i<<" ";
                            goto START;
                        }
                    }
                }
                if(requirement){
                    START:
                    continue;
                }
                // 没被引用加入
                if(tx.txOutput[i].canBeUnlockedWith(address)){
                    s.smap[tx.id].push_back(i);
                    s.value += tx.txOutput[i].getValue();
                    // std::cout<<"没有被引用： \n";
                    // std::cout<<tx.id<<" add value : "<<i<<" "<<tx.txOutput[i].getValue()<<"; ";
                }
                // std::cout<<"\n";
            }
            if(tx.isCoinbase() != true){
                for(auto input: tx.txInput){
                    s.smap[input.getTxid()].push_back(input.getVout());
                    // std::cout<<"被引用： \n";
                    // std::cout<<input.getTxid()<<": "<<input.getVout()<<" ";
                }
                // std::cout<<"\n";
            }
        }
        currentHash = b.getPrevBlockHash();
        if(b.getPrevBlockHash() == "0")
                break;
    }
    // std::cout<<"-----------------------------\n";
    return s;
}

// // vector中第一个数字是address有多少可用的value，剩余的是tx对应的ouput的indice
// SpendableOutput Blockchain::FindSpendableOutput(std::string address, int amount) const {
//     auto unspentTxs = FindUnspentOutput(address);
//     SpendableOutput sOutputs;
//     for(auto& tx: unspentTxs){
//         int id = 0;
//         for(auto& ouput: tx.txOutput){
//             if(ouput.canBeUnlockedWith(address) && sOutputs.value < amount){
//                 sOutputs.value += ouput.getValue();
//                 sOutputs.smap[tx.id].push_back(id);
//                 if(sOutputs.value >= amount)
//                     goto OK;
//             }
//             id++;
//         }
//     }
//     OK:
//     return sOutputs;
// }



void Blockchain::GetBalance(std::string address) const{
    auto spent = FindUnspentOutput(address);
    std::cout<<"The balance of "<<address<<" is "<<spent.value<<"\n";
}


void Blockchain::Send(std::string from, std::string to, int amount){
    auto tx = NewUTXOTransaction(from, to, amount, *this);
    std::vector<Transaction> arr;
    arr.push_back(tx);
    (*this).mineBlock(arr);
}


Blockchain::~Blockchain(){
    delete db;
}




