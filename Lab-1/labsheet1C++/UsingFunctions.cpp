//Simple example of a function in C++

#include <iostream>	
using namespace std;

int squareInt(int p) // this function takes one parameter - int p
{                    // it returns an integer 
	return p*p;
}

void main()
{
	int x = 2;
	int y = 0;
	y = squareInt(x);  
	cout << "Square of x: " << y << endl;
}
