#include <stdexcept>
#include <sstream>
#include <memory>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include "transaction.h"
#include "utils.h"

bool TxInput::canUnlockOutputWith(std::string data) const {
    return scriptSig == data;
}

const std::string TxInput::getTxid() const{
    return txid;
}

int TxInput::getVout() const{
    return vout;
}

const std::string& TxInput::getScriptSig() const{
    return scriptSig;
}

bool TxOutput::canBeUnlockedWith(std::string data) const{
    return data == scriptPubkey;
}

int TxOutput::getValue() const{
    return value;
}

std::string TxOutput::getScriptPubKey() const{
    return scriptPubkey;
}

bool Transaction::isCoinbase() const {
    return txInput.size() == 1 && txInput[0].getTxid() == "" && txInput[0].getVout() == -1;
}

/**
 * 
 * 将trasaction进行序列化
 * 并对transaction序列化的值进行sha256哈希
 * 并设置transaction.ID为其哈希值
 * 
 */
void Transaction::setId() {
    std::stringstream ss; 
    {
        cereal::BinaryOutputArchive oarchive(ss); 
        oarchive(*this); // Write the data to the archive
    }
    this->id = getSha2(ss.str());
}

Transaction NewCoinbaseTx(std::string to, std::string data)
{
    if(data == ""){
        std::stringstream ss;
        ss<<"Reward 10 bitcoin to ";
        ss<<to;
        data = ss.str();
    }

    TxInput txinput("", -1, data);
    TxOutput txoutput(10, to);
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;
    inputs.push_back(txinput);
    outputs.push_back(txoutput);
    Transaction tx(inputs, outputs);
    tx.setId();
    std::cout<<tx.id<<"\n";
    return tx;
}

/**
 * @brief 在blockchain中创建交易(发送者，收款人，数量，区块链)
 * 
 * 找到可以花费的交易ouput
 * 看数量是不是够花
 * 构建交易的inputs和outputs
 * 如果发送数量小于总余额，构建一个change
 * 用Inputs和outputs构建transaction
 * transaction设置ID
 * 返回transaction
 * 
*/
Transaction NewUTXOTransaction(std::string from, std::string to, int amount, Blockchain& bc)
{
    std::vector<TxInput> txInputs;
    std::vector<TxOutput> txOutputs;
    auto spendableOutput = bc.FindUnspentOutput(from);
    int total = spendableOutput.value;
    auto smap = spendableOutput.smap;
    if(spendableOutput.value < amount){
        throw std::runtime_error("You are trying to spend more money than you have!!!");
    }
    for(auto it = smap.begin(); it != smap.end(); it++){
        auto txid = it->first;
        auto outs = it->second;
        for(auto id: outs){
            TxInput inputs(txid, id, from);
            txInputs.push_back(inputs);
        }
    }
    txOutputs.push_back(TxOutput(amount, to));
    if(total > amount){
        txOutputs.push_back(TxOutput(total-amount, from));
    }

    Transaction tx(txInputs, txOutputs);
    tx.setId();
    return tx;
}