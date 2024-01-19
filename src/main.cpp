//
// Created by Chenleyi on 2024/01/16
//

#include "block.h"
#include "proofofwork.h"
#include "blockchain.h"
#include <assert.h>
#include <leveldb/db.h>
#include <leveldb/iterator.h>
#include <iostream>
#include <string>
#include "cista.h"
#include <vector>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <sstream>
    
using std::endl;
using std::cout;
using std::string;


int main()
{
    leveldb::DB* db;
    leveldb::Options options;

    // 设置数据库路径和其他选项
    options.create_if_missing = true; // 如果数据库不存在，则不创建

    // 打开数据库
    leveldb::Status status = leveldb::DB::Open(options, "/learn/cpp/Bitcoin-simplified/data", &db);

    if (status.ok()) {
        std::cout << "Database opened successfully." << std::endl;

        // 写入数据
        std::string key1 = "key1";
        std::string value1 = "value1";
        status = db->Put(leveldb::WriteOptions(), key1, value1);

        std::string key2 = "key2";
        std::string value2 = "value2";
        status = db->Put(leveldb::WriteOptions(), key2, value2);

        // 读取数据
        std::string readValue;
        status = db->Get(leveldb::ReadOptions(), key1, &readValue);
        if (status.ok()) {
            std::cout << "Read value for key1: " << readValue << std::endl;
        } else {
            std::cout << "Error reading data for key1: " << status.ToString() << std::endl;
        }

        // 遍历数据
        leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            std::cout << "Key: " << it->key().ToString() << ", Value: " << it->value().ToString() << std::endl;
        }
        if (!it->status().ok()) {
            std::cout << "Error iterating over data: " << it->status().ToString() << std::endl;
        }
        delete it;

        // 关闭数据库
        delete db;
    } else {
        std::cout << "Error opening database: " << status.ToString() << std::endl;
    }

    return 0;
}

// int main(int argc,char** argv){
//     Blockchain bc(newBlockchain());
//     bc.addBlock("Jan 17 2024 13:08");
//     bc.addBlock("send 1 btc to Jack");
//     return 0;
// }