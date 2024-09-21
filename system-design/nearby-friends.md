# Introduction

This is similar to a feature on the facebook app where you can see which friends are nearby, if the users give location permissions to the app. Some points to note:

- Assume 1 billion users with 10% users who use this feature
- No need to consider GDPR and other compliance related stuff
- 10 km or near can be considered "nearby" and this radius can be changed
- distance is measured in straight line
- Location history should be stored for ML
- If a friend is inactive for more than 10 mins, they should not be shown on the app

## Functional Requirements

- Users should be able to see nearby friends. Each entry should have the distance and timestamp indicating the last time the distance was updated.
- The list should be updated every few seconds.

## Non functional requirements

- Low latency: Receive location updates from friends without much delay.
- Reliability: Occasional data point loss is acceptable, but system should be reliable overall
- Eventual consistency: Few second delay in receiving location update in replicas should be acceptable with eventual consistency

## Estimation

Total users of nearby friends = 100 million
DAU = 10% of 100 million = 10 million
Users report location every 30 seconds
QPS = 10,000,000/30 = 334,000

## High Level Design

In theory, we can use p2p connections where each phone connects to other phones in vicinity.  This will not work practically:

1. Mobile connects are flaky
2. Battery consumption will be huge because each phone will maintain multiple connections

Therefore, websocket connection is the best protocol to use here.

### Proposed Design

![proposed design](image.png)

#### Load balancer

- Separates out the requests for web socket servers and api servers
- spreads out the traffic evenly

#### RESTful API Server

These servers are stateless servers which handle requests like add/delete friends, profile update etc.

#### Web Socket Servers

These are stateful servers that the clients connect to and send location updates. 
The clients also receive location updates from nearby friends through these servers.

#### Redis location Cache

A cache is maintained with the friend_id and location to keep track of the latest location update. The cache has a TTL which represents if the user is active or not.

- When TTL expires, it means that the location has not been updated for a long time and the user is not active
- When cache is updated with new location, TTL is also updated.

#### User database

SQL database to store user info

#### Location history database

Database to store location history of user for machine learning usecases.

#### Redis Pub/Sub server

