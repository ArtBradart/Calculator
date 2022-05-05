#pragma once

#include "FNode.h"

class FBinaryNode : public FNode
{
public:
	enum EOperation {
		Power,
		Divide,
		Multiply,
		Subtract,
		Add
	};

	FBinaryNode() {}
	FBinaryNode(const string& source, FNodePtr left, FNodePtr right, EOperation oper);

	virtual double GetResult() const override;

	FNodePtr GetLeft() const;
	void SetLeft(FNodePtr left);
	FNodePtr GetRight() const;
	void SetRight(FNodePtr right);
	EOperation GetOperator() const;
	void SetOperator(EOperation oper);

protected:
	FNodePtr _left = nullptr;
	FNodePtr _right = nullptr;
	EOperation _operator = EOperation::Add;
};

