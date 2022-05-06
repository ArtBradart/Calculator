#pragma once

#include "ExpressionParser.h"
#include "FNode.h"

using namespace std;

class Expression
{
public:
	Expression() {};
	Expression(const string& str);

	bool IsValid() const;
	string GetErrorMsg() const;
	double GetResult() const;
	void SetSource(const string& source);
	string GetSource() const;

protected:
	FNodePtr _root = nullptr;
	string _source = "";

};

