#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>
#include <openssl/sha.h>
#include <ecdsa/base58.h>
#include <ecdsa/key.h>
#include <cstring>

time_t getUnixTime();

std::string getSha2(std::string text);

std::vector<uint8_t> ripemd160(const uint8_t *data, int len);

std::vector<uint8_t> sha256(const uint8_t *data, int len);

std::tuple<std::vector<uint8_t>, bool> HashText(const std::string &text);

std::tuple<std::vector<uint8_t>, bool> Signing(std::shared_ptr<ecdsa::Key> pkey, const std::string &path);

bool Verifying(const ecdsa::PubKey &pub_key, const std::string &path, const std::vector<uint8_t> &signature);

std::string BinaryToHexString(const unsigned char *bin_data, size_t size, bool is_little_endian);

std::string FromPublicKey(const std::vector<uint8_t> &pub_key, unsigned char prefix_char, unsigned char *out_hash160 = nullptr);
#endif