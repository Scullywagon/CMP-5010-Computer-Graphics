// This is a very simple C++ program

//It illustrates the use of the cout statement to print to the console window.

//You will notice that two variables of type int are defined.
//Each line finishes with a semicolon
//The main function is enclosed by curly braces, as are all functions and control structures such as
//if statements and for loops.
//Text can be printed out using cout by putting text in between speech marks.

#include <iostream>	 //this is a C++ preprocessor directive. 
                     //Lines beginning with a # are processed by the preprocessor before the 
					 //program is compiled. 
			//iostream must be included for any program which outputs data to the screen
			//or inputs data from the keyboard. 
using namespace std;      


int main()
{
	int x = 2;
	int y = 3;

	//You can print strings out between speech marks.
	//You must separate strings and variable names with '<<'.
	cout << "Sum of x and y: " << x+y << endl;

	return 0;  //since we defined the return type of the function main as
	           //int we should return an integer.
}
