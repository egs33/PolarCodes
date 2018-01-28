#include "Channel.h"
#include <random>
#include <ctime>
#include <stdexcept>


double Channel::w(const std::vector<int> &y, const std::vector<int> &x) const {
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

double Channel::w(int length, const std::vector<int> &y, const std::vector<int> &u, int bit) const {
    if (length == 1) {
        return w(y[0], bit);
    }
    std::vector<std::vector<int>> subVectors(4);
    subVectors[0] = std::vector<int>(length / 2); //y[0...N/2]
    subVectors[1] = std::vector<int>(length / 2); //y[N/2+1...N]
    subVectors[2] = std::vector<int>(u.size() / 2); //u[0...i-1](odd)+u[0...i-1](even)
    subVectors[3] = std::vector<int>(u.size() / 2); //u[0...i-1](enen)
    for (int j = 0; j < length / 2; j++) {
        subVectors[0][j] = y[j];
        subVectors[1][j] = y[j + length / 2];
    }
    for (int j = 0; j < u.size() / 2; j++) {
        subVectors[2][j] = u[2 * j] ^ u[2 * j + 1];
        subVectors[3][j] = u[2 * j + 1];
    }
    if (u.size() % 2 == 0) {
        double sum = 0;
        for (int j = 0; j < 2; j++) {
            sum += w(length / 2, subVectors[0], subVectors[2], bit ^ j) *
                   w(length / 2, subVectors[1], subVectors[3], j) / 2;
        }
        return sum;
    } else {
        return w(length / 2, subVectors[0], subVectors[2], u[u.size() - 1] ^ bit) *
               w(length / 2, subVectors[1], subVectors[3], bit) / 2;
    }
}

std::vector<int> Channel::combine(const std::vector<int> &u) {
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

int Channel::combinedAt(const std::vector<int> &u, int x, int y) {
    if (x == 0) {
        return u[y];
    }

    if (y < u.size() / 2) {
        std::vector<int> temp(u.size() / 2);
        for (int i = 0; i < u.size() / 2; i++) {
            temp[i] = u[2 * i] ^ u[2 * i + 1];
        }
        return combinedAt(temp, x - 1, y);
    } else {
        std::vector<int> temp(u.size() / 2);
        for (int i = 0; i < u.size() / 2; i++) {
            temp[i] = u[2 * i + 1];
        }
        return combinedAt(temp, x - 1, y - u.size() / 2);
    }
}

double Channel::logLikelihoodRatio(int length, const std::vector<int> &y, const std::vector<int> &u) const {
    if (length == 1) {
        return log2(w(y[0], 0) / w(y[0], 1));
    }
    std::vector<std::vector<int>> subVectors(4);
    subVectors[0] = std::vector<int>(length / 2); //y[0...N/2]
    subVectors[1] = std::vector<int>(length / 2); //y[N/2+1...N]
    subVectors[2] = std::vector<int>(u.size() / 2); //u[0...i-1](odd)+u[0...i-1](even)
    subVectors[3] = std::vector<int>(u.size() / 2); //u[0...i-1](enen)
    for (int j = 0; j < length / 2; j++) {
        subVectors[0][j] = y[j];
        subVectors[1][j] = y[j + length / 2];
    }
    for (int j = 0; j < u.size() / 2; j++) {
        subVectors[2][j] = u[2 * j] ^ u[2 * j + 1];
        subVectors[3][j] = u[2 * j + 1];
    }
    if (u.size() % 2 == 0) {
        double temp_a = logLikelihoodRatio(length / 2, subVectors[0], subVectors[2]);
        double temp_b = logLikelihoodRatio(length / 2, subVectors[1], subVectors[3]);

        return 2 * atanh(tanh(temp_a / 2) * tanh(temp_b / 2));
    } else {
        double temp = (u[u.size() - 1] == 1 ? -1 : 1) * logLikelihoodRatio(length / 2, subVectors[0], subVectors[2]) +
                      logLikelihoodRatio(length / 2, subVectors[1], subVectors[3]);
        return std::isnan(temp) ? 0 : temp;
    }
}

std::vector<int> Channel::channel(const std::vector<int> &x) const {
    std::vector<int> ret(x.size());
    for (int i = 0; i < x.size(); ++i) {
        ret[i] = channel(x[i]);
    }
    return ret;
}

BEC::BEC(double p) : p(p) {}

double BEC::w(int y, int x) const {
    if (y == x) {
        return 1 - p;
    } else if (y == 2) {
        return p;
    }
    return 0;
}

double BEC::symmetricCapacity(int n, int i) const {
    if (n == 1) {
        return 1 - p;
    }
    if (i % 2 == 0) {
        return std::pow(symmetricCapacity(n / 2, i / 2), 2);
    }
    double temp = symmetricCapacity(n / 2, (i - 1) / 2);
    return 2 * temp - std::pow(temp, 2);
}

int BEC::channel(int x) const {
    static std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    static std::uniform_real_distribution<double> random(0.0, 1.0);

    if (p > random(mt)) {
        return 2;
    }
    return x;
}

BSC::BSC(double p) : p(p) {}

int BSC::channel(int x) const {
    static std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
    static std::uniform_real_distribution<double> random(0.0, 1.0);

    if (p > random(mt)) {
        return 1 - x;
    }
    return x;
}

double BSC::w(int y, int x) const {
    if (y == x) {
        return 1 - p;
    } else if (y == 1 - x) {
        return p;
    }
    return 0;
}

double BSC::symmetricCapacity(int n, int i) const {
    // Unimplemented
    return 0;
}
