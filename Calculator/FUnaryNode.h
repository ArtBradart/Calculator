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
	FUnaryNode(const string& source, FNodePtr node, EOperation oper);

	virtual double GetResult() const override;

	FNodePtr GetNode() const;
	void SetNode(FNodePtr node);
	EOperation GetOperator() const;
	void SetOperator(EOperation oper);

protected:
	FNodePtr _node = nullptr;
	EOperation _operator;
};

