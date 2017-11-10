#ifndef POLAR2_CHANNEL_H
#define POLAR2_CHANNEL_H

#include <vector>

class Channel {
public:
    /**
     * @param std:vector<int> x input of channel
     * @return output of channel
     */
    virtual std::vector<int> channel(std::vector<int> x) = 0;

    /**
     * calc likelihood
     * @param y
     * @param x
     * @return
     */
    virtual double w(int y, int x) = 0;

    /**
     * calc likelihood
     * @param y
     * @param x
     * @return
     */
    double w(std::vector<int> y, std::vector<int> x);

    /**
     * W^i_N(y,u|b)
     * @param length N
     * @param y contains N elements
     * @param u contains (i-1) elements
     * @param bit b
     * @return
     */
    double w(int length, std::vector<int> y, std::vector<int> u, int bit);

    static virtual std::vector<int> combine(std::vector<int> u) final;
};

class BEC : public Channel {
private:
    double p;
public:
    BEC(double p);

    std::vector<int> channel(std::vector<int> x) override;

    double w(int y, int x) override;
};

class BSC : public Channel {
private:
    double p;
public:
    BSC(double p);

    std::vector<int> channel(std::vector<int> x) override;

    double w(int y, int x) override;
};


#endif //POLAR2_CHANNEL_H
