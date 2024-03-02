#include "../core/user.hpp"
#include "../core/crypto.hpp"
#include "../core/transaction.hpp"
#include "../core/merkle_tree.hpp"
#include <iostream>
using namespace std;

TEST(single_node_works) {
    MerkleTree m;
    User miner;
    Transaction a = miner.mine();
    vector<Transaction> items;
    items.push_back(a);
    m.setItems(items);
    shared_ptr<HashTree> proof = m.getMerkleProof(a);
    ASSERT_TRUE(proof->left->hash == a.getHash());
    ASSERT_TRUE(proof->right->hash == a.getHash() );
    SHA256Hash ha = a.getHash();
    ASSERT_TRUE(proof->hash == concatHashes(ha,ha));
}

bool checkProofRecursive(shared_ptr<HashTree> root) {
    if (!root->left && !root->right) {
        // fringe node
        return true;
    } else {
        if (concatHashes(root->left->hash, root->right->hash) != root->hash) return false;
        return checkProofRecursive(root->left) && checkProofRecursive(root->right);
    }
}

TEST(single_three_nodes_works) {
    MerkleTree m;
    User miner;
    User receiver;
    Transaction a = miner.mine();
    Transaction b = miner.send(receiver,50);
    Transaction c = miner.send(receiver,50);
    vector<Transaction> items;
    items.push_back(a);
    items.push_back(b);
    items.push_back(c);
    m.setItems(items);
    shared_ptr<HashTree> proof = m.getMerkleProof(a);
    
    ASSERT_TRUE(concatHashes(proof->left->hash, proof->right->hash) == proof->hash);
    ASSERT_TRUE(checkProofRecursive(proof));
}

TEST(larger_tree_works) {
    MerkleTree m;
    User miner;
    User receiver;

    vector<Transaction> items;
    for(int i =0; i < 4000; i++) {
        items.push_back(miner.send(receiver,i));
    }
    m.setItems(items);
    shared_ptr<HashTree> proof = m.getMerkleProof(items[4]);
    ASSERT_TRUE(checkProofRecursive(proof));
}
