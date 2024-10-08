# Stock Exchange System

## Functional Requirements

1. The ability to buy and sell stocks during trading hours only.
2. Wallet management - Witholding of stocks when the order is pending, enough balance in the wallet to execute trades
3. Risk checks - A user should not be able to place more than 1 million orders per share per day.
4. Support for trading at least 100 stocks.

## Non-functional Requirements

1. Availability: Atleast 4 nines of availability(99.99%) because down time is costly for exchanges.
2. Fault tolerance: Fault tolerance is needed for fast recovery to limit the impact of an incident.
3. latency: Should be millisecond level. We should try to lower the 99 percentile latency. This is because if this number is high, it will be a bad user experience. Latency is measured from the time order is placed by the order to the time the order is filled and reported back to the user.
4. Security:
    a. KYC of customers
    b. Prevention of DDoS attack on public facing web pages like price graphs etc.

## Back-of-envelope Calculations

1. 1 billion orders per day: 1,000,000,000
2. Stock market operation hours: 6.5 hour per weekday
3. Total time in seconds: 6.5 * 3600
4. QPS = 1,000,000,000/(6.5 * 3600) = 43,000

Peak QPS = 5 * QPS = 215,000

## High Level Design

Some business knowledge:

1. Most users trade on stock exchange via brokers. These brokers provide a user friendly interface.
2. Institutional clients are the once who trade in big volumes. They need special software to do it. For example, hedge funds need softwares which support very low latency as compared to normal users.
3. Limit order is a buy or sell order with a fixed price. When price matches, it gets executed.
4. Market order: It excecutes at market value of the price, immediately.
5. FIX protocol: This protocol is used for trading information transfer over the network.

![stock exchange high level design](image.png)

There are three flows:

1. Trading flow: Critical path which takes care of buying and selling of stocks.
2. Reporting flow: The flow which persists the data for reporting and risk management
3. Market data flow: Collects the orders and executions and publishes it to the market

### Critical Path or Trading flow

1. User places the order from the broker
2. The broker send the order to stock exchange's component gateway.
3. Component gateway does basic validations, rate limiting, authentication. It then forwards the order to order manager
4. Order manager performs risk checks as set by the exchange
5. After successful risk checks, order manager checks that there are sufficient funds in wallet.
6. After that order is sent to the matching engine, where it matches the similar sell request, it generates two orders one for buy and one for sell.
7. The executions are sent back to the client

### Market data flow

1. The matching engine publishes a stream of executions to the market data publisher.
2. Market data publisher creates order books and charts.
3. It then sends the market data to the data service.
4. Data service stores the data in specialized storage for real-time analytics. Brokers connect to the data service.
5. Brokers relay the data back to the clients.

### Reporting flow

1. Collects the data like order number, client_id etc. and persists into the database.

### Matching Engine

Matching engine performs the following functions:

1. Matching of buy and sell orders. Matching function must be fast and accurate
2. Send market data to market data publisher
3. Construction of order book.

The Matching engine must be **highly available** and **deterministic**. That is, the same sequence of orders must produce the same executions.

### Sequencer

This is what makes the matching engine deterministic. It's main job is to attach a sequence id to every order.

1. Sequencer has two instances: inbound sequencer and outbound sequencer.
2. Inbound sequencer adds squence ids to each order coming and then sends it to the matching engine.
3. Outbound sequence adds sequence ids to each execution and sends it back to the order manager.
4. This is done to detect any missing orders/executions.
5. Matching engine is connected to these two sequencers, one is used for receiving orders and other for publishing executions backt to order manager.
6. Sequencer also acts like a messaging queue. Kafka could be used but its latency is very low and  is not predictable.

### Order manager

1. Order manager manages an order's state.
2. It also executes the risk checks.
3. It verifies the orders against the wallet's funds.
4. It sends the orders to the sequencer and receives the executions from the matching engine through the sequencer.
5. Some non essential attributes are removed from the order before sending it to the matching engine.

### Client gateway

1. Client gateway does authentication, rate limiting, validations etc.
2. There will be **different** client gateways for different type of users. Intitutional clients will have very low latency client gateway.
3. Client gateway lies on the critical path so it should be low latency. It is a tradeoff to decide what to include in the client gateway and what to leave.
4. Colocation engine is a trading software running inside the exchange data center, which is the lowest latency possible. Brokers/dealers use these colocation engines.

## Market data publisher

1. The market data publiser creates the order books and the candlestick charts from the executions received.
2. This data is sent to the data service to which the clients subscribe to get the data.

## Reporting

1. Provide auditing, history, tax reporting, compliance etc.

## API Design

The following APIs would be needed:

- POST /v1/order
  - Parameters: symbol, side, quantity, price
  - Response: id, creationTime, filledqty, remainingQty, status(new/cancelled/filled)
- GET /v1/execution?symbol={:symbol}&orderId={:orderId}&startTime={:startTime}&endTime={:endTime}
  - Parameters: symbol, orderId(optional), startTime, endTime
  - Response: Array of executions(id, orderId, symbol, quanty, price, type, side)
- GET /v1/orderbook/symbol={:symbol}&depth={:depth}
  - Response: bids and asks
- GET /v1/candles?symbols={:symbols}&startTime={:startTIme}&endTime={:endTime}

## Data modoels

Three main types of data:

- Product, order and execution
- Order book
- Candlestick chart

### Product

Product is the actual stock. For example, HDFC is one product. It can have the following attributes:

1. Symbol
2. Name
3. description
4. lotSize
5. productId

### Order

Order is an instruction to buy or sell a stock at the best available price. It ensures that the stock buy/sell will be executed but doesn't gaurantee a specific price. An order can have the following attributes

1. orderId
2. productId
3. price
4. quantity
5. status
6. type
7. userId
8. transactionTime

### Execution

Execution of an order is the actual fulfillment of the order placed by the client. It can have the following attributes:

1. execId
2. orderId
3. price
4. quantity
5. type
6. status
7. userId
8. transactionTime

![order execution product](2024-08-15-23-26-46.png)

Points:

1. Orders and execution int the critical path are not stored in DB in order to achieve high performance. Trades are executed in memory. Orders and executions are archived to disk after the day closes.
2. Reporter writes orders and executions to DB for reporting.

### Orderbook

How orders are executed? Watch this: https://www.youtube.com/watch?v=Kl4-VJ2K8Ik

> Note: Exchange  tries to give the best price to the customer. It means if the customer is buying at the market, it will try to give the cheapest price possible. If the customer is selling, it will give the max price possible from the trades in order book.

Notes:

1. Buys are sorted in decreasing order because when you sell you want the highest price
2. Sells are sorted in increasing order because when you buy you want the lowest price

An efficient data structure for order book must satisfy:

1. constant lookup time: We would need to find out how many shares are available at a particular price level.
2. Fast add/cancel/execute operations at O(1).
3. Query best BID/ASK.
4. Iterate through price levels.

The following data structure would work well:

```java
class PriceLevel {
    private Price limitPrice;
    private long volume;
    private List<Order> orders; // Doubly linked list
}
class Book<Side> {
    private Side side;
    private Map<Price, PriceLevel> limitMap;
}
class OrderBook {
    private Book<Sell> sellBook;
    private Book<Buy> buyBook;
    private Map<OrderId, Order> orderMap;
}
```

- Adding new order means adding the order to the tail of doubly linked list which is O(1)
- Executing new order means deleting order from head of the list which is O(1);
- Cancelling order means we will take the pointer value form orderMap against the orderID and use that to delete it from the respective doubly linked list.

### Candlestick

```java
class CandleStick {
    private long openPrice;
    private long closePrice;
    private long lowPrice;
    private long highPrice;
    private long volume;
    private long timestamp;
    private int interval;
}

class CandlestickChart {
    private LinkedList<Candlestick> sticks;
}
```

## Design Deep Dive

### Performance

- Latency should be very low for stock exchange.
- Latency can be reduced along the critical path in two ways:
  - Reduce the number of tasks in critical path.
  - Reduce the time taken by each task in critical path.

Reducing the number of tasks in critical path: We will only have the essential components needed for a trade to execute:

- gateway
- order manager
- sequencer
- matching engine

Reducing the time taken by each component in critical path:

- Network request/response for all compoents would take single digit millisecond latency.
- Disk IO by sequencer would take tens of milliseconds of latency.
- This number is high for low latency trading systems.
- Exchanges want to achieve latency in order of tens of microseconds by removing network and disk IO as much as possible.
- This can be done by putting all the components on 1 server. This enables all the components to talk via mmap as as event store.
- Each component runs as the process on the single server.
- Tasks are executed by the application loop which polls for tasks in a while loop.
- Each application loop is single threaded and pinned to the CPU core
  - Pinning means no context switch as the CPU is always available.
  - No lock contention since there is only 1 thread that updates the states.
- mmap is a way for processes to share memory in performant manner, it helps avoid disk access and brings it down to 0. mmap acts like a messaging bus.

![singer server design](image-1.png)

### Event Sourcing

Event sourcing is a way to manage states of an application such that it allows us to regenerate the same output if we replay the states. In this approach, we store an immutable log of events in order of the timestamp in which they executed along with the event status.
Event sourcing allows us to regenerate the same state of the system in case of failures because we have stored each and every event in the database. We can just replay those events to generate the state of the system before the failure. It is also useful for auditing and compliance since a stock exchange needs to store each and every information.

Sequencer is the component that reads the newOrderEvent from the ring buffer(which gets it from the gateway) and adds a sequence number to it and forwards it to the mmap/eventStore.
We can have secondary sequencers as well for high availability.

![event sourcing system design](image-2.png)

![sequencer](image-3.png)

In the above design, order manager has been made a library and every service can maintain their own order states. This is done because having a central order manager would hurt latency. Each service can maintain their own order states because of event sourcing and they should be identifical

### High availability

- We would need atleast 4 nines of availability.
- We identify single points of failure and fix them. For example, we can have a passive matching engine.
- Detection of failure and fail over to the backup instance should be fast.
- Stateless services like gateway can be scaled horizontally by adding more services and hence reduce single points of failure.
- Stateful services like matching engine would need the ability to copy states across replicas.
- We can have the primary hot matching engine which consumes new events and sends output to the eventStore. We can have a secondary warm matching engine which consumes the exact same events as primary but doesn't send any output. When primary goes down, secondary can replace it immediately.
- Since all of the components run inside a single server, we would need to create warm servers by replicating events across servers and machines. This is because in case the entire primary hot server goes down, we would need the warm server up quickly. This is done using reliable UDP and multicasting.

### Fault Tolerance

Let's answer some questions:

Q. There might be false alarms resulting in unnecessary failovers. Also, there might be bug in the code which will bring down backup instance as well if we fail over to it. How to tackle this?

A. We first need to perform failovers manually when releasing a new system. Failover should also be part of integration test pipeline and only when failover is successful we should push the new code to prod. We can also use Chaos engineering.

Q. How to decide which server to take over?
A. We can choose any leader election algorithm. Raft is one such algorithm.

Q. What about RTO and RPO?
A. RTO stands for recovery time object which is the time a system can be down until significant harm is done to the business
RPO stands for recovery point object which stands for the amount of data that can be lost until significant damage is done to the system.

For RTO, we would need automatic failover. For RPO, we would need to maintain multiple copies of data because RPO needs to be near zero for stock exchange. Raft will make sure that state consensus is reached and when 1 leader is down the other leader should be able to function immediately.

### Matching Algorithm

The algorithm used is FIFO matching algorithm. The order which comes fist is fulfilled first.

```java

Context handleOrder(Orderbook orderBook, OrderEvent orderEvent) {
    Order order = createOrderFromEvent(orderEvent);
    switch (msgType):
    case NEW:
        return handleNew(orderBook, order);
    case CANCEL:
        return handleCancel(orderBook, order);
    default:
        return ERROR;
}

Context handleNew(OrderBook orderBook, Order order) {
    if (order.side.equals(BUY)) {
        return match(orderBook.sellBook, order);
    } else {
        return match(orderBook.buyBook, order);
    }
}

Context match(Book<Side> sellBook, Order order) {
    List<Order> marketOrders = sellOrders.limitMap.get(order.price).orders;
    Iterator<Order> orderIter = getIterator(marketOrders);
    remainingQty = order.quantity;
    while (orderIter.hasNext() && remainingQty > 0) {
        Quantity matched = min(orderIter.next.quantity, order.quantity);
        order.matchedQuantity += matched;
        remainingQty = order.quantity - order.MatchedQuantity;
        if (order.quantity > orderIter.next.quantity) {
            remove(order.next);
        }
        generateMatchFill();
    }
}
```

### Market Data Publisher Optimizations

- Hedge funds use the Stock Exchange API to build their own candlestick charts for technical analysis.
- Market data publiser service gets matched data from matching engine and publishes the data for the subscribers.
- Ring buffer is used in MDP

![ring buffers in mdp](image-4.png)

### Distribution Fairness

- Each subscriber should get data at the same time to avoid unfair advantage.
- This can be done using multicast with reliable UDP
- We can also assign random order to the subscribers when the market opens

### State machine replication

State machine replication is a fault tolerance practice where multiple replicas are maintained for each state machine and all of them process the data in the same way to reach the same final state. When one machine goes down, we can safely replace it with another machine. The replicas agree on the final state using some consensus algorithm like Paxos or Raft.

SMR is used in for orderbook and machine engine. Here are the key components of SMR in a stock exchange:

1. State machines: This would be orderbook or matching engine
2. Consensus Algorithm: Paxos or Raft
3. Communication Layer: How replicas will talk to each other
4. Failure detection and recovery: processes to identify failures and initiate recovery.

### Multicast

Different protocls to transfer data:

1. Unicast: From one system to another.
2. Broadcast: From one system to another in the same subnetwork.
3. Multicast: From one system to another in other subnetwork.

- Multicast is used where all the receivers are put in the same multicast group.
- This ensures that all of them in theory receive the data at the same time.
- Since multicast uses UDP, some of the data might be dropped.
- There are ways of retransmission of data.

### Colocation

- Many stock exchanges offer services like putting the hedge funds server in their data center
- This reduces the network delay and hence latency in placing the orders.
- This is usually a paid VIP feature

### Network Security

Techniques to combat DDoS:

- Isolate public services and private services. Have multiple read-only copies to isolate problems.
- Use caching layer for infrequently updated data which will prevent database reads.
- Harden the URLs agains Ddos attack. For example, https://www.nse.com/data?from=134&to=156 can be used by an attacker to get different types of data. Instead, a better url would be https://www.nse.com/data/recent which gives less control to the attacker.
- Some form of allowlist/denylist is needed.
- Rate limiting

### Retransmitors in multicast

Retransmitters are used in multicast to add reliability since multicast uses UDP with no assurity of packet delivery. Retransmittors use NACK which means that the receivers sends a NACK message to the sender. The sender then finds the missing packet using the NACK and sends it back to the receivers who sent the NACK or to the entire list of receivers.

 Retransmitters maintain all the transactions in memory for the day. So, in case there is a hardware problem in a port, the client can connect to a new port and the retransmitter can replay the messages for that client on the new port so that everyone is synchronized. This is called state machine replication.

### Wrap Up

Remember the following for designing a stock exchange:

- Entire design is based on a single server or even a single process to reduce latency.
- We need very low latency.
- We need high availability and eventual consistency.
- Multicast is used to ensure distribution fairness.

## Q & A

### How are reliable UDP and mmaps used in stock exchanges?

In stock exchange system design, both mmaps (memory-mapped files) and reliable UDP play crucial roles, albeit in different areas:

Mmaps in Stock Exchange System Design:

Primarily used for high-performance data sharing and persistence.

Sharing Market Data:

Mmaps enable efficient sharing of large amounts of market data (e.g., order books, trade histories) between different components or processes within the exchange system.
Multiple processes can map the same file into their address spaces, allowing direct access to shared data without the need for explicit inter-process communication (IPC) mechanisms.
This can significantly improve performance and reduce latency, particularly in high-frequency trading environments.
Persistent Storage:

Mmaps can be used to efficiently persist critical data to disk. Changes made to the mapped memory region are automatically written back to the file, ensuring data durability.
This can be useful for storing order books, trade logs, and other data that needs to be recovered in case of system failures.

Reliable UDP in Stock Exchange System Design:

Primarily used for low-latency market data dissemination.

Market Data Distribution:
As explained previously, reliable UDP enables the efficient and scalable broadcasting of real-time market data to a large number of subscribers.
Its low latency, multicast support, and tolerance for some packet loss make it well-suited for this purpose.
Key Points on how they complement each other:

Mmaps for Internal Data Sharing: Mmaps are ideal for sharing data between different components within the exchange system itself, where high performance and low latency are critical.
Reliable UDP for External Data Distribution: Reliable UDP is used to distribute market data to external subscribers (e.g., trading firms, brokers), where scalability and tolerance for occasional packet loss are important.
Example Scenario:

An exchange system uses mmaps to share the order book between its matching engine and risk management components. This allows for fast and efficient access to the latest order information.
Simultaneously, the exchange uses reliable UDP to broadcast market data updates to subscribers, ensuring they receive timely information about trades and price changes.
Conclusion:

Both mmaps and reliable UDP are valuable tools in stock exchange system design. Mmaps enable high-performance data sharing and persistence within the system, while reliable UDP provides efficient and scalable market data distribution to external subscribers. By leveraging the strengths of both techniques, exchanges can build robust and performant systems that meet the demands of today's fast-paced financial markets.

### How do subscribers connect to a stock exchange?

Common Connection Methods

**Internet-Based Connections**

Advantages:

- Cost-effective
- Accessible from anywhere with an internet connection
- Suitable for retail investors and smaller institutions

Disadvantages:

- Higher latency compared to dedicated connections
- Susceptible to internet congestion and outages
- Security concerns due to public network exposure

**Dedicated Leased Lines**

Advantages:

- Lower latency and higher bandwidth compared to internet connections
- More reliable and secure
- Suitable for professional traders and larger institutions requiring high-performance access

Disadvantages:

- Significantly more expensive than internet connections
- Requires physical installation and infrastructure

Co-location

Advantages:

- Ultra-low latency due to physical proximity to the exchange's servers
- Access to specialized, high-speed data feeds
- Suitable for high-frequency trading firms and other latency-sensitive participants
Disadvantages:
- Most expensive option
- Limited availability due to physical space constraints
- Requires specialized hardware and infrastructure

### Can caching prevent DDoS attack?

While caching cannot prevent DDoS attack, it can help mitigate its effects:

1. Reduce server load: If we caching effectively, most of the queries won't hit the DB hence server resources won't be exhausted
2. Absorb Traffic spikes: CDNs distribute the traffic across multiple regions thus making it hard for attackers to target a single origin server.

TODOS:
9. Write how orderbook works using pen and paper.
14. How to prevent Ddos attacks.
15. Consensus algorithm/leader election are paxos or raft