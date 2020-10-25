#include<iostream>
#include"functional.h"

using namespace std;

int main() {
	struct plus<int>a;
	
	cout << a(3, 5) << endl;
	return 0;
}