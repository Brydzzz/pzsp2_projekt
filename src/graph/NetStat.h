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

    NetStat(unsigned int  throughput, unsigned int loss,unsigned int delay, int jitter) {
        this->throughput = throughput;
        this->loss = loss;
        this->delay = delay;
        this->jitter = jitter;
    }
    NetStat() : NetStat(0,0,0,0) {

    }

};
namespace std {
    template<>
    struct numeric_limits<NetStat> {
        static constexpr NetStat max() noexcept {
            return NetStat(UINT_MAX, UINT_MAX, UINT_MAX, INT_MAX);
        }
    };
}


#endif //NETSTAT_H
