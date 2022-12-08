# Blockchain Expert

# Fundamentals of Blockchain

## Advantages of Blockchain?

- Blockchain helps you create decentralized systems. 
- In a centralized system, you trust the owner of the software to not miss use your information. The owner can be Amazon, Google or the government.
- Decentralized system aims at eliminating this dependency. In such a sytem, no one can control the system and the trust is ensured by software.

## Web 1.0 vs Web 2.0 vs Web 3.0

### Web 1.0
- Mostly readonly content.
- No focus on UI/UX. It's literally like someone has just dumped a book online.

### Web 2.0
- Focuses on user generated content.
- Large focus on UI / UX.
- Uses AJAX and javascript for dynamic websites.
- User data is owned and controlled by companies.

### Web 3.0
- Focuses on how is data stored and controlled.
- Data is not controlled by a party but owned by the network as a whole.
- Immersion over interaction.

## Centralized vs Non centralized Systems
- Most systems we use today are centralized.
- We use centralized system because we trust that system.

## Advantages and Disadvantages

![centralized vs non centralized systems](2022-12-04-11-37-06.png)

## Ledgers

- Suppose Alice sends Bob $1.
- Who will ensure the trust in this transaction?
- Alice and Bob can do it through a bank and now both Alice and Bob would trust the bank for the authenticity of this transaction
- This trusted transaction is called a ledger. 
- In a decentralized system, this ledger is with different machines. So you trust the network not a single entity.

## Wallets

- Wallets hold your cryptocurrency. These essentially contain the following things
    - Private Key - You cannot lose your private key at any cost. Cannot be recovered.
    - Public Key - Needed so that people can send you funds. Public key is generated using the private key and a hash function.
    - Address - Public key is passed through hash function to generate address. We do this so that we can generate a readable address which we can easily use. Address is shorter than the public key hence it is easy to deal with. It also adds an extra layer of security.
- Private Keys are not stored in plain text. They are stored in encrpyted format. You would have a password which you would use to encrpyt the private key using an encryption algorithm, and it will spit out some random string which means nothing to anyone.
- You can have multiple address to give to different people. This helps you to organize your funds.

## Hash Functions

- A hash function is a function whose inverse doesn't exist.
- Hash functions are used to generate the public key from the private key,
- y = f(x) ; x is the private key, y is the public key.
- The security of the internet is based on the properties of hash function.
- you can still use brute force to guess the private key from the public key and hash function. This is very difficult to do.
- Hash functions are very fast to compute.
- They generate outputs which are uniform. eg. having the same length.
- Hash functions are deterministic i.e. same output for same input. This property is used to prove the owner of a wallet. The private key is passed throught the hash function. If the same public key is generated then that person is the owner of the wallet.
- Hash functions cannot have hash collisions practically.
- The output of a hash function is called digest.

## Seed Phrases

- Seed phrases help you regenerate your private key in case you lose it.
- All the wallets provide you seed phrases to do private key recovery.

## Transactions

- Transactions allow you to exchange funds over the network
- A transaction consist of the following:
    - From: sender
    - To: recipient
    - Amount: amount of funds
    - Gas/Fee: Amount you need to pay to the miners to add your transaction to the blockchain
    - signature: the digital signature which proves the authenticity 
    - Tx Hash: transaction hash unique to every transaciton
    - nonce: number of transactions from that account. When you make your first transaction nonce: 1 then 2,3,4...
    - date: auto-populated
- Transaction pool: Where the transactions go before being officially added to the blockchain. Miners pick up transactions from the transaction pool and then add them to the blockchain.
- Inputs to the transaction
- Outputs to the transaction
    - To : address of recipient
    - Amount: amount to be sent
    - Fee/Gas: More gas, more priority to your transaction to be picked up by miners. There is a min. gas fee.
- To pay the amount of the transaction, you need to feed more transactions to your transaction which sum upto atleast the amount + gas fee. This would prove that you have let's say 6 bitcoins which you are paying to someone else.

![transactions in bitcoin](2022-12-04-14-32-28.png)
- If your transactions > amount + gas, the remaining amount is sent back as change to your wallet.
- This keeps an entire history of transactions or log on the blockchain.
- **Transaction hash** - The message (data for transaction) is passed through a hash function to generate the transaction hash. A transaction hash is tightly associated with the original message. Any change in message would make this transaction hash invalid.
- **Signing key**: this is your private key used to sign the transaction hash
- **Verification key**: this is the public key used to decrypt the signature and verify the transaction hash.
- **Verification process** - Miners verify your transaction using the following process
    1. They use your public key to decrpyt the signature and get the original transaction hash
    2. They then use the hash function to generate the transaction hash on their own.
    3. If both the hashes are equal, it means the signature is valid.
    4. the transaction is then added to the blockchain.

## Blocks

- Block data contains the following 
    - Magic number : A constant value to identify the block
    - Block size: The amount of data which a block can hold
    - Block header: Contains metadata
    - Transaction count: Number of transactions stored on the block
    - Transaction data: actual transaction data
    - Version: version of blockchain network
    - previous block hash : hash of previous block. Helps to connect the chain.
    - hash merkle root: merkle tree is used to check if a transaction is part of a block or not. Used for validation
    - timestamp
    - bits/difficulty: More bits, more difficulty to mine the block
    - nonce: Proves that a certain block is valid and some work has been done to prove it.
    - block reward: how much bitcoin to pay the miners

> The first block in the bitcoin network is called the genesis block

- Genesis block defines the difficulty and difficulty interval
- mining rewards and reward changes
- circulating supply eg 21 million bitcoins

## Blockchain Security

- Blockchain should prevent someone from deleting a transaction from a block
- Blockcain should prevent somene from doing the same transaction twice with the same inputs. Since transactions in a bitcoin blockchain are submitted randomly, if someone submits two transactions with the same input, one of them would be rejected(when it will see that a transaction with similar input is already present in the blockchain).
- exploited code : code vulnerabilities could also cause a problem to the blockchain.
- **51% attack** : If the blockchain network is small, a single individual can get access to 51% resources and get consensus. He can then vote a wrong transaction to be accepted.

## Proof of Work

- Miners take transactions out of their transaction pool and try to do some proof of work on it.
- They first check whether the transaction is valid or not by looking at its signature.
- They also look at previous block to check whether there is no double spending or not.
- Lets say the difficulty or bits is 20. Now, for a random hash to start with 20 zeros is very difficult. This is where nonce comes into picture.
- The miners try to guess this nonce such that the hash finally starts with 20 zeros. This is the proof of work.
- All the miners store their own version of blockchain.

### Scenario 1
- Let's say there are 3 miners who have their own up-to-date copy of blockchain network.
- Miner1 verifies a transaction and is able to find the once.
- He then submits this to the blockchain.
- Other miners are listing, they see that the block submitted by miner1 is valid, and they update their local copy of blockchain.
- If they were working on the same block, they stop their work and again try to pick a new transaction on the blockchain.

### Scenario 2
- If the block submitted by miner1 was invalid, the other miners continue to guess the nonce on the same transaction until someone finds the correct one.

> This is a race for the miners. Whoever has better compute power has better change of guessing the once - hence better change to submit a block to the block chain.

### Scenario 3
- Two miners submit the same block at the same time. 
- In this case we create fork chains.
- When some new miners pull the block chain, some will receive one block chain some will receive another fork chain

![fork chains](2022-12-09-01-41-33.png)
- How will the network again resolve it into a flat chain?
- This will take place when a miner submits a new block. If the miner submits a new block and it gets added to the green chain, it becomes longer.
- The longer chain is then declared to be the valid blockchain, and all the miners who got a copy of yellow block chain have to pull again.

![longest chain is valid](2022-12-09-01-44-26.png)

### What do we mean by longest chain ? 
- Longest chain in a bitcoin network is a chain which has all the valid blocks with highest cumulative difficulty/bit.
- Longest chain decided not by length but by cumulative difficulty of mining.

### Confirmation

- number of blocks that has been added since your latest processing block.
- This gives you an idea what is the probability that your block will be rejected.

### Bits/difficulty

- Bitcoin network changes the bits every 2 weeks.
- Bitcoin network wants to have block addition time as 10 minutes.
- If you have more miners, you have more compute power so this time would be less.
- Then you have to increase the bits so that time is close to 10 minutes.
- If the miners are less, the difficulty is reduced so that again, the time taken is 10 mins.


### Your chances of mining a block

- Let's say you have a single graphics card. 
- The chances that you would be able to guess a nonce in 10 minutes before someone with a minin farm is less.
- What can you do?
- You can take part in Pool mining. 
- A large pool of miners mine blocks together and share the reward based on the hash power someone was contributing.
- One disadvantage of pool mining is that this reduces the decentralization of the network because now a large entity has a lot of power in the network.

### More about Proof of Work
- Sometimes a miner can also create an empty block and add it to the blockchain.
- Let's say the miner has its transaction pool and is trying to add transaction to the block.
- In the meantime, the miner also tries to verify if there was already a block submitted which had similar transactions which it has in its transaction pool.
- It then removes these transactions from the pool.
- If any of these repeated transaction enters his block, his block would be invalid.
- But he can always create an empty block because he knows the previous block hash. He can add that empty block to the network too!
- miners block on empty blocks when they are verifying their transaction pool for duplicates, so that they don't waste time and resources.
- Probability of finding the nonce for empty block == probability to find nonce for a non-empty block
- Empty blocks are also useful because they increase the security of chain by increasing its length.

### Environmental Impact
- There is a huge environment impact at a given time because a lot of miners do redundant work and waste resources.

### How to hack Proof of Work
- People try to get 51% network and try to create the longest chain with invalid blocks.
- They can do it because they have enough miners to it.
- Basically, you need to race against the network to continue this fradulant blockchain.




     

 











