#include "blockchain.h"
#include "transaction.h"
#include "spdlog/spdlog.h"
#include "cxxopts.hpp"
#include <iostream>
#include <ctime>

int main(int argc,char** argv){
    std::string from = "cly";
    Blockchain bc(from);
    cxxopts::Options options("Bitcoin simplified", "A simplified version of Bitocin");
    options.add_options()
    ("s,Send", "Add data to transaction", cxxopts::value<std::vector<std::string>>());

    options.add_options()
    ("g,getBalance", "Get the balance of specific address", cxxopts::value<std::string>());

    options.add_options()
    ("p,printChain", "Print each block's info");

    auto result = options.parse(argc, argv);

    if(result.count("Send")){
        std::vector<std::string> res = result["Send"].as<std::vector<std::string>>();
        bc.Send(res[0], res[1], std::stoi(res[2]));
    }
    if(result.count("printChain")){
        bc.printChain();
    }
    if(result.count("getBalance")){
        auto res = result["getBalance"].as<std::string>();
        bc.GetBalance(res);
    }
}