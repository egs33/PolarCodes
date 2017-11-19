#ifndef POLAR2_CHANNEL_H
#define POLAR2_CHANNEL_H

#include <vector>

class Channel {
public:
    /**
     * @param std:vector<int> x input of channel
     * @return output of channel
     */
    virtual std::vector<int> channel(const std::vector<int> &x) const = 0;

    /**
     * calc likelihood
     * @param y
     * @param x
     * @return
     */
    virtual double w(int y, int x) const = 0;

    /**
     * calc likelihood
     * @param y
     * @param x
     * @return
     */
    double w(const std::vector<int> &y, const std::vector<int> &x) const;

    /**
     * W^i_N(y,u|b)
     * @param length N
     * @param y contains N elements
     * @param u contains (i-1) elements
     * @param bit b
     * @return
     */
    double w(int length, const std::vector<int> &y, const std::vector<int> &u, int bit) const;

    /**
     *
     * @param n length
     * @param i index(0,...n-1)
     * @return
     */
    virtual double symmetricCapacity(int n, int i) const = 0;

    static std::vector<int> combine(const std::vector<int> &u);
};

class BEC : public Channel {
private:
    double p;
public:
    BEC(double p);

    std::vector<int> channel(const std::vector<int> &x) const override;

    double w(int y, int x) const override;

    double symmetricCapacity(int n, int i) const override;
};

class BSC : public Channel {
private:
    double p;
public:
    BSC(double p);

    std::vector<int> channel(const std::vector<int> &x) const override;

    double w(int y, int x) const override;

    double symmetricCapacity(int n, int i) const override;
};


#endif //POLAR2_CHANNEL_H
