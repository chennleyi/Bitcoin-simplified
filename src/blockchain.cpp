#include "blockchain.h"
#include "block.h"
#include "proofofwork.h"
#include "transaction.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include <iostream>

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
    for(auto＆ tx: transactions){
        
    }
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
    leveldb::Status status = leveldb::DB::Open(options, "/home/runner/Xiang-Mu/Bitcoin-simplified/data/testdb", &db);
    if(false == status.ok()){
        std::cout<<"open failure";
        throw std::runtime_error("g");
    }
    if(hasKeyWithPrefix(db, blocksBucket) == false){
        std::cout<<"No blockchain existed, create a new one";
        auto cbtx = NewCoinbaseTx(addr, std::string("The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"));
        auto genesis = newGenesisBlock(cbtx);
        std::cout<<"Mining the Genesis block\n\nSUCESS!!!\n\n";
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
        std::cout<<"Prev hash: "<<b.getPrevBlockHash();

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
    std::unordered_map<std::string, std::vector<int>> umap;
    while(true){
        std::string value;
        db->Get(leveldb::ReadOptions(), blocksBucket+currentHash, &value); 
        Block b = decerealBlock(value);
        for(auto tx: b.transactions){
            for(int i = 0; i < tx.txOutput.size(); ++i){
                if(umap.find(tx.id) != umap.end()){
                    for(auto j: umap[tx.id]){
                        if(i == j){
                            goto START;
                        }
                    }
                }
                if(requirement){
                    START:
                    continue;
                }
                if(tx.txOutput[i].canBeUnlockedWith(address)){
                    s.smap[tx.id].push_back(i);
                    s.value += tx.txOutput[i].getValue();
                }
            }
            if(tx.isCoinbase() != true){
                for(auto input: tx.txInput){
                    umap[input.getTxid()].push_back(input.getVout());
                }
            }
        }
        currentHash = b.getPrevBlockHash();
        if(b.getPrevBlockHash() == "0")
                break;
    }
    return s;
}

int Blockchain::GetBalance(std::string address) const{
    auto spent = FindUnspentOutput(address);
    return spent.value;
}


void Blockchain::Send(std::string from, std::string to, int amount){
    auto tx = NewUTXOTransaction(from, to, amount, *this);
    std::vector<Transaction> arr;
    arr.push_back(tx);
    (*this).mineBlock(arr);
}

Transaction Blockchain::FindTx(std::string txid){
    std::string currentHash = tip;
    while(true){
        std::string value;
        db->Get(leveldb::ReadOptions(), blocksBucket+currentHash, &value); 
        Block b = decerealBlock(value);
        for(auto& tx: b.transactions){
            if(tx.id == txid){
                return tx;
            }
        }
        if(b.getPrevBlockHash() == "0"){
            break;
        }
    }
    throw std::runtime_error("Couldn't find the specific tx");
}

Blockchain::~Blockchain(){
    delete db;
}




