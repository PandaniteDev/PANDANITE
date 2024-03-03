class MemPool {
public:
    static constexpr int TX_BRANCH_FACTOR = 10;
    static constexpr int MIN_FEE_TO_ENTER_MEMPOOL = 1;

    MemPool(HostManager& h, BlockChain& b);

    ~MemPool();

    void mempool_sync();

    void sync();

    bool hasTransaction(const Transaction& t);

    ExecutionStatus addTransaction(const Transaction& t);

    size_t size() const;

    std::vector<Transaction> getTransactions() const;

    std::pair<char*, size_t> getRaw() const;

    void finishBlock(Block& block);

private:
    HostManager& hosts;
    BlockChain& blockchain;
    bool shutdown;

    mutable std::mutex mempool_mutex;
    mutable std::mutex toSend_mutex;

    std::set<Transaction> transactionQueue;
    std::map<std::string, TransactionAmount> mempoolOutgoing;
    std::vector<Transaction> toSend;
    std::vector<std::thread> syncThread;
};
 
