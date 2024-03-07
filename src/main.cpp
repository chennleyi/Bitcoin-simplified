#include "wallet.h"
int main() {
    std::unique_ptr<ecdsa::Key> pkey = std::make_unique<ecdsa::Key>();
    auto pub_key = pkey->CreatePubKey();
    unsigned char hash160[20];
    unsigned char prefix_char;
    std::cout<<FromPublicKey(pub_key.get_pub_key_data(),prefix_char, hash160)<<std::endl;


    Wallet w;
    std::cout<<"Address: "<<w.GetAddress()<<std::endl;
}