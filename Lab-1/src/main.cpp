#include <iostream>
#include <math.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

using namespace std;

int first();
int arrays();
int usingFunctions();
float quadraticFormula(float a, float b, float c);

int main()
{
    cout << "FIRST" << endl;
    first();
    cout << "ARRAY" << endl;
    arrays();
    cout << "FUNCTIONS" << endl;
    usingFunctions();
    cout << "QUADRATIC FORMULA" << endl;
    cout << quadraticFormula(3, 4, 1) << endl;
    return 0;    
}

// This is a very simple C++ program

//It illustrates the use of the cout statement to print to the console window.

//You will notice that two variables of type int are defined.
//Each line finishes with a semicolon
//The main function is enclosed by curly braces, as are all functions and control structures such as
//if statements and for loops.
//Text can be printed out using cout by putting text in between speech marks.

int first()
{
	int x = 2;
	int y = 3;

	//You can print strings out between speech marks.
	//You must separate strings and variable names with '<<'.
	cout << "Sum of x and y: " << x+y << endl;

	return 0;  //since we defined the return type of the function main as
	           //int we should return an integer.
}

void DoubleElements(int x[], int Size);  // prototype functions
void PrintElements(int x[], int Size);

int arrays()
{
	int anArray[5] = {1,2,3,4,5}; 
	PrintElements(anArray, 5);   
	DoubleElements(anArray, 5); 
	PrintElements(anArray, 5);
	return 0;
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

int squareInt(int p) // this function takes one parameter - int p
{                    // it returns an integer 
	return p*p;
}

int usingFunctions()
{
	int x = 2;
	int y = 0;
	y = squareInt(x);  
	cout << "Square of x: " << y << endl;
	return 0;
}

float quadraticFormula(float a, float b, float c)
{
    float top = (-1 * b) + (sqrt((b * b) - (4 * a * c)));
    float bottom = 2 * a;
    float result = top / bottom;
    return result;
}