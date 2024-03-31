# How do search engines work?

1. Crawler - crawlers constantly crawl the internet for new web pages. They look for urls, then read the content and other urls within the web page to crawl the entire internet.
2. Indexing - one the content is crawled, it is stored in a database and indexed. Indexing helps in retrieval of the webpage. The pages are categorized based on freshness, content, site quality
3. Ranking - Search engines use complex algorithms to rank the pages based on user engagement, page load speed etc.
4. Querying - user's query is analysed and the sites are served from the index based on the input query.

Source: bytebytego newsletter

# How uber uses redis to scale it's internal usage of cache?

## Why create a caching service?
- Each team in uber created their own redis solution hence there were multiple implementations doing the same thing which wasted engineering hours

## Goals for CacheFront
- Reduce the need for vertical and horizontal scalihg
- improve P50 and P99 latency
- Replace custom built solutions and force teams to use 1 centralized solution managed by DocStore team

## Handling Cache reads
- Read rows from cache
- Read missing rows from DB
- Asynchronously update the cache with missing rows

## Cache invalidation
- TTL could be used but TTL takes time for cache refresh and uber wanted a more frequent invalidation strategy. Reducing TTL could cause more cache misses which would not solve the problem
- Uber used FLUX which is their own CDC(change data capture) service. They listened to DB query logs to know which rows are updated and then use that to update the cache, using a pubsub model. Consumer of the db events will refresh the cache when a row is updated.
- This approach makes cache consistent with DB within seconds

## Latency reduction
- Uber used sliding window to measure the availability spikes in a rolling window. If the spikes in the current window is more than the threshold, the request is short circuited and that cache node is made unavailable. This reduces the latency penalty since the request was bound to go to DB anyways due to the cache node going down

source: [Do we need to use different API gateways for PC, mobile and browser separately?](https://blog.bytebytego.com/p/how-uber-uses-integrated-redis-cache?utm_source=post-email-title&publication_id=817132&post_id=142949172&utm_campaign=email-post-title&isFreemail=true&r=lz1jk&triedRedirect=true&utm_medium=email)
