#include <chrono>
#include <ctime>
#include <cstdint>
#include "sha256/SHA256.h"

std::string getSha2(std::string text){
    SHA256 sha;
    sha.update(text);
    return SHA256::toString(sha.digest());
}

time_t getUnixTime(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}