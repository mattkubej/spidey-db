# Spidey DB

Simple in-memory graph database

## How to run

1. `make build`
2. `./build/spidey-db`

Spidey listens on port 6425

## How to communicate

Spidey communicates via [RESP](https://redis.io/topics/protocol), so redis-cli
will work or you can communicate over TCP

### With redis-cli

`redis-cli -p 6425`

### Supported Commands

* COMMAND
* PING
* SETVERTEX _[key]_ _[value]_
* DELVERTEX _[key]_
* GETVERTEX _[key]_
* SETEDGE _[vertex-key]_ _[vertex-key]_
* DELEDGE _[vertex-key]_ _[vertex-key]_
* GETGRAPH
* GETNEIGHBORS _[vertex-key]_ _[distance]_
