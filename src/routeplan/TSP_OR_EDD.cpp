#include "../../include/routeplan/TSP_OR_EDD.hpp"

// double TSP_OR_EDD::haversine(double lat_1_deg,double lon_1_deg,double lat_2_deg,double lon_2_deg)
// { 
//     double PI = 3.14159265359;
//     double lat_1_rad, lon_1_rad, lat_2_rad, lon_2_rad;
//     lat_1_rad = lat_1_deg * (PI / 180);
//     lon_1_rad = lon_1_deg * (PI / 180);
//     lat_2_rad = lat_2_deg * (PI / 180);
//     lon_2_rad = lon_2_deg * (PI / 180);
//     double delta_lat, delta_lon;
//     delta_lat = lat_1_rad - lat_2_rad;
//     delta_lon = lon_1_rad - lon_2_rad;

//     // Calculate sin^2 (delta / 2) for both lat and long
//     double sdlat = pow(sin(delta_lat / 2), 2);
//     double sdlon = pow(sin(delta_lon / 2), 2);

//     // Radius of the Earth (approximate)
//     const double radius_earth_miles = 3963;
//     const double radius_earth_km = 6378;

//     // http://en.wikipedia/org/wiki/Haversine_formula
//     // d=2r*asin(sqrt(sin^2((lat1-lat2)/2)+cos(l1)cos(l2)sin^2((lon2-lon1)/2)))
//     //  if t = sqrt(sin^2((lat1-lat2)/2)+cos(l1)cos(l2)sin^2((lon2-lon1)/2)
//     //  -> d = 2 * radius_earth * asin (t)	
//     double t = sqrt(sdlat + (cos(lat_1_rad) * cos(lat_2_rad) * sdlon));
//     double distance_miles = 2 * radius_earth_miles * asin(t);
//     double distance_km = 2 * radius_earth_km * asin(t);
//     return distance_km;
// }
// double TSP_OR_EDD::haversine(double lat_1_deg,double lon_1_deg,double lat_2_deg,double lon_2_deg){
//   return sqrt((lat_1_deg-lat_2_deg)*(lat_1_deg-lat_2_deg)+(lon_1_deg-lon_2_deg)*(lon_1_deg-lon_2_deg));
// }
// double TSP_OR_EDD::getX(double lon){
//     // width is map width
//     double x = fmod((2043*(180+lon)/360), (2043 +(2043/2)));

//     return x;
// }

// double TSP_OR_EDD::getY(double lat){
//     // height and width are map height and width
//     double PI = 3.14159265359;
//     double latRad = lat*PI/180;

//     // get y value
//     double mercN = log(tan((PI/4)+(latRad/2)));
//     double y     = (1730/2)-(2043*mercN/(2*PI));
//     return y;
// }

void TSP_OR_EDD::ComputeEuclideanDistanceMatrix(std::vector<item>& cluster)
{
    distances.clear();
    distances.assign(cluster.size()+1, std::vector<int64_t>(cluster.size()+1, int64_t{0}));

    std::vector<item> mod_cluster;
    mod_cluster.push_back(item(1, 1, 1, warehouse.latitude, warehouse.longitude));
    for(auto x: cluster)
    {
        mod_cluster.push_back(x);
    }

    for (int fromNode = 0; fromNode < mod_cluster.size(); fromNode++) {
    for (int toNode = 0; toNode < mod_cluster.size(); toNode++) {
      if (fromNode != toNode)
        distances[fromNode][toNode] = static_cast<int64_t>(
          Dist(mod_cluster[fromNode].coordinate, mod_cluster[toNode].coordinate)*60.0/speed);
    }
  }
}
bool cmp(item a, item b){
  return a.time>b.time;
}
void TSP_OR_EDD::PlanRoute(vector<item> &cluster, Coordinate w){
    
    plannedPath.clear();
    
    if(cluster.size() == 1){
      plannedPath.push_back(cluster[0]);
      cost = 2 * Dist(w, cluster[0].coordinate);
      std::cout << "Route distance: " << cost<< "km";
      std::cout << "  "<<endl;
      return;
    }
    

    RoutingIndexManager manager(cluster.size()+1, num_vehicles, depot);
    RoutingModel routing(manager);
    warehouse = w;
    ComputeEuclideanDistanceMatrix(cluster);
    
    // for(int i = 0 ; i < distances.size(); i++){
    //   for(int j =0 ; j< distances.size(); j++){
    //     cout<<distances[i][j]<<" ";
    //   }
    //   cout<<endl;
    // }
    const int transit_callback_index = routing.RegisterTransitCallback(
      [this, &manager](int64_t from_index,
                                   int64_t to_index) -> int64_t {
        // Convert from routing variable Index to distance matrix NodeIndex.
        auto from_node = manager.IndexToNode(from_index).value();
        auto to_node = manager.IndexToNode(to_index).value();
        return this->distances[from_node][to_node];
      });

    routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

    vector<item> mod_cluster;
    item temp;
    temp.coordinate=w;
    temp.time = 1440;
    mod_cluster.push_back(temp);
    for(auto x: cluster)
    {
        mod_cluster.push_back(x);
    }
    for(int i =0;i<mod_cluster.size();i++){
      cout<<mod_cluster[i].coordinate.latitude<<" "<<mod_cluster[i].coordinate.longitude<<" "<<mod_cluster[i].time<<endl;
    }
    for(int i =0 ;i< distances.size(); i++){
      for(int j = 0; j< distances.size(); j++){
        cout<<distances[i][j]<<" ";
      }
      cout<<endl;
    }
    cout<<"------------------------------------------------------------------------------------------------------------"<<endl;
    std::string time{"Time"};
    routing.AddDimension(transit_callback_index,  // transit callback index
                       int64_t{1440},             // allow waiting time
                       int64_t{300},             // maximum time per vehicle
                       false,  // Don't force start cumul to zero
                       time);
    
    const RoutingDimension& time_dimension = routing.GetDimensionOrDie(time);
    for (int i = 1; i < mod_cluster.size(); ++i) {
    int64_t index = manager.NodeToIndex(RoutingIndexManager::NodeIndex(i));
    cout<<mod_cluster[i].time-480<<endl;
    time_dimension.CumulVar(index)->SetRange(0, mod_cluster[i].time-480);
  }
  for (int i = 0; i < num_vehicles; ++i) {
    int64_t index = routing.Start(i);
    time_dimension.CumulVar(index)->SetRange(0, 1440);
  }
  for (int i = 0; i < num_vehicles; ++i) {
    routing.AddVariableMinimizedByFinalizer(
        time_dimension.CumulVar(routing.Start(i)));
    routing.AddVariableMinimizedByFinalizer(
        time_dimension.CumulVar(routing.End(i)));
  }
    RoutingSearchParameters search_parameters = DefaultRoutingSearchParameters();
    search_parameters.set_first_solution_strategy(
      FirstSolutionStrategy::PATH_CHEAPEST_ARC);
    search_parameters.set_local_search_metaheuristic(
      LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
    search_parameters.mutable_time_limit()->set_seconds(5);
    const Assignment* solution = routing.SolveWithParameters(search_parameters);
    if(solution == NULL){
      sort(cluster.begin(),cluster.end(), cmp);
      cluster.pop_back();
      drop_offs++;
      PlanRoute(cluster, w);
      return;
    }
    savePath(cluster, manager, routing, solution);
}

void TSP_OR_EDD::savePath(vector<item>&clusters ,const RoutingIndexManager& manager,
                   const RoutingModel& routing, const Assignment* solution)
{
  std::vector<item> mod_cluster;
  mod_cluster.push_back(item(1, 1, 1, warehouse.latitude, warehouse.longitude));
  for(auto x: clusters)
  {
      mod_cluster.push_back(x);
  }
  const RoutingDimension& time_dimension = routing.GetDimensionOrDie("Time");
  int64_t total_time{0};
  for (int vehicle_id = 0; vehicle_id < num_vehicles; ++vehicle_id) {
    int64_t index = routing.Start(vehicle_id);
    std::cout << "Route for vehicle " << vehicle_id << ":";
    std::ostringstream route;
    while (routing.IsEnd(index) == false) {
      plannedPath.push_back(mod_cluster[manager.IndexToNode(index).value()]) ;
      auto time_var = time_dimension.CumulVar(index);
      route << manager.IndexToNode(index).value() <<" -> "
      << " Time("
           << solution->Min(time_var) << ", " << solution->Max(time_var)
           << ") -> ";
      
      index = solution->Value(routing.NextVar(index));
    }
    auto time_var = time_dimension.CumulVar(index);
    std::cout << route.str() << manager.IndexToNode(index).value() << " Time("
              << solution->Min(time_var) << ", " << solution->Max(time_var)
              << ")";
    std::cout<<endl;
    std::cout << "Time of the route: " << solution->Min(time_var) << "min";
    total_time += solution->Min(time_var);
    cost = total_time;
  }
  std::cout << "Problem solved in " << routing.solver()->wall_time() << "ms";
}
    // int64_t index = routing.Start(0);
    // int64_t distance{0};
    
    // std::cout<<"Debugggggg \n";
  //   while (routing.IsEnd(index) == false) {
    // std::cout<<mod_cluster[manager.IndexToNode(index).value()].coordinate.latitude << " " << mod_cluster[manager.IndexToNode(index).value()].coordinate.longitude ;
    
    // int64_t previous_index = index;
    // index = solution.Value(routing.NextVar(index));
    // distance += routing.GetArcCostForVehicle(previous_index, index, int64_t{0});
  // }
  // double tot_dist = (double)distance ;
 
  // std::cout << "Route distance: " << tot_dist<< "km";
  // std::cout << "  ";
  // std::cout << "Problem solved in " << routing.solver()->wall_time() << "ms" << std::endl;


void TSP_OR_EDD::CalculateCost(){
    ;
}