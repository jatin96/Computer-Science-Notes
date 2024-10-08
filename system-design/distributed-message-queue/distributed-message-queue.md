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
- When a consumer subscribes to a topic, it pulls data from one or more of these partitions.
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

### Data storage

### Message data structure

### Message key

### Message value

### Other fields of message

### Batching

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

