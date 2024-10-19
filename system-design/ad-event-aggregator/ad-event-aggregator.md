# Ad click event aggregator

## Google Ads 101

- Digital ads have a bidding system called real time bidding(RTB). Advertisers publish what they are willing to pay and publishers publish what they want. Matching happens and that is how the price of ad is decided
- Ad click aggregation helps decide the price of the Ad. Based on the events, two numbers are calculated which are imporant - Click through rate and coversion rate

## Functional Requirements

- Aggregate the number of clicks of ad_id in the last M minutes
- Return top 100 most clicked ads every minute
- Support aggregation filter by different attributes
- Dataset volume is Google scale

## Non functional requirements

- Correctness of aggregated data is important as it is used for RTB and ads billing
- Properly handle delayed or duplicate events
- Robustness: System should be resilient to partional failures
- Latency: End to end latency for aggregation should be few mins but for RTB should be low latency

### Estimations

- 1B DAU
- Assume 1 click per user per day
- total QPS = 1,000,000,000/10000 = 10000 QPS
- peak QPS = 50000 QPS
- Assume single click event occupies 0.1KB
- Daily storage = 0.1KB * 1B = 100GB
- monthy storage = 30 * 100GB = 3TB