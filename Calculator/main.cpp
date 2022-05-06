#include <iostream>
#include <string>
#include <Windows.h>

#include "Expression.h"

using namespace std;


struct Result {
	string str = "";
	bool isValid = true;
};

enum class EColor : WORD {
	Title = 0x0003,
	Input = 0x000F,
	Result = 0x0002,
	Error = 0x0004
};


void SetColor(EColor color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)color);
}

Result Calculate(const string& source)
{
	Expression exp(source);
	return exp.IsValid() ? 
		Result({ to_string(exp.GetResult()), true }):
		Result({ exp.GetErrorMsg(), false });
}


int main(int argc, char** argv)
{
	
	while (true)
	{
		SetColor(EColor::Title);
		cout << "Input: ";

		SetColor(EColor::Input);
		string str;
		getline(cin, str);

		Result result = Calculate(str);
		SetColor(result.isValid ? EColor::Result : EColor::Error);
		cout << result.str << endl << endl;
	}

	return 0;
}