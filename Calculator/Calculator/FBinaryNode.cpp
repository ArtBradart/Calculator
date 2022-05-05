#include "FBinaryNode.h"

FBinaryNode::FBinaryNode(const string& source, FNodePtr left, FNodePtr right, EOperation oper)
	: FNode(source)
	, _left(left)
	, _right(right)
	, _operator(oper)
{}

double FBinaryNode::GetResult() const
{
	if ((_left == nullptr) || (_right == nullptr)) {
		return 0.0;
	}

	double leftResult = _left->GetResult();
	double rightResult = _right->GetResult();

	switch (_operator)
	{
	case Power: return pow(leftResult, rightResult);
	case Add: return leftResult + rightResult;
	case Divide: return leftResult / rightResult;
	case Multiply: return leftResult * rightResult;
	case Subtract: return leftResult - rightResult;
	}

	return 0.0;
}

FNodePtr FBinaryNode::GetLeft() const
{
	return _left;
}

void FBinaryNode::SetLeft(FNodePtr left)
{
	_left = left;
}

// Right.

FNodePtr FBinaryNode::GetRight() const
{
	return _right;
}

void FBinaryNode::SetRight(FNodePtr right)
{
	_right = right;
}

FBinaryNode::EOperation FBinaryNode::GetOperator() const
{
	return _operator;
}

void FBinaryNode::SetOperator(EOperation oper)
{
	_operator = oper;
}
