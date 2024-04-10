#include <gtest/gtest.h>
#include "wallet.h"
#include "blockchain.h"
#include "block.h"
#include "proofofwork.h"
#include "transaction.h"
#include <iostream>
#include <unistd.h>

class TestFixture : public ::testing::Test {
protected:
    // 测试之前把数据库删除了，不然数据不好做验证
    void SetUp() override {
      std::string command = "rm -rf ../data/testdb";
      system(command.c_str());
    }
};

const char *sigBase58 = "381yXZBRv3MAiTfKSkaKsgQ2Te4P6eHWpcuecnsyjpeeyVzkJVby9ZcZHtpeXQ4dq8xw4UGvQFzXhaJZ8oXyuwAjf62r83Zk";
const char *privKeyBase58 = "DNymwn5s2gRgXGUxxvP3GqeBhMU3eLw14TGmuKbvpgZ7";
std::string signMessage = "Hello world!";
Wallet w(privKeyBase58);

// 验证签名
// 验证验证
TEST_F(TestFixture, signAndVerifyMessage){
  std::string sig = w.Sign(signMessage);
  EXPECT_EQ((const char*)"hello,world", sigBase58);
  EXPECT_TRUE(w.Verify(signMessage, sig));
}

// 验证发送功能
// 验证查看钱包余额功能
TEST_F(TestFixture, TestSendBalance) {
  std::string myAddr = "cly";
  std::string yAddr = "yzy";
  std::string lAddr = "lt";
  Blockchain bc(myAddr);
  bc.Send(myAddr, lAddr, 4);
  bc.Send(myAddr, yAddr, 2);
  EXPECT_EQ(bc.GetBalance(myAddr), 4);
  EXPECT_EQ(bc.GetBalance(lAddr), 4);
  EXPECT_EQ(bc.GetBalance(yAddr), 2);
}