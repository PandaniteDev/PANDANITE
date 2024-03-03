#include "data_store.hpp"
#include <thread>
#include <memory>
#include <filesystem>

DataStore::DataStore() : db(nullptr) {}

void DataStore::closeDB() {
    db.reset();  // Use smart pointer to automatically manage memory.
}

std::string DataStore::getPath() const {
    return path;
}

void DataStore::clear() {
    std::unique_ptr<leveldb::Iterator> it(db->NewIterator(leveldb::ReadOptions()));
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        std::string key = it->key().ToString();
        leveldb::Status status = db->Delete(leveldb::WriteOptions(), key);
        if (!status.ok()) throw std::runtime_error("Could not clear data store : " + status.ToString());
    }
    assert(it->status().ok());
}

void DataStore::deleteDB() {
    leveldb::Options options;
    leveldb::Status status = leveldb::DestroyDB(path, options);
    std::filesystem::remove_all(path);  // Use std::filesystem consistently.
    if (!status.ok()) throw std::runtime_error("Could not close DataStore db : " + status.ToString());
}

void DataStore::init(std::string path) {
    closeDB();  // Ensure previous resources are released.
    this->path = std::move(path);  // Use move semantics for efficiency.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, this->path, &db);
    if (!status.ok()) throw std::runtime_error("Could not write DataStore db : " + status.ToString());
}
