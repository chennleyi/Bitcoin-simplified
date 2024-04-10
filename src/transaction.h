#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include <string>
#include <vector>
#include <memory>
#include "wallet"
#include "blockchain.h"

class Blockchain;

class TxInput{
private:
    std::string txid;
    int vout;
    std::string scriptSig;
    std::string pubkey;

public:
    TxInput(std::string t, int v, std::string pk):txid(t),vout(v),pubkey(pk){}
    TxInput() = default;

    bool canUnlockOutputWith(std::string) const;
    bool IsCoinbase() const;


    const std::string getTxid() const;
    int getVout() const;
    const std::string& getScriptSig() const;
    const std::string& getPubkey() const;


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
    TxOutput(int val, std::string scriptpubkey):value(val), pubkeyHash(scriptpubkey){}
    TxOutput() = default; 
    bool canBeUnlockedWith(std::string) const; 
    int getValue() const;
    std::string getScriptPubKey() const;

    template<class Archive>
    void serialize(Archive& archive){
        archive(value, pubkeyHash);
    }


};


class Transaction{
public:
    std::vector<TxInput> txInputs;
    std::vector<TxOutput> txOutputs;
    std::string id;
public:
    Transaction(std::vector<TxInput>& _txInputs, std::vector<TxOutput>& _txOutputs):txInputs(_txInputs),txOutput(_txOutputs){}
    Transaction() = default;
    
    void setId();
    bool isCoinbase() const;

    void signTx(Transaction& t);
    void verifyTx(Transaction& t);

    template<class Archive>
    void serialize(Archive& archive){
        archive(txInput, txOutput, id);
    }
};

// 创建
Transaction NewCoinbaseTx(std::string to, std::string data);
Transaction NewUTXOTransaction(Wallet& w, std::string to, int amount, Blockchain& blockchain);
Transaction TrimmedTx(Transaction& tx);
#endif