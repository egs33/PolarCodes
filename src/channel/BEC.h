#ifndef POLARCODE_BEC_H
#define POLARCODE_BEC_H

#include "Channel.h"

class BEC : public Channel {
private:
    double p;
public:
    explicit BEC(double p);

    int channel(int x) const override;

    double w(int y, int x) const override;

    double symmetricCapacity(int n, int i) const override;
};


#endif //POLARCODE_BEC_H
