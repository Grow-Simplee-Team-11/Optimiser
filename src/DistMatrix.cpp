#include "../include/DistMatrix.hpp"

#define DEBUG _
#include "../include/json.hpp"
using json = nlohmann::json;
using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void parseResponse(string response, vector<vector<double>> &matrix,bool getTime) {
    json j = json::parse(response);
    json origin_dest_pairs = j["resourceSets"][0]["resources"][0]["results"];
    int num_locs = matrix.size();

    for (auto &orig_dist : origin_dest_pairs) {
        int orig_idx = orig_dist["originIndex"];
        int dest_idx = orig_dist["destinationIndex"];
        double dist = orig_dist["travelDistance"];
        double time = orig_dist["travelDuration"];
        if(getTime)
            matrix[orig_idx][dest_idx] = time;
        else
            matrix[orig_idx][dest_idx] = dist;
    }
}

static string getCURLResponse(string curl_url) {
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, curl_url.c_str());
        /* Perform the request, res wil l get the return code */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return readBuffer;
}

std::vector<std::vector<double>> compute_matrix(const std::vector<Coordinate> &locations,
                                                DistanceType disType, bool getTime) {
    int num_pts = locations.size();
    vector<vector<double>> matrix(num_pts, vector<double>(num_pts));

    if (disType == MANHATTAN || disType == EUCLIDEAN || disType == HAVERSINE) {
        auto disfunc = disType == MANHATTAN ? manhattan_dist : (EUCLIDEAN ? euclidean_dist : haversine_dist);
        for (int i = 0; i < locations.size(); i++)
            for (int j = 0; j < locations.size(); j++) {
                matrix[i][j] = disfunc(locations[i], locations[j]);
            }
        return matrix;
    }
    // ----------------------- REAL WORLD DISTANCE USING GOOGLE MAPS API ----------------------- //
    else if(disType == REAL){
        string origin = "", destination = "";
        for (int i = 0; i < locations.size(); i++) {
            origin += to_string(locations[i].latitude) + "," + to_string(locations[i].longitude);
            destination += to_string(locations[i].latitude) + "," + to_string(locations[i].longitude);
            if (i != locations.size() - 1) {
                origin += ";";
                destination += ";";
            }
        }
        // Create curl url and get response
        string curl_url = string("") + BASE_URL + 
                        "?origins=" + origin +
                        "&destinations=" + destination + TRAVEL_MODE +
                        "&key=" + API_KEY;
        string response = getCURLResponse(curl_url);
    #ifdef DEBUG
        cout << "CURL URL:" << curl_url << endl
            << "Response:" << response << endl;
    #endif
        parseResponse(response, matrix,getTime);
    }
    
    return matrix;
}

void testComputeMatrix(bool getTime) {
    srand(42);
    // ------------FOR MANHATTAN DISTANCE AND EUCLIDEAN DISTANCE ----- //
    cout << "Testing compute_matrix() for Manhattan and Euclidean distance" << endl;
    int num_locations = 10;
    vector<Coordinate> locations(num_locations);
    for (int i = 0; i < num_locations; i++){
        locations[i].latitude = rand() % 100;
        locations[i].longitude = rand() % 100;
    }

    // auto matrix = compute_matrix(locations, MANHATTAN,0);
    // auto matrix2 = compute_matrix(locations, EUCLIDEAN,0);
    // for (int i = 0; i < num_locations; i++) {
    //     for (int j = 0; j < num_locations; j++) {
    //         assert(manhattan_dist(locations[i], locations[j]) == matrix[i][j]);
    //         assert(euclidean_dist(locations[i], locations[j]) == matrix2[i][j]);
    //     }
    // }

    // cout << "Test passed" << endl;

    num_locations = 3;
    locations.resize(num_locations);
    locations[0] = {22.339430, 87.325340};        // IIT Kgp
    locations[1] = {22.652830722, 88.441331568};  // CCU
    locations[2] = {21.626617, 87.507431};        // Digha Beach

    auto distance_matrix = compute_matrix(locations, REAL, false);


    cout << "Distance Matrix:" << endl;
    cout << "Location 1: IIT Kharagpur" << endl;
    cout << "Location 2: CCU" << endl;
    cout << "Location 3: Digha Beach" << endl;

    for (int i = 0; i < num_locations; i++) {
        for (int j = 0; j < num_locations; j++) {
            cout << distance_matrix[i][j]  << " " << (getTime == true ? "mins" : "km") << " ";
        }
        cout << endl;
    }
}

// int main() {
//     testComputeMatrix(0);
//     return 0;
// }