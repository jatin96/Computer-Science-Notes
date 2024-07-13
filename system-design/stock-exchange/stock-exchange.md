# Stock Exchange System

## Functional Requirements

1. The ability to buy and sell stocks during trading hours only.
2. Wallet management - Witholding of stocks when the order is pending, enough balance in the wallet to execute trades
3. Risk checks - A user should not be able to place more than 1 million orders per share per day.
4. Trading should be performed for atleast 100 stocks.

## Non-functional Requirements
Non functional requirements hsould be based on the following:
1. Support for alteast 100 symbols
2. Support 1 billion trades per day

1. Availability: Atleast 4 nines of availability: 99.99%. This is because down time is costly for exchanges
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
5. FIX protocol: This protocol is used for trading information transfer over the network


**Add diagram here**

There are three flows:
1. Critical path: Placing orders
2. Reporting flow: Reports the data and persists it in database
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

## Reporting flow
1. Collects the data like order number, client_id etc. and persists into the database.

## Matching Engine
Matching engine performs the following functions:
1. Mathcing of buy and sell orders. Matching function must be fast and accurate
2. Send market data to market data publisher
3. Construction of order book.

The Matching engine must be highly available and deterministic. That is, the same sequence of orders must produce the same executions.

## Sequencer
This is what makes the matching engine deterministic.
1. Sequencer has two instances: inbound sequencer and outbound sequencer.
2. Inbound sequencer adds squence ids to each order coming and then sends it to the matching engine.
3. Outbound sequence adds sequence ids to each execution and sends it back to the order manager.
4. This is done to detect any missing orders/executions.
5. Matching engine is connected to these two sequencers, one is used for receiving orders and other for publishing executions backt to order manager.
6. Sequencer also acts like a messaging queue. Kafka could be used but its latency is very low and  is not predictable.

## Order manager
1. Order manager manages an order's state.
2. It also executes the risk checks
3. It verifies the rorders against the wallets funds.
4. It sends the orders to the seqencer and receives the executions from the matchign engine through the sequencer.
5. Some non essential attributes are removed from the order before sending it to the matching engine.

## Client gateway
1. Client gateway does authentication, rate limiting, validations etc. 
2. There will be different client gateways for different type of users. Intitutional clients will have very low latency client gateway.
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
    - Response: id, creationTime, filledqty, remqty, status.
- GET /v1/execution?symbol={:symbol}&orderId={:orderId}&startTime={:startTime}&endTime={:endTime}
    - Parameters: symbol, orderId, startTime, endTime
    - Response: Array of executions(id, orderId, symbol, quanty, price, type, side)
- GET /v1/orderbook/symbol={:symbol}&depth={:depth}
    - Response: bids and asks
- GET /v1/candles?symbols={:symbols}&startTime={:startTIme}&endTime={:endTime}

