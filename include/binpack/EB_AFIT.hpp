#ifndef __EB_AFIT__HEADER__
#define __EB_AFIT__HEADER__
#include "../interface/BinPackInterface.hpp"
class EB_AFIT: public BinPackInterface{
    public:
        EB_AFIT();
        void BinPack(vector<item>& cluster, Bin b);
        float CalculateCost();
};
#endif