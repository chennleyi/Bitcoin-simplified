//
// Created by Chenleyi on 2024/01/16
//

#include "block.h"
#include "proofofwork.h"
#include "blockchain.h"

// #include <iostream>
// #include <string>
// #include <vector>
// #include <fstream>
// #include <sstream>
    

int main(int argc,char** argv){
    Blockchain bc(newBlockchain());
    bc.addBlock("Jan 17 2024 13:08");
    bc.addBlock("send 1 btc to Jack");
    return 0;
}