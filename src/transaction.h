#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include <string>
#include <vector>
#include <memory>
#include "blockchain.h"

class Blockchain;

class TxInput{
private:
    std::string txid;
    int vout;
    std::string scriptSig;
    std::string pubkey;

public:
    TxInput(std::string t, int v, std::string s):txid(t),vout(v),scriptSig(s){}
    TxInput() = default;
    bool canUnlockOutputWith(std::string) const;
    const std::string getTxid() const;
    int getVout() const;
    const std::string& getScriptSig() const;
    bool IsCoinbase() const;

    template<class Archive>
    void serialize(Archive& archive){
        archive(txid, vout, scriptSig);
    }


};


class TxOutput{
private:
    int value;
    std::string pubkeyHash;
public:       
    TxOutput(int val, std::string scriptpubkey):value(val), scriptPubkey(scriptpubkey){}
    TxOutput() = default; 
    bool canBeUnlockedWith(std::string) const; 
    int getValue() const;
    std::string getScriptPubKey() const;

    template<class Archive>
    void serialize(Archive& archive){
        archive(value, scriptPubkey);
    }


};


class Transaction{
public:
    std::vector<TxInput> txInput;
    std::vector<TxOutput> txOutput;
    std::string id;
public:
    Transaction(std::vector<TxInput>& txinput, std::vector<TxOutput>& txoutput):txInput(txinput),txOutput(txoutput){}
    Transaction() = default;
    
    void setId();
    bool isCoinbase() const;


    template<class Archive>
    void serialize(Archive& archive){
        archive(txInput, txOutput, id);
    }
};

// 创建
Transaction NewCoinbaseTx(std::string to, std::string data);
Transaction NewUTXOTransaction(std::string from, std::string to, int amount, Blockchain& blockchain);

#endif