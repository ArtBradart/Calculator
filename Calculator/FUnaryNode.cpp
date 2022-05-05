#include "FUnaryNode.h"

#include <cmath>

FUnaryNode::FUnaryNode(const string& source, FNodePtr node, EOperation oper)
	: FNode(source)
	, _node(node)
	, _operator(oper)
{}

double FUnaryNode::GetResult() const
{
	if (_node == nullptr) return 0.0;

	double result = _node->GetResult();
	auto deg2rad = [](double deg)->double {
		return deg * 3.14159265 / 180.0;
	};

	switch (_operator) {
	case Not: return -result;
	case Sin: return sin(deg2rad(result));
	case Cos: return cos(deg2rad(result));
	case Tan: return tan(deg2rad(result));
	case Abs: return abs(result);
	default: return result;
	}
}

FNodePtr FUnaryNode::GetNode() const
{
	return _node;
}

void FUnaryNode::SetNode(FNodePtr node)
{
	_node = node;
}

void FUnaryNode::SetOperator(EOperation oper)
{
	_operator = oper;
}

FUnaryNode::EOperation FUnaryNode::GetOperator() const
{
	return _operator;
}
