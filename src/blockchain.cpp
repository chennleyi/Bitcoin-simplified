//
// Created by Chenleyi on 2024/01/16
//

#include "blockchain.h"
#include "block.h"
#include <string>


Blockchain::Blockchain(Block b){
    blocks.push_back(b);
}

void Blockchain::addBlock(std::string data){
    auto prevBlock = blocks[blocks.size() - 1];
    blocks.push_back(newBlock(data, prevBlock.getHash()));
}

Blockchain newBlockchain(){
    return Blockchain(newGenesisBlock());
}