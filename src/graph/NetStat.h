//
// Created by tomek on 11/15/25.
//

#ifndef NETSTAT_H
#define NETSTAT_H
struct NetStat {
    unsigned int throughput, loss, delay;
    int jitter;
}
#endif //NETSTAT_H
