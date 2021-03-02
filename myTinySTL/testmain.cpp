#include<iostream>
#include"functional.h"
#include"vector.h"
#include"deque.h"
#include"numeric.h"
using namespace std;
using namespace myTinySTL;
int main() {
	
	deque<int>de;
	
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	de.push_back(2);
	bool tmp = de.empty();
	cout << tmp << endl;
	cout << de.front() << endl;
	cout << de.size() << endl;
	de.pop_back();
	vector<int>vec{ 4,2,56,1,7,0,14,8 };
	int t = 2;
	int res=myTinySTL::count(vec.begin(), vec.end(), 1);
	vector<int>vec2;
	adjacent_difference(vec.begin(), vec.end(), vec.begin());
	for (auto v : vec) {
		cout << v << " ";
	}
	return 0;
}