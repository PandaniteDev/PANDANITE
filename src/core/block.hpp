#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <map>
#include "transaction.hpp"
#include "constants.hpp"
#include "common.hpp"
#include "crypto.hpp"
#include "helpers.hpp"
#include "merkle_tree.hpp"

struct BlockHeader {
    uint32_t id;
    uint64_t timestamp;
    uint32_t difficulty;
    uint32_t numTransactions;
    SHA256Hash lastBlockHash;
    SHA256Hash merkleRoot;
    SHA256Hash nonce;

    BlockHeader(uint32_t _id, uint64_t _timestamp, uint32_t _difficulty,
                uint32_t _numTransactions, SHA256Hash _lastBlockHash,
                SHA256Hash _merkleRoot, SHA256Hash _nonce)
        : id(_id), timestamp(_timestamp), difficulty(_difficulty),
          numTransactions(_numTransactions), lastBlockHash(_lastBlockHash),
          merkleRoot(_merkleRoot), nonce(_nonce) {}
};

class Block {
public:
    Block();
    Block(json data);
    Block(const Block& b);
    Block(const BlockHeader& b, std::vector<Transaction>& transactions);
    BlockHeader serialize();
    json toJson();
    void addTransaction(Transaction t);
    void setNonce(SHA256Hash s);
    void setMerkleRoot(SHA256Hash s);
    void setTimestamp(uint64_t t);
    void setId(uint32_t id);
    void setDifficulty(uint8_t d);
    SHA256Hash getHash() const;
    SHA256Hash getNonce() const;
    SHA256Hash getMerkleRoot() const;
    SHA256Hash getLastBlockHash() const;
    void setLastBlockHash(SHA256Hash hash);
    uint64_t getTimestamp() const;
    uint32_t getDifficulty() const;
    const std::vector<Transaction>& getTransactions() const;
    std::vector<Transaction>& getTransactions();
    uint32_t getId() const;
    bool verifyNonce();

private:
    uint32_t id;
    uint64_t timestamp;
    uint32_t difficulty;
    std::vector<Transaction> transactions;
    SHA256Hash merkleRoot;
    SHA256Hash lastBlockHash;
    SHA256Hash nonce;

    static const int BLOCKHEADER_BUFFER_SIZE = 116;
};

bool operator==(const Block& a, const Block& b);

#endif // BLOCK_HPP
