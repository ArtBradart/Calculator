#include "FNode.h"

FNode::FNode(const string& s)
{
	SetSource(s);
}

void FNode::SetSource(const string& s)
{
	_source = s;
}

string FNode::GetSource()
{
	return _source;
}
