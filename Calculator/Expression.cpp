#include "Expression.h"

Expression::Expression(const string& str)
{
	SetSource(str);
}

bool Expression::IsValid() const
{
	return _root != nullptr || !ExpressionParser::IsError();
}

string Expression::GetErrorMsg() const
{
	return ExpressionParser::PopErrorMsg();
}

double Expression::GetResult() const
{
	return _root ? _root->GetResult() : 0.0;
}

void Expression::SetSource(const string& source)
{
	if (_source != source)
	{
		_source = source;
		string temp = ExpressionParser::Clear(_source);
		_root = ExpressionParser::Parse(temp);
	}
}

string Expression::GetSource() const
{
	return _source;
}
