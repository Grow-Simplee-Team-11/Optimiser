#include "ClusteringInterface.hpp"
#include "fesif.h"

using namespace std;

class FESIF : public ClusteringInterface {
    public:
        void ComputeClusters(vector<item> packages, Coordinate warehouse, int numRiders, Bin b) {
            setClusters(wrapperLMD(packages, warehouse, numRiders, b));
        }
};