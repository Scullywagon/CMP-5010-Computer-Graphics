
// Arrays passed by reference to functions

#include <iostream>	
using namespace std;

void DoubleElements(int x[], int Size);  // prototype functions
void PrintElements(int x[], int Size);

void main()
{
	int anArray[5] = {1,2,3,4,5}; 
	PrintElements(anArray, 5);   
	DoubleElements(anArray, 5); 
	PrintElements(anArray, 5);
}

void DoubleElements(int x[], int Size)  // when arrays are passed to a function they are passed by reference
{
	for(int i=0; i<Size; i++)
	{
		x[i] *= 2;      //each element of the array is multiplied by two
	}
}

void PrintElements(int x[], int Size)
{
	for(int i=0; i<Size; i++)
	{
		cout << x[i] << " ";
	}
	cout << endl;
}
