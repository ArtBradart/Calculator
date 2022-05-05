#include <iostream>
#include <string>
#include "Expression.h"

using namespace std;

string Calculate(const string& source)
{
	Expression exp(source);
	return exp.IsValid() ? 
		to_string(exp.GetResult()):
		"Error";
}

int main(int argc, char** argv)
{
	while (true)
	{
		cout << "Input: ";
		string str;
		getline(cin, str);
		cout << Calculate(str) << endl << endl;
	}

	return 0;
}