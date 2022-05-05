#pragma once

#include <string>

using namespace std;

class FNode
{
public:
	virtual double GetResult() const = 0;

	FNode() {}
	FNode(const string& s);

	void SetSource(const string& s);
	string GetSource();

protected:
	string _source = "";

};

