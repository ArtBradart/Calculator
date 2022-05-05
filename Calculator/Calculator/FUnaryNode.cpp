#include "FUnaryNode.h"

FUnaryNode::FUnaryNode(const string& source, FNode* node, EOperation oper)
	: FNode(source)
	, _node(node)
	, _operator(oper)
{}

double FUnaryNode::GetResult() const
{
	if (_node == nullptr) return 0.0;

	double result = _node->GetResult();

	switch (_operator) {
	case Not: return -result;
		//case Sin: return sin(Math.toRadians(result));
		//case Cos: return cos(Math.toRadians(result));
		//case Tan: return tan(Math.toRadians(result));
	case Abs: return abs(result);
	default: return result;
	}
}

FNode* FUnaryNode::GetNode() const
{
	return _node;
}

void FUnaryNode::SetNode(FNode* node)
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
