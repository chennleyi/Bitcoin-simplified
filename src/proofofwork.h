#ifndef proofofwork_h_
#define proofofwork_h_
#include <cstdint>
#include <string>
#include <gmpxx.h>


class Block;
class Proofofwork{
public:
    Proofofwork(Block&);
    Proofofwork() = delete;
    int64_t getNonce() const;
    std::string getHash() const;
    bool validate() const;
private:
    int target{32};
    mpz_t number;
    uint64_t nonce;
    std::string hash{""};
};

#endif


