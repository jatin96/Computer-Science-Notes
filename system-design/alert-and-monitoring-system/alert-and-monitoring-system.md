# Alerting and Monitoring System

## Functional Requirements

- Collection of operational metrics like CPU usage, memory usage, disk space consumption and other like requests per second for a server etc.
- Data retention for 1 year
- Data resolution should be full for 7 days, then 1 min for 30 days and further up by 1 hour after 30 days
- Notification channels: Email, webhooks etc.

## Non functional requirements

- Scalability : System should be scalable to accomodate growing metrics
- Low latency: The system needs to have low query latency for dashboards and alerts
- Reliability: System should be highly reliable to avoid missing critical alerts
- Flexibility: Flexible enough to integrate with new technologies

## Scale

- 100 million DAU
- 1000 server pools
- 100 machines per pool
- 100 metrics per machine
- 10 million metrics in total
- Data retention: 1 year

## High Level Design

### Fundamentals

We will have 5 major components:

- Data collection: Collect metrics data from different sources
- Data transmission: transfer data from source to metrics monitoring system
- Data storage: organize and store incoming data
- Alerting: analyze incoming data, detect anomalies and fire alerts
- Visualization: present the data in charts, graphs etc.

### Data model

- Metrics are repesented as time series data
- We use line protocol to represent the data
- The following fields are used
    - metric_name
    - set of labels
    - array of values and their timestamp

Eg:
- CPU usage
- host:i69
- timestamp: 123242435
- value: 0.34

### Data access pattern

- The system is write heavy because metrics are stored at a high frequency, aroung 10 million metrics per day
- The read load is spiky since alerting system and graphs might do a lot of read queries during certain times

### Data storage system

- We would need custom databases which handle time series data well because these databases can achieve the same level of performance as a traditional database with far few  servers.
- SQL DB might not be a good choice here since calculating moving average might be difficult in terms of query language.
- Relational DB might not perform well under heavy write load
- Indexing would be needed for each label
- Cassandra could be used but we would need to optimize it for time series data storage
- InfluxDB could be used here which is a time series database and provides its own query language which is suitable for time series data
- InfluxDB builds indices on the labels for fast query and aggregation operations

### High Level Design

## Design Deep Dive

### Metrics Collection

#### Pull Model

- The metrics collector pulls the data from the services
- The metrics collector needs to know the complete list of endpoints to pull data from
- The services can be registered in service discovery like Zookeeper which will keep a track of services to pull data from and any changes to them
- The metrics collector will contact Zookeeper to get a list of endpoints to pull data from and then call the those endpoints to collect the data
- The services need to implement a /metrics endpoint to expose the metrics data and might need to integrate with client library as well
- metrics collect can poll service discovery for any changes in endpoints or it can also register for listening for events generated as a result of changes in service endpoints.
- We would need to use multiple metrics collectors for scale and fault tolerance. How will we coordinate so that multiple collectors don't end up persisting duplicate data?
- We can use consistent hash ring for this

#### Pull model

- A collection agent is installed on every server which is a long running software that collects the metrics and sends it to the metrics collector
- Collection agent may also aggregate the metrics before sending them to the metrics collector
- Aggregation reduces the volumne of data sent to the metrics collector
- If push traffic is high on the metrics collector, the collection agent can keep some data in buffer and send it later. This can also result in loss of data if the servers in autoscaling group and rotated out frequently.
- 

