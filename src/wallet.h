#ifndef WALLET_H__
#define WALEET_H__
#include "utils.h"

class Wallet
{
private:
  ecdsa::Key key;
public:
    Wallet(const char* privKeyBase58){
      std::vector<uint8_t> privKeyData;
      base58::DecodeBase58(std::string(privKeyBase58),privKeyData);
      key.set_priv_key_data(privKeyData);
    }
    Wallet() = default;
    std::string GetAddress() const;
    std::string GetPrivateKey() const;
    std::string GetPubKey() const;
    std::string Sign(std::string message);
    bool Verify(std::string message, std::string sig);
    
};

#endif