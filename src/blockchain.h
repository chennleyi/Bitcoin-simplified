#ifndef BLOCKCHAIN_H_
#define BLOCKCHAIN_H_
#include <vector>
#include <string>
#include <leveldb/db.h>
#include <map>

const std::string blocksBucket = "blocksBucket";

class Block;
class Blockchain;
class Transaction;
class TxOutput;
class TxIntput;

struct SpendableOutput{
    int value{0};
    std::map<std::string, std::vector<int>> smap;
};


class Blockchain{
public:
    Blockchain(std::string addr);
    Blockchain() = default;
    ~Blockchain();
    
    void mineBlock(std::vector<Transaction> transactions);
    SpendableOutput FindUnspentOutput(std::string address) const;
    void Send(std::string from, std::string to, int amount);

    void printChain();
    void GetBalance(std::string addr) const;
    Transaction FindTx(std::string txid);
private:
    std::string tip;
    leveldb::DB* db = nullptr;
};

#endif