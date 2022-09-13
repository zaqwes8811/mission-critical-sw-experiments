//
// Created by zaqwes on 13.09.2022.
//

// https://stackoverflow.com/questions/11126093/how-do-i-know-if-my-server-has-numa
// https://stackoverflow.com/questions/8154162/numa-aware-cache-aligned-memory-allocation

// example
// https://github.com/ReidAtcheson/numaallocator/blob/master/numaalloc.h

// Best practice?
// https://scicomp.stackexchange.com/questions/2028/portable-multicore-numa-memory-allocation-initialization-best-practices

// https://stackoverflow.com/questions/7986903/can-i-get-the-numa-node-from-a-pointer-address-in-c-on-linux

// Heap - ok? but stack? local vars?
// https://linux.die.net/man/2/get_mempolicy

// boost
// https://www.boost.org/doc/libs/1_66_0/libs/fiber/doc/html/fiber/numa.html