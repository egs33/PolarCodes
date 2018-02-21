#ifndef POLARCODE_BSC_H
#define POLARCODE_BSC_H


#include "Channel.h"

class BSC : public Channel {
private:
    double p;
public:
    explicit BSC(double p);

    int channel(int x) const override;

    double w(int y, int x) const override;

    double symmetricCapacity(int n, int i) const override;
};


#endif //POLARCODE_BSC_H
