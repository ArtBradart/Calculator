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
	FBinaryNode(const string& source, FNode* left, FNode* right, EOperation oper);

	virtual double GetResult() const override;

	FNode* GetLeft() const;
	void SetLeft(FNode* left);
	FNode* GetRight() const;
	void SetRight(FNode* right);
	EOperation GetOperator() const;
	void SetOperator(EOperation oper);

protected:
	FNode* _left = nullptr;
	FNode* _right = nullptr;
	EOperation _operator = EOperation::Add;
};

