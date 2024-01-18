#ifndef proofofwork_h_
#define proofofwork_h_
#include <cstdint>
#include <string>

class Block;
class Proofofwork{
public:
    Proofofwork(Block&);
    Proofofwork() = delete;
    int64_t getNonce() const;
    std::string getHash() const;
private:
    int target = 32;
    int64_t nonce;
    std::string hash;
};

#endif


