#pragma once

#include "FNode.h"

class FUnaryNode : public FNode
{
public:
	enum EOperation {
		Not,
		Contain,
		Abs,
		Sin,
		Cos,
		Tan
	};

	FUnaryNode() {}
	FUnaryNode(const string& source, FNode* node, EOperation oper);

	virtual double GetResult() const override;

	FNode* GetNode() const;
	void SetNode(FNode* node);
	EOperation GetOperator() const;
	void SetOperator(EOperation oper);

protected:
	FNode* _node = nullptr;
	EOperation _operator;
};

