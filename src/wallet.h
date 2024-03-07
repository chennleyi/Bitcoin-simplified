#ifndef WALLET_H__
#define WALEET_H__
#include "utils.h"

class Wallet
{
private:
    std::unique_ptr<ecdsa::Key> pkey;
public:
    Wallet(){
        pkey = std::make_unique<ecdsa::Key>();
    }
    std::string GetAddress() const;
    std::string GetPrivateKey() const;
    std::string GetPrivateKeyHex() const;
    std::string GetPubKey() const;
};

#endif