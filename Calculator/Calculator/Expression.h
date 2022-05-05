#pragma once

#include "ExpressionParser.h"
#include "FNode.h"

using namespace std;

class Expression
{
private:
	Expression();

public:
	static Expression* Inst();

	bool IsValid() const;
	double GetResult() const;
	void SetSource(const string& source);
	string GetSource() const;

protected:
	FNode* _root = nullptr;
	string _source = "";

	static Expression* _instance;
};

