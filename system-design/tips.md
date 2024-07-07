# General Tips

- For service discovery like which is the appropriate server based on load, geography etc. we can use Zookeeper
- Map reduce is not to be used for any user facing feature.
- Most of the cases relational database would work.
- Nonsql databases is to be used only when you know know you would not need certain aggregations on the data for sure. NoSQL databases depend too much on the structure how the data is stored.
- Cassandra is a good choice for highly scalable write heavy system like chat applications.
