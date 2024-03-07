#include "utils.h"
#include "sha256/SHA256.h"
#include <chrono>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#define BUFF_SIZE 1024

std::string getSha2(std::string text){
    UTILS::SHA256 sha2;
    sha2.update(text);
    return UTILS::SHA256::toString(sha2.digest());
}


time_t getUnixTime(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}


std::tuple<std::vector<uint8_t>, bool> HashText(const std::string &text) {
  std::vector<uint8_t> md;

  std::stringstream input(text);

  // Hash file contents
  SHA512_CTX ctx;
  SHA512_Init(&ctx);

  // Reading...
  char buff[BUFF_SIZE];
  while (!input.eof()) {
    input.read(buff, BUFF_SIZE);
    size_t buff_size = input.gcount();
    SHA512_Update(&ctx, buff, buff_size);
  }

  // Get md buffer.
  md.resize(SHA512_DIGEST_LENGTH);
  SHA512_Final(md.data(), &ctx);

  return std::make_tuple(md, true);
}


std::tuple<std::vector<uint8_t>, bool> Signing(std::shared_ptr<ecdsa::Key> pkey, const std::string &path) {
  std::vector<uint8_t> signature;
  std::vector<uint8_t> md;
  bool succ;
  std::tie(md, succ) = HashText(path);
  if (!succ) {
    return std::make_tuple(signature, false);
  }
  std::tie(signature, succ) = pkey->Sign(md);
  if (!succ) {
    std::cerr << "Cannot signing file!" << std::endl;
    return std::make_tuple(signature, false);
  }

  return std::make_tuple(signature, true);
}


bool Verifying(const ecdsa::PubKey &pub_key, const std::string &path, const std::vector<uint8_t> &signature) {
  std::vector<uint8_t> md;
  bool succ;
  std::tie(md, succ) = HashText(path);
  if (succ) {
    return pub_key.Verify(md, signature);
  }
  return false;
}


std::string BinaryToHexString(const unsigned char *bin_data, size_t size, bool is_little_endian) {
  std::stringstream ss_hex;
  if (is_little_endian) {
    // Little-endian
    for (int i = size - 1; i >= 0; --i) {
      ss_hex << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(bin_data[i]);
    }
  } else {
    // Normal binary buffer.
    for (int i = 0; i < size; ++i) {
      ss_hex << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(bin_data[i]);
    }
  }
  return ss_hex.str();
}

std::vector<uint8_t> ripemd160(const uint8_t *data, int len) {
  // Prepare output data
  std::vector<uint8_t> md;
  md.resize(RIPEMD160_DIGEST_LENGTH);

  // Calculate RIPEMD160
  RIPEMD160_CTX ctx;
  RIPEMD160_Init(&ctx);
  RIPEMD160_Update(&ctx, data, len);
  RIPEMD160_Final(md.data(), &ctx);

  return md;
}

std::vector<uint8_t> sha256(const uint8_t *data, int len) {
    // Prepare output data
    std::vector<uint8_t> md;
    md.resize(SHA256_DIGEST_LENGTH);

    // Calculate SHA256
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, data, len);
    SHA256_Final(md.data(), &ctx);

    // Returns
    return md;
}

std::string FromPublicKey(const std::vector<uint8_t> &pub_key,unsigned char prefix_char,unsigned char *out_hash160){
    // 1. SHA256
  auto result = sha256(pub_key.data(), pub_key.size());

  // 2. RIPEMD160
  result = ripemd160(result.data(), result.size());

  // 3. Add 0x00 on front
  std::vector<uint8_t> temp;
  temp.resize(result.size() + 1);
  temp[0] = 0x00; //prefix_char准确来说是'\0', 这个地方记得初始化，不然编译器抽风会随机初始化成其它字节导致生成的地址不以1开头
  std::memcpy(temp.data() + 1, result.data(), result.size());
  //temp: 0 + ripemd160(sha256(data))
  result = temp;

  // 4. SHA256 twice
  // result: doublesha(0 + ripemd160(sha256(data)))
  result = sha256(result.data(), result.size());
  result = sha256(result.data(), result.size());

  // 5. Take first 4 bytes only and add to temp
  std::vector<uint8_t> long_result;
  long_result.resize(temp.size() + 4);
  memcpy(long_result.data(), temp.data(), temp.size());
  //0 + ripemd160(sha256(data)) +  (前四个字节)doublesha(0 + ripemd160(sha256(data)))
  memcpy(long_result.data() + temp.size(), result.data(), 4);

  // Copying hash160 if required.
  if (out_hash160) {
      memcpy(out_hash160, long_result.data() + 1, 20);
  }
  // 6. Base58
  std::string addr = base58::EncodeBase58(long_result);

  // Returns address string
  return addr;
}

