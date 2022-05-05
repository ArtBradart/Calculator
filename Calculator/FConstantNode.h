#pragma once

#include "FNode.h"

class FConstantNode : public FNode
{
public:
	FConstantNode() {}
	FConstantNode(const string& source, double value);

	virtual double GetResult() const override;

	double GetValue() const;
	void SetValue(double value);

protected:
	double _value = 0.0;

};

