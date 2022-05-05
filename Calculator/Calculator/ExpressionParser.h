#pragma once

#include <string>
#include "FNode.h"

using namespace std;

class ExpressionParser
{
public:
	static string Clear(const string& source);
	static FNodePtr Parse(const string& source);

private:
	static FNodePtr PlusMinus(const string& s);
	static FNodePtr MultDiv(const string& s);
	static FNodePtr Power(const string& s);
	static FNodePtr Contain(const string& s);
	static FNodePtr FuncOrValue(const string& s);
	static FNodePtr Num(const string& s);
	static FNodePtr Function(const string& func, FNodePtr node);

	static string RemoveOfString(const string& source, const string& fragment);
};

