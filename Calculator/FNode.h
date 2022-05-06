#pragma once

#include <string>
#include <memory>

using namespace std;

class FNode
{
protected:
	FNode() {}
	FNode(const string& s);

public:
	virtual double GetResult() const = 0;

	void SetSource(const string& s);
	string GetSource() const;

protected:
	string _source = "";

};

typedef shared_ptr<FNode> FNodePtr;

