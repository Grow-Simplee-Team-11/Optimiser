//**********************
// INCLUDED HEADER FILES
//**********************

#include <time.h>

#include "../../include/binpack/EB_AFIT.hpp"
#include "../../include/datastructures.hpp"

using namespace std;

void sortCluster(vector<item>& cluster) {
    sort(cluster.begin(), cluster.end(), [](const item& a, const item& b) {
        return a.orig_rank < b.orig_rank;
    });
}

double EB_AFIT::getInversionCount(vector<item>& cluster) {
    sortCluster(cluster);
    int num_cnt = 0;
    for (int i = 0; i < cluster.size(); i++) {
        for (int j = i + 1; j < cluster.size(); j++) {
            if (cluster[i].position.y < cluster[j].position.y) {
                if (cluster[i].position.x < cluster[j].position.x + cluster[j].size.length ||
                    cluster[i].position.x + cluster[i].size.length > cluster[j].position.x) {
                    if (cluster[i].position.z < cluster[j].position.z + cluster[j].size.height ||
                        cluster[i].position.z + cluster[i].size.height > cluster[j].position.z) {
                        num_cnt++;
                    }
                }
            }
        }
    }
    int n = cluster.size();
    double ans = 1 - (double)num_cnt / ((n * (n - 1)) / 2);
    cout << "Number Inversions = "<<num_cnt << endl;
    return ans;
}

time_t start, finish;

EB_AFIT::EB_AFIT() {
}

void EB_AFIT::BinPack(vector<item>& cluster, Bin b) {
    time(&start);
    Binpacker binpacker;
    binpacker.n = cluster.size();
    binpacker.totalboxvol = 0;
    for (auto& it : cluster) {
        it.volume = it.size.length * it.size.width * it.size.height;
        binpacker.totalboxvol += it.volume;
    }
    binpacker.temp = 1;
    binpacker.totalvolume = binpacker.temp * b.size.length * b.size.width * b.size.height;
    binpacker.scrapfirst = new scrappad;
    for (int i = 0; i < binpacker.n; i++) {
        binpacker.boxStatus[i] = 0;
    }
    

    binpacker.bestvolume = 0;
    binpacker.packingbest = 0;
    binpacker.hundredpercent = 0;
    binpacker.itenum = 0;
    binpacker.px = b.size.length;
    binpacker.py = b.size.height;
    binpacker.pz = b.size.width;

    binpacker.remainpy = binpacker.py;
    binpacker.remainpz = binpacker.pz;
    binpacker.remainpx = binpacker.px;
    for (int i = 0; i < cluster.size(); i++) {
        cluster[i].orig_rank = i;
    }
    if (binpacker.scrapfirst == NULL) {
        printf("Insufficient memory available\n");
        exit(1);
    }
    int index = binpacker.n - 1;

    while(index>=0){
      int l = 0;
      int r = index;
      int ans = -1;
      Binpacker ansVar;
      while(l<=r){
        int mid = (l + r)/2;

        vector<item> temp;
        for(int i=mid;i<=index;i++){
          temp.push_back(cluster[i]);
        }

        Binpacker tempPacker(binpacker, temp.size());
        tempPacker.execiterations(temp);
        int allPacked = 1;
        for(int i=0;i<temp.size();i++){
          if(tempPacker.boxStatus[i]==0){
            allPacked = 0;
          }
        }

        if(allPacked){
          ans = mid;
          ansVar = tempPacker;
          tempPacker.scrapmemb = tempPacker.scrapfirst;
          r = mid-1;
          
          for(int i=ans;i<=index;i++){
            cluster[i] = temp[i-ans];
          }
        }
        else{
          l = mid+1;
        }
      }

      if(ans==-1){
        index--;
      }
      else{
        index = ans - 1;
        binpacker.remainpy = ansVar.remainpy;
        binpacker.remainpx = ansVar.remainpx;
        binpacker.remainpz = ansVar.remainpz;

        binpacker.packedy = ansVar.packedy;
        binpacker.prepackedy = ansVar.prepackedy;
        binpacker.preremainpy = ansVar.preremainpy;

        binpacker.scrapfirst = ansVar.scrapfirst;

        binpacker.boxx = ansVar.boxx;
        binpacker.boxy = ansVar.boxy;
        binpacker.boxz = ansVar.boxz;

        binpacker.bboxx = ansVar.bboxx;
        binpacker.bboxy = ansVar.bboxy;
        binpacker.bboxz = ansVar.bboxz;

        binpacker.cboxx = ansVar.cboxx;
        binpacker.cboxy = ansVar.cboxy;
        binpacker.cboxz = ansVar.cboxz;

        binpacker.bfx = ansVar.bfx;
        binpacker.bfy = ansVar.bfy;
        binpacker.bfz = ansVar.bfz;

        binpacker.bbfx = ansVar.bbfx;
        binpacker.bbfy = ansVar.bbfy;
        binpacker.bbfz = ansVar.bbfz;

        binpacker.px = ansVar.px;
        binpacker.py = ansVar.py;
        binpacker.pz = ansVar.pz;
      }
    }
    
    time(&finish);
    binpacker.report(cluster);
    this->packed_items = cluster;
    sortCluster(this->packed_items);

    return;
}

float EB_AFIT::CalculateCost() {
    cost = 100 - binpacker.percentagepackedbox;
    return cost;
}

class DataModel{
	public:
	Coordinate warehouse;
	Bin bin;
	vector<item> packages;
	int optimal;
	int numRiders = 5;
};


// int get_rand(int a,int b){
//     return a + rand()%(b - a);
// }

// DataModel ReadVRPs(string filename);



// int main(int argc, char** argv) {
//     DataModel dm;
	
//     if(argv[1] != NULL){
//       try{
//         dm = ReadVRPs(argv[1]);
//       }
//       catch(string err){
//         cout << err;
//       }
      
//     }
//     else{
//       string s;
//       cin >> s;
//       try{
//         dm = ReadVRPs(s);
//       }
//       catch(string err){
//         cout << err;
//       }
//     }

//     EB_AFIT eba;
//     //     104, 96, 84
//     // 1. 70, 104, 24, 4
//     // 2. 14, 104, 48, 2
//     // 3. 40, 52, 36, 3
//     // srand(time(NULL));

//     // int CLUSTER_SIZE = 20;
//     // vector<int> min_dimensions(3),max_dimensions(3),bin_dimensions(3);
//     // min_dimensions = {3,3,3};
//     // max_dimensions = {40,20,40};
//     // bin_dimensions = {80,100,80};

//     // vector<item> cluster(CLUSTER_SIZE);
//     // for(int i = 0;i < CLUSTER_SIZE;i++){
//     //     cluster[i].size.length = get_rand(min_dimensions[0],max_dimensions[0]);
//     //     cluster[i].size.width = get_rand(min_dimensions[1],max_dimensions[1]);
//     //     cluster[i].size.height = get_rand(min_dimensions[2],max_dimensions[2]);
//     // }
//     // item i;
//     // i.size.length = 80;
//     // i.size.width = 80;
//     // i.size.height = 80;
//     // cluster.push_back(i);
//     // cluster[0].size.width = 10;
//     // cluster[0].size.length = 10;
//     // cluster[0].size.height = 10;
//     // cluster[1].size.width = 20;
//     // cluster[1].size.length = 20;
//     // cluster[1].size.height = 20;
//     // cluster[2].size.width = 20;
//     // cluster[2].size.length = 20;
//     // cluster[2].size.height = 30;

//     // Bin b(bin_dimensions[0], bin_dimensions[1], bin_dimensions[2]);

//     auto cluster = dm.packages;
//     auto b = dm.bin;
//     eba.BinPack(cluster, b);

    
//     auto res = eba.GetPackaging();
//     eba.CalculateCost();
//     cout << eba.GetPackagingCost() << endl;
//     cout << eba.getInversionCount(cluster) << endl;

//     for (auto x : cluster) {
//         // x.print();
//         cout << x.orig_rank << "\n";
//         cout << "\n";
//         cout << x.position.x << " " << x.position.y << " " << x.position.z << endl;
//     }



//     bool first;
//     ofstream output;
//     first = true;
//     output.open("../../visual/app/cuboids_to_render.xml");
//     if(!output.is_open()){
//         cout<<"File NOT OPEN"<<endl;
//     }
//     output<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>"<<endl;
//     output<<"<!DOCTYPE boost_serialization>"<<endl;
//     output<<"<boost_serialization signature=\"serialization::archive\" version=\"10\">"<<endl;
//     for(int j = 0 ; j < cluster.size(); j++){
//         first == true ? output<<"<cuboid class_id=\"0\" tracking_level=\"0\" version=\"0\">"<<endl : output<<"<cuboid>"<<endl;
//         first = false;
//         double h = cluster[j].size.height, w = cluster[j].size.width, l = cluster[j].size.length;
//         output<<"<width>"<<l<<"</width>"<<endl;
//         output<<"<height>"<<h<<"</height>"<<endl;
//         output<<"<depth>"<<w<<"</depth>"<<endl;
//         cout<<cluster[j].position.x<<" "<<cluster[j].position.y<<" "<<cluster[j].position.z<<" "<<l<<" "<<h<<" "<<w<<endl;
//         output<<"<x>"<<(double)cluster[j].position.x+l/2<<"</x>"<<endl;
//         output<<"<y>"<<(double)cluster[j].position.y+h/2<<"</y>"<<endl;
//         output<<"<z>"<<(double)cluster[j].position.z+w/2<<"</z>"<<endl;
//         output<<"</cuboid>"<<endl;
//     }
//     output<<"<base class_id=\"1\" tracking_level=\"0\" version=\"0\">"<<endl;
//     output<<"<width>"<<b.size.width<<"</width>"<<endl;
//     output<<"<height>"<<b.size.length<<"</height>"<<endl;
//     output<<"<x>0</x>"<<endl;
//     output<<"<y>0</y>"<<endl;
//     output<<"</base>"<<endl;
//     output<<"</boost_serialization>"<<endl;
//     output.close();

//     return 0;
// }

// DataModel ReadVRPs(string filename){
// 	DataModel dm;
// 	int nbClients = 30;
// 	// add items to datamodel
// 	dm.packages.resize(nbClients);
// 	// dm.bin.capacity = vehicleCapacity;
// 	dm.bin.size.height = 100;
// 	dm.bin.size.length = 80;
// 	dm.bin.size.width = 80;
// 	// dm.warehouse.latitude  = depot_lat;
// 	// dm.warehouse.longitude = depot_lon;
// 	// int ub = 13 , lb = 9;
// 	srand ( time(NULL) );

//     vector<int> min_dimensions(3),max_dimensions(3),bin_dimensions(3);
//     min_dimensions = {3,3,3};
//     max_dimensions = {40,20,40};
//     bin_dimensions = {80,100,80};


// 	for(int i=0; i<nbClients; i++)
// 	{
// 		// dm.packages[i].coordinate.latitude = x_coords[i];
// 		// dm.packages[i].coordinate.longitude = y_coords[i];
// 		dm.packages[i].volume = 1;
// 		dm.packages[i].size.width = get_rand(min_dimensions[0],max_dimensions[0]);
// 		dm.packages[i].size.height = get_rand(min_dimensions[1],max_dimensions[1]);
// 		dm.packages[i].size.length = get_rand(min_dimensions[2],max_dimensions[2]);
//     cout<<dm.packages[i].size.length<<" "<<dm.packages[i].size.height<<" "<<dm.packages[i].size.width<<endl;
// 		// dm.packages[i].weight = demands[i];
// 		// dm.packages[i].time = ((rand() % (ub - lb + 1)) + lb)*60 ;
// 		// dm.packages[i].time = ((rand() % (ub - lb + 10)));
// 	}
// 	return dm;
// }