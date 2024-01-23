#include "blockchain.h"
#include "cxxopts.hpp"

int main(int argc,char** argv){
    Blockchain b;
    cxxopts::Options options("Bitcoin simplified", "A simplified version of Bitocin");
    options.add_options()
    ("d,addData", "Add data to transaction", cxxopts::value<std::string>());

    options.add_options()
    ("p,printChain", "Print the the entire blockchain's blocks' info");

    auto result = options.parse(argc, argv);

    if(result.count("addData")){
        b.addBlock(result["addData"].as<std::string>());
    }
    if(result.count("printChain")){
        b.printChain();
    }
}