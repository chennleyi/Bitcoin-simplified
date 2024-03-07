#include "wallet.h"

std::string Wallet::GetAddress() const
{
    auto pub_key = pkey->CreatePubKey();
    unsigned char* hash160 = nullptr;
    unsigned char prefix_char = '\0';
    // std::cout<<prefix_char<<std::endl;
    std::string t = FromPublicKey(pub_key.get_pub_key_data(),prefix_char, hash160);
    return t;
}

std::string Wallet::GetPrivateKey() const
{
    return base58::EncodeBase58(pkey->get_priv_key_data());
}

std::string Wallet::GetPrivateKeyHex() const
{
    return BinaryToHexString(pkey->get_priv_key_data().data(), pkey->get_priv_key_data().size(), true);
}

std::string Wallet::GetPubKey() const
{
    return base58::EncodeBase58(pkey->get_pub_key_data());
}