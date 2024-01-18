//
// Created by Chenleyi on 2024/01/16
//

#include "block.h"
#include "proofofwork.h"
#include "blockchain.h"
#include <assert.h>
#include <leveldb/db.h>
#include <iostream>
#include <string>
using std::endl;
using std::cout;
using std::string;
int main(int argc,char** argv){
    Blockchain bc(newBlockchain());
    bc.addBlock("Jan 17 2024 13:08");
    bc.addBlock("send 1 btc to Jack");
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;

    // open
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());

    string key = "陈乐义";
    string value = "Alina";

    // write
    status = db->Put(leveldb::WriteOptions(), key, value);
    assert(status.ok());

    // read
    status = db->Get(leveldb::ReadOptions(), key, &value);
    assert(status.ok());

    cout << "value of key: " << key << " is " << value << endl;

    // delete
    status = db->Delete(leveldb::WriteOptions(), key);
    assert(status.ok());

    status = db->Get(leveldb::ReadOptions(), key, &value);
    assert(!status.ok());

    // close
    delete db;

    return 0;
}