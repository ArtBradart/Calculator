#include <iostream>
#include <string>
#include <Windows.h>

#include "Expression.h"
#include "FNodeManager.h"

using namespace std;


struct Result {
	double value = 0.0;
	string errorMsg = "";
	bool isValid = true;
};

enum class EColor : WORD {
	Title = 0x0003,
	Input = 0x000F,
	Result = 0x0002,
	Error = 0x0004,
	Debug = 0x0006
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
		Result({ exp.GetResult(), "", true }):
		Result({ 0.0, exp.GetErrorMsg(), false });
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
		if (result.isValid)
		{
			SetColor(EColor::Result);
			cout << result.value;
		}
		else
		{
			SetColor(EColor::Error);
			cout << result.errorMsg;
		}

		cout << endl << endl;

		SetColor(EColor::Debug);
		cout << "Debug info: " << endl;
		cout << FNodeManager::Instance().DebugString() << endl;
	}

	return 0;
}