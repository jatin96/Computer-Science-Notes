# Google Maps

## Functional Requirements

- User location update: Users update the their location to the server for improving Google maps
- Navigation Service: Navigating to a destination with ETA service
- Map rendering: Rendering map on the device

## Non functional requirements

- Availability: We need high availability
- Smooth navigation: Clients should experience smooth map rendering
- Accuracy: User hould not be given wrong directions
- Data and battery usage: The client should not consume much memory and battery
- Scalability: The design should be scalable

## Map 101

### Going from 3d to 2d
- Map projection is the process of conversion of 3d map to 2d map

### Geocoding

- Process of converions of address to latitude and longitude

### Geohashing

- Process of conversion of a area of land into a string of letters and digit
- The world is divided into smaller grids where each grid is represented in binary
- Binary is then converted into base32 string to represent latitude and longitude in 1 dimention

### Map rendering

- The entire map cannot be loaded in one go because the data would be huge
- Tiling is used where a limited set of grid is loaded based on the client device's viewport
- We store different tiles at different zoom levels. Based on the client device's zoom level, respective tiles are loaded

### Road data processing for navigation algorithms

- Most of the routing algorithms use variation of Dijksta's algorithm
- Routing algorithms operate on graph data structure
- Intersection of roads are nodes and the roads are edges of the graph
- Representing the entire world as graph is too large for any algorithm
- We use tiling and geohashing where the world is divided into grids and small graph data structures are built to represent that grid
- Each graph datastructure hold reference to adjacent grids to stitch together more info if needed

### Hierarchical routing tiles

- We also need tiles at the correct level of detail. For example, when someone is traveling cross country, shopping very detailed road representing each small route would be too complex and would slow down the routing algorithms
- We have three level of tiles
  - First level shows highly detailed local roads
  - Second level shows roads connecting districts
  - Third level shows roads which are major highways
- Each tile can have reference to another tile with a different zoom level to connect highway and local roads, for example
  
## Estimations

### Storage

Road data : TBs of data
Metadata: Metadata for each map tile would be negligible

Map data:

Each tile = 100 KB
total tiles at zoom level 21 = 4.4 trillion
total size = 440 PB
90% of area is ocean
effective area = 50 PB
total area at all zoom levels = 50 + 50/4 + 50/16 .... = 67 PB ~ 100PB

### Server throughput

DAU = 1 billion
each user uses 35 mins of map per week
total week request = 35 billion mins per week = 5 B mins per day
If we send requests every second, then total requests = 5 * 60B / 10^5 = 3 million QPS
lets say we will send it every 15 seconds = 200,000 QPS
Peak QPS = 5 * QPS = 1 million QPS

## High level design

### Location service

- We can send location in batches every 15 seconds.
- At Google Maps scale, the write volume is still very high. We would need to use a write heavy database like Cassandra
- We also would be using Kafka for stream processing of data for various use cases
- HTTP protocol can be used

```
POST /v1/location
Parameters
locs: {lat, long, timestamp) tuple
```

### Navigation Service
- Navigation service will find a reasonably fast route from source to destination
- Accuracy is critical, we can tolerate a little bit of latency

```json
GET /v1/nav?source=A&desitnation=B

Response:
{
  'distance': 5,
  'duration': 1h,
}
```

### Map rendering

- We can pregenerate a set of tiles at various zoom level.
- Each tile is represented by it's geohash.
- The client sends its location and zoom level and based on that we can calculate the tile represented by its geohash
- The tiles are served from CDN
- The client can cache some tiles on the device itself because a user might use a route multiple times. For example, navigating to office everyday will use the same tiles
- How to find which tile to request and make the map url can be done in two ways. First option is to use the latitude, longitude and zoom level to find the geohash and then request the tile using path.com/234df3.png. This would work but the encoding of geohash would be done on different devices running on different platforms. Changing the logic in future would involve updating a large number of devices, which could be a problem. We can create a URL serice for this, whose only job will be to construct the map url and send it back to the client. This will give us more control over the encoding, since we would be owning the server.
- 

## Deep dive

### Data model

#### Routing tiles

#### User location data

#### Geocoding database

#### Precomputed images of world map

### Services

#### Location Service

#### Rendering map 

#### ETA service

#### Ranker service

#### Updater service

### Improvement

### Delivery protocols



