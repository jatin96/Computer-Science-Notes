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

### Server throughput

## High level design

### Location service

### Navigation Service

### Map rendering

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



