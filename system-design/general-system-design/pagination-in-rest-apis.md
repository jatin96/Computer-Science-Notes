# Pagination

The process of returning a subset of results which are managable on the UI instead of returning the entire dataset.

## Types of pagination

### Offset Pagination

- Use offset and limit to fetch data. For example GET /items?offset=20&limit=10
- Pros: Simple to implement and understand
- Cons: Can become inefficient for large datasets because the query would need to scan the entire data uptil offset + limit.

### Token based pagination

- Use a token to represent state of data being returned and use that to fetch the next set of data. For example Get/items?token=xyz&limit=20.
- Pros: Abstracts the pagination details from the clients. The clients can simply use the token from the previous call to get the next set of data.
- Cons: Complicated to implemented

## How to implemented token-based pagination

### Define the token

Token can be based on the unique identifier and can be encoded or decoded.

### API Endpoint Design

GET items/?token=\<token\>&limit=\<number\>

### Generate Token

When returning the paginated results, we should return the token representing the last itemId.

### Fetch data based on token

Here is how to do it on the backend side:

1. Parse Query parameters to get token and limit
2. Decode the token the get itemId
3. Use the following query

```sql
SELECT * from items where id > :lastItemId ORDER BY id LIMIT :limit;
```

4. Return the result along with a new token which is based on the lastItemId.

Note: Token pagination relies on the column being ordered. We can use timestamp column for it as well.
