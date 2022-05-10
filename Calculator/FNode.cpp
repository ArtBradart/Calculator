#include "FNode.h"
#include "FNodeManager.h"

FNode::FNode(const string& s)
{
	SetSource(s);
}

FNode::~FNode()
{
	FNodeManager::Instance().UnregisterNode(_source);
}

void FNode::SetSource(const string& s)
{
	_source = s;
}

string FNode::GetSource() const
{
	return _source;
}
