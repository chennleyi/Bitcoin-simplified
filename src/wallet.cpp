#include "wallet.h"

std::string Wallet::GetAddress() const
{
    auto pub_key = key.CreatePubKey();
    unsigned char* hash160 = nullptr;
    unsigned char prefix_char = '\0';
    std::string t = FromPublicKey(pub_key.get_pub_key_data(),prefix_char, hash160);
    return t;
}

std::string Wallet::GetPrivateKey() const
{
    return base58::EncodeBase58(key.get_priv_key_data());
}


std::string Wallet::GetPubKey() const
{
    return base58::EncodeBase58(key.get_pub_key_data());
}
// 将哈希后的message用key进行签名并返回base58编码的数据
std::string Wallet::Sign(std::string message){
  std::vector<uint8_t> signature;
  bool succ;
  std::tie(signature, succ) = key.Sign(sha256(message));
  if(!succ){
    throw std::runtime_error("an Error occurs while signing message");
  }
  return base58::EncodeBase58(signature);
}

// 对签名进行bese58解码,然后再与message哈希后进行验证
bool Wallet::Verify(std::string message, std::string sig){
  auto pub_key = key.CreatePubKey();
  std::vector<uint8_t> signature;
  base58::DecodeBase58(sig.c_str(), signature);
  return pub_key.Verify(sha256(message),signature);
}
