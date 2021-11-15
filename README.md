# RDMA in Go

I just learnt Go,
and I think Goroutine should be a very nice tool to write RDMA-based systems.
Hence I tried to build a simple Go wrapper layer on top of ibverbs,
so that we could interact with RDMA NIC in Go.


Since I just learnt Go, I'm still learning the best practices for Go.
So bear with me on any pitfalls.

## Discussion

I finished the first couple small functions using cgo.
It is actually quite easy to do so.
Now I'm deciding whether I should continue or not.

Regarding the APIs, there are two design choices.
The first is I can expose all the low-level `ibv_xxx` APIS to Go.
However, the downside is that I need to basically rewirte a lot
IBV_XXX related flags and structures in Go. This seesm tedious.
The second approach is to write majority code in C and only
expose a simple set of higher-level APIS like connect, send, receive.
Similar to what I did for Clio and SuperNIC. And I feel the second
approach is actually quite easy to do.

Anyways, I don't have much intentive to go on. May come back if needed. (Nov 15, 2021)