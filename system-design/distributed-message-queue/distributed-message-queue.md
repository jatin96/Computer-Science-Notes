# Distributed Message Queue

## Functional Requirements

- Producers can send messages to queue
- Consumers can consume messages from queue
- Delivery semantics
  - At least once
  - Exactly once
  - At most once
- history data can be truncated
- message size is in kb
- Ability to deliver the message in order they were added in the queue

## Non functional requirements

- High throughput or low latency which can be configured
- Scalable
- Persistent and durable: Data to be stored on disk and replicated to multiple nodes

## High level design

> Note: Traditional messaging queue are event streaming platforms which only maintain messages in memory long enough for them to be consumed. Disk write happens only in case of overflow. Messaging ordering is also not guaranteed.

### Messaging models

### Point 2 point

- One message can be consumed by only one consumer
- Once the message is consumed and ack'ed by the consumer, the message is removed from the queue
- Message is maintained in-memory only till the message is consumed

### Publish subscribe

- There is a concept of topics which are categories to hold messages.
- Subscribers subscribe to topics
- One message can be consumed by many subscribers
- We can also support point to point model using the concept of consumer groups

### Topics, partitions, and brokers

- Topics are divided into parititons for scalability
- Paritions are distributed among a set of servers called brokers
- Each partition behaves like a FIFO queue
- Position of a message in partition is called offset
- Message is sent to the topic, it is actually sent to a parition based on the message key
- When a consumer subscribes to a topic, it pu  lls data from one or more of these partitions.
- When there are multiple consumers subscribing to a topic, each is responsible for a subset of partitions. This is called consumer group.

### Consumer group

- A consumer group is a conceptual group of consumer which subscribe to some topics.
- For example, a consumer group for billing messages.
- A consumer group can consume messages from many topics
- Each consumer group maitains its own offset which tells to what message number the message has been consumed
- Therefore a single message can be consumed by multiple consumers since two consumers belonging to different consumer groups will have different offset
- consumers in a group can consume messages in parallel. This can cause problems like ordered consumption of messages cannot be guaranteed.
- We can add a constraint that a parition will only have 1 consumer in that group which will ensure that all messages are consumed by one consumer and hence they are ordered
- The above makes it point to point model
- If number of consumers are larger than paritions in a group, some consumers will sit idle

### High level architecture

## Design deep dive

- Design focuses on batching. Producers batch the request, brokers stored the messages in batches and consumers consume the messages in batches.
- We want high throughput with persistence
- Message are transferred from producers to consumers with no modification to avoid copying small data

### Data storage

- System is read-write heavy
- no update or delete operations
- sequential data access

Choices for persistence

1. Database: It is difficult to find a database which is write and read heavy at scale
2. Write ahead log: WAL is file which only allows appending lines to the end of the file. WAL have purely sequential access so the disk performance is really good.

### Segmentation

- WAL cannot be one huge file so we need segmentation
- Only the active segment allows write request
- Inactive segments serve read requests
- If a server is inactive for a long time, we can truncate the segment to clean data
- segments of a partition are arranged inside folders

> Note: Sequential access of data makes read-write quite fast even on disk. Disk is usually slow when we do random access

### Message data structure

The idea here is to avoid copying of data becaus at our scale it can be costly. The message has the following fields:

- key byte[]
- value byte[]
- timestamp long
- size long
- topic string
- partition int
- offset long

### Message key

- Messaging key is used to find the partition where the message should go based on consitent hashing
- If key is not given, a partition is randomly choosen
- User can define its own hashing algorithm as well for even distribution
- Hashing algorithm will distribute messages evenly even if we increase the partitions
- keys might not be unique

### Message value

- this is the actual payload of the message

### Other fields of message

- topic, partition, offset
- We can find a message using topic -> partition -> offset

### Batching

- Batching is very imporant in this system because of the scale
- We can batch messages on producers and send multiple messages in one batch to save network io
- We can batch messages while writing to WAL where we can write many logs in chunk. 
- This will increase throughput
- There is a tradeoff between latency and throughtput. If we want to improve latency, we need to reduce the batch sizes which means we might need more paritions since more requests would come in

### Producer flow

### Consumer flow

### Push vs pull

#### Push model


#### Pull model

### State storage

### Metadata storage

### Zookeeper

### Replication

### In-sync replica

### Scalability

#### Producer

#### Consumer

#### Broker

#### Partition

### Data delivery semantics

#### At most once

#### At least once

#### Exactly once

## Advanced Features

