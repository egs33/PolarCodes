#ifndef POLARCODE_GNCOSETCODE_H
#define POLARCODE_GNCOSETCODE_H

#include <vector>
#include "Channel.h"

class GnCosetCode {
protected:
    unsigned int length;
    std::vector<int> informationSet;
    std::vector<int> frozenBits;
    const std::vector<std::pair<int,int>> sendBits;

    GnCosetCode();

    explicit GnCosetCode(const std::vector<std::pair<int, int>> &sendBits);

    void duplicatePath(std::vector<std::pair<std::vector<int>*, double>> &list, const std::vector<int> &y, const Channel &channel) const;

public:

    GnCosetCode(unsigned int length, const std::vector<int> &informationSet, const std::vector<int> &frozenBits,
                const std::vector<std::pair<int, int>> &sendBits);

    GnCosetCode(unsigned int length, std::vector<int> informationSet, std::vector<int> frozenBits);

    GnCosetCode(unsigned int length, std::vector<int> informationSet);

    GnCosetCode(unsigned int length, const std::vector<int> &informationSet,
                const std::vector<std::pair<int, int>> &sendBits);

    std::vector<int> encode(const std::vector<int> &information) const;

    std::vector<int> createU(const std::vector<int> &information) const;

    unsigned int getLength() const;

    const std::vector<int> &getInformationSet() const;

    const std::vector<int> &getFrozenBits() const;

    virtual std::vector<int> SuccessiveCancellationDecode(const std::vector<int> &y, const Channel &channel) const;

    virtual std::vector<int> LlrSCDecode(const std::vector<int> &y, const Channel &channel) const;

    virtual std::vector<int> SCLDecode(const std::vector<int> &y, int L, const Channel &channel) const;
};


#endif //POLARCODE_GNCOSETCODE_H
