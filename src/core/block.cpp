#include "block.hpp"
#include "helpers.hpp"
#include "crypto.hpp"
#include "openssl/sha.h"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <cstring>
#include <algorithm>
using namespace std;

// ... (Include other necessary headers)

// BlockHeader functions
BlockHeader blockHeaderFromBuffer(const char* buffer) {
    BlockHeader b;
    b.id = readNetworkUint32(buffer);
    b.timestamp = readNetworkUint64(buffer);
    b.difficulty = readNetworkUint32(buffer);
    b.numTransactions = readNetworkUint32(buffer);
    b.lastBlockHash = readNetworkSHA256(buffer);
    b.merkleRoot = readNetworkSHA256(buffer);
    b.nonce = readNetworkSHA256(buffer);
    return b;
}

void blockHeaderToBuffer(BlockHeader& b, char* buffer) {
    writeNetworkUint32(buffer, b.id);
    writeNetworkUint64(buffer, b.timestamp);
    writeNetworkUint32(buffer, b.difficulty);
    writeNetworkUint32(buffer, b.numTransactions);
    writeNetworkSHA256(buffer, b.lastBlockHash);
    writeNetworkSHA256(buffer, b.merkleRoot);
    writeNetworkSHA256(buffer, b.nonce);
}

// Block class
Block::Block() : nonce(NULL_SHA256_HASH), id(1), timestamp(getCurrentTime()), difficulty(MIN_DIFFICULTY),
    merkleRoot(NULL_SHA256_HASH), lastBlockHash(NULL_SHA256_HASH) {
    // Other constructor logic if needed
}

Block::Block(const Block& b) : nonce(b.nonce), id(b.id), difficulty(b.difficulty),
    timestamp(b.timestamp), merkleRoot(b.merkleRoot), lastBlockHash(b.lastBlockHash) {
    this->transactions = b.transactions;
}

Block::Block(json block) : nonce(stringToSHA256(block["nonce"])), merkleRoot(stringToSHA256(block["merkleRoot"])),
    lastBlockHash(stringToSHA256(block["lastBlockHash"])), id(block["id"]), difficulty(block["difficulty"]),
    timestamp(stringToUint64(block["timestamp"])) {
    for (const auto& t : block["transactions"]) {
        Transaction curr = Transaction(t);
        this->transactions.push_back(curr);
    }
}

Block::Block(const BlockHeader& b, vector<Transaction>& transactions) : id(b.id), timestamp(b.timestamp),
    difficulty(b.difficulty), nonce(b.nonce), merkleRoot(b.merkleRoot), lastBlockHash(b.lastBlockHash) {
    for (const auto& t : transactions) {
        this->addTransaction(t);
    }
}

BlockHeader Block::serialize() {
    BlockHeader b;
    b.id = this->id;
    b.timestamp = this->timestamp;
    b.difficulty = this->difficulty;
    b.numTransactions = this->transactions.size();
    b.nonce = this->nonce;
    b.merkleRoot = this->merkleRoot;
    b.lastBlockHash = this->lastBlockHash;
    return b;
}

json Block::toJson() {
    json result;
    result["id"] = this->id;
    result["hash"] = SHA256toString(this->getHash());
    result["difficulty"] = this->difficulty;
    result["nonce"] = SHA256toString(this->nonce);
    result["timestamp"] = uint64ToString(this->timestamp);
    result["transactions"] = json::array();
    result["merkleRoot"] = SHA256toString(this->merkleRoot);
    result["lastBlockHash"] = SHA256toString(this->lastBlockHash);

    for (const auto& t : this->transactions) {
        result["transactions"].push_back(t.toJson());
    }
    return result;
}

// ... (Other member functions)

bool operator==(const Block& a, const Block& b) {
    return a.id == b.id && a.nonce == b.nonce && a.timestamp == b.timestamp &&
           a.lastBlockHash == b.lastBlockHash && a.difficulty == b.difficulty &&
           a.merkleRoot == b.merkleRoot && std::equal(a.transactions.begin(), a.transactions.end(), b.transactions.begin());
}

// ... (Other functions)
