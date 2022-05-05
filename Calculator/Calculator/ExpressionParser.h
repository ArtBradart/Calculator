#pragma once

#include <string>
#include "FNode.h"

using namespace std;

class ExpressionParser
{
public:
	static string Clear(const string& source);
	static FNode* Parse(const string& source);

private:
	static FNode* PlusMinus(const string& s);
	static FNode* MultDiv(const string& s);
	static FNode* Power(const string& s);
	static FNode* Contain(const string& s);
	static FNode* FuncOrValue(const string& s);
	static FNode* Num(const string& s);
	static FNode* Function(const string& func, FNode* node);

	static string RemoveOfString(const string& source, const string& fragment);
};

