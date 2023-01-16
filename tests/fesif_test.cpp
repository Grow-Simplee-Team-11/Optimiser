int main(int argc, char** argv) {
	ifstream input;
	input.open("input.txt");
	Coordinate warehouse;
	input>>warehouse.longitude>>warehouse.latitude;
	Bin bin;
	input>>bin.size.length>>bin.size.width>>bin.size.height;
	bin.capacity = 25;
	int n;
	input>>n;
	vector<item> items(n);
	for(int i=0;i<n;i++) {
		input>>items[i].coordinate.longitude>>items[i].coordinate.latitude;
		input>>items[i].size.length>>items[i].size.width>>items[i].size.height;
		items[i].weight = 1;
	}

	wrapperLMD(items, warehouse, 100, bin);
	freeGlobalMemory();
	localFree();
	
	return 0;
}
