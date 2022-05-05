#include "FConstantNode.h"

FConstantNode::FConstantNode(const string& source, double value)
	: FNode(source)
	, _value(value)
{}

double FConstantNode::GetResult() const
{
	return _value;
}

double FConstantNode::GetValue() const
{
	return _value;
}

void FConstantNode::SetValue(double value)
{
	_value = value;
}
