#include <iostream>
#include <vector>
#include <map>
using namespace std;
typedef int(*mappedFunction)(vector<int>);


int a ( vector<int> A ) {
	return A[0];
}

int b ( vector<int> A ) {
	return 2;
}

int main() {
	map <string, mappedFunction> any_map;
	any_map["A"] = a;
	any_map["B"] = b;
	vector<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);

	cout<< any_map["A"](x) <<endl;
	cout<< any_map["B"](x) <<endl;
}