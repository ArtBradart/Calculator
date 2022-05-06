#include <iostream>
#include <string>
#include "Expression.h"

using namespace std;

string Calculate(const string& source)
{
	Expression exp(source);
	if (!exp.IsValid())
	{
		return exp.GetErrorMsg();
	}
	return to_string(exp.GetResult());
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