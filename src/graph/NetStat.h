//
// Created by tomek on 11/15/25.
//

#ifndef NETSTAT_H
#define NETSTAT_H
#include <limits.h>
#include <limits>

class NetStat {
  public:
    unsigned int throughput, loss, delay;
    int jitter;

    bool operator==(const NetStat &other) const {
        return this->throughput == other.throughput &&
               this->loss == other.loss && this->delay == other.delay &&
               this->jitter == other.jitter;
    }
    int operator+(const int n) const {
        return static_cast<int>(throughput + n);
    }
    NetStat(unsigned int throughput, unsigned int loss, unsigned int delay,
            int jitter) {
        this->throughput = throughput;
        this->loss = loss;
        this->delay = delay;
        this->jitter = jitter;
    }
    NetStat() : NetStat(0, 0, 0, 0) {}
};

#endif // NETSTAT_H
