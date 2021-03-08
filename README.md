# Spidey DB

Simple in-memory graph database

## How to run

1. `make build`
2. `./build/spidey-db`

Spidey listens on port ...

## How to communicate

Spidey communicates via [RESP](https://redis.io/topics/protocol), so redis-cli
will work or you can communicate over TCP

### With redis-cli

`redis-cli -p ...`

### Supported Commands

* COMMAND
* PING
* SETEDGE _source-vertex-id_ _destination-vertex-id_
* SETVERTEX _id _value_
* GETVERTEX _id_
* GETNEIGHBORS _vertex-id_ _distance_
