# Google Maps

## Functional Requirements

- User location update
- Navigation service including ETA
- Map rendering

## Non functional requirements

- Availability - High availability
- Consistency - eventual consistency
- Accuracy - users should not be given wrong directions
- Data and battery usage - should use less resources on phone
- smooth navigation - UI should render and refresh smoothly

## Map 101

- Positioning system : measured in latitude and longitude.
- going from 3D to 2D : Since world is a sphere, it is converted into a 2D space using certain algorithms. This is called Map projection.
- Geocoding: The process of converting a location into latitude and longitude is called geocoding.
- geohashing: The process of diving the world into grid and each grid is represented by binary number which are converted into base32.
- Map rendering: How are maps rendered on phones. Google Maps uses the concept of tiling. There are small grids which are fetched according to the location of the user. Different zoom levels have different levels of details in the grid. Appropriate grids are fetched and stitched together as per the Phone's screen.
- Road data processing for nagivation algorithms: Most of the routing algorithms uses some variation of Dijkstra's algorithm. the crossings act as nodes and the roads as edges. Making one big graph data structure for the entire world is not feasible. So a small piece of grid is taken and that grid is represent by graph. this grid has reference to neighboring grids, so we can fetch more data if required.
- We need to maintain 3 sets of routing tiles. One would be the most detailed which would contain small gullies and roads. Other one would be main roads connecting districts. The other would be main highway connecting states. These routing tiles can be combines as per the needs. Routing tiles are needed because in case a user wants to go across states, running the routing algorithm on a very detailed graph would need too much memory and won't be needed. In that case we can use the routing tile representing the highways.

## Storage Estimation

### Map of world

- We would need to storage different tile at different zoom levels
- Assume 1 tile is of 100KB. 
- Assume x million tiles at the higest zoom level
- Each zoom level up would reduce the tiles by 4.
- Total tiles = x + x/4 + x/16 .... * 100KB
- Total storage = 67PB ~ 100PB

### Road info

- We get raw road data from external system which is in TBs

### Metadata

- Each map tile would have some metadata which could be ignored

## Server throughput

- Total DAU = 1 billion
- Assume a user uses 35 mins of navigate per day
- Assume updates happen in batches every 15 seconds
- Total QPS = 1 billion * 35 * 60 requests per day
- Tota QPS = 1 billion * 35 * 60 / 100000 = 3 million
- Total QPS = 3/15 million = 200,000 QPS
- Peak QPS = 5 * QPS = 1 million QPS

TODO
- Read about cassandra which is a high write database
- Read about rastorized and vectorized images and how vectorized images provide a better zooming experience.
