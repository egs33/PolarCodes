#include "Channel.h"
#include <random>
#include <ctime>
#include <stdexcept>


double Channel::w(std::vector<int> y, std::vector<int> x) {
    if (y.size() != x.size()) {
        throw std::invalid_argument("different size");
    }
    double ret = 1;
    for (int i = 0; i < y.size(); ++i) {
        ret *= this->w(y[i], x[i]);
    }
    return ret;
}

std::vector<std::vector<int>> createAllBitPatterns(int length) {
    if (length == 0) {
        return std::vector<std::vector<int>>(1, std::vector<int>());
    }
    std::vector<std::vector<int>> recursive = createAllBitPatterns(length - 1);
    std::vector<std::vector<int>> ret;
    for (const std::vector<int> &v : recursive) {
        for (int i = 0; i < 2; ++i) {
            std::vector<int> temp(v);
            temp.push_back(i);
            ret.push_back(temp);
        }
    }
    return ret;
}

double Channel::w(int length, std::vector<int> y, std::vector<int> u, int bit) {
    double sum = 0;
    std::vector<std::vector<int>> allBitPatterns = createAllBitPatterns(length - u.size() - 1);
    for (const std::vector<int> &v : allBitPatterns) {
        std::vector<int> temp(u);
        temp.reserve((unsigned int) length);
        temp.push_back(bit);
        std::copy(v.begin(), v.end(), std::back_inserter(temp));
        sum += this->w(y, Channel::combine(temp));
    }

    return sum / allBitPatterns.size();
}

std::vector<int> Channel::combine(std::vector<int> u) {
    std::vector<int> ret(u.size());
    if (u.size() == 1) {
        ret[0] = u[0];
        return ret;
    } else if (u.size() == 2) {
        ret[0] = u[0] ^ u[1];
        ret[1] = u[1];
        return ret;
    }
    std::vector<int> temp1(u.size() / 2);
    std::vector<int> temp2(u.size() / 2);
    for (int i = 0; i < u.size() / 2; i++) {
        temp1[i] = u[2 * i] ^ u[2 * i + 1];
        temp2[i] = u[2 * i + 1];
    }
    std::vector<int> temp_dest1 = combine(temp1);
    std::vector<int> temp_dest2 = combine(temp2);
    for (int i = 0; i < u.size() / 2; i++) {
        ret[i] = temp_dest1[i];
        ret[u.size() / 2 + i] = temp_dest2[i];
    }
    return ret;
}

BEC::BEC(double p) : p(p) {}

std::vector<int> BEC::channel(std::vector<int> x) {
    std::vector<int> ret(x.size());
    std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<double> random(0.0, 1.0);

    for (int i = 0; i < x.size(); ++i) {
        if (p > random(mt)) {
            ret[i] = 2;
        } else {
            ret[i] = x[i];
        }
    }
    return ret;
}

double BEC::w(int y, int x) {
    if (y == x) {
        return 1 - p;
    } else if (y == 2) {
        return p;
    }
    return 0;
}

double BEC::symmetricCapacity(int n, int i) {
    if (n == 1) {
        return 1 - p;
    }
    if (i % 2 == 0) {
        return std::pow(symmetricCapacity(n / 2, i / 2), 2);
    }
    double temp = symmetricCapacity(n / 2, (i - 1) / 2);
    return 2 * temp - std::pow(temp, 2);
}


BSC::BSC(double p) : p(p) {}

std::vector<int> BSC::channel(std::vector<int> x) {
    std::vector<int> ret(x.size());
    std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<double> random(0.0, 1.0);
    for (int i = 0; i < x.size(); ++i) {
        if (p > random(mt)) {
            ret[i] = 1 - x[i];
        } else {
            ret[i] = x[i];
        }
    }
    return ret;
}

double BSC::w(int y, int x) {
    if (y == x) {
        return 1 - p;
    } else if (y == 1 - x) {
        return p;
    }
    return 0;
}

double BSC::symmetricCapacity(int n, int i) {
    // Unimplemented
    return 0;
}
