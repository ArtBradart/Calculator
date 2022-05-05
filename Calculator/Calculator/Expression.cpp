#include "Expression.h"

Expression* Expression::Inst()
{
	if (_instance) return _instance;
	return _instance = new Expression();
}

bool Expression::IsValid() const
{
	return _root != nullptr;
}

double Expression::GetResult() const
{
	if (_root != nullptr)
	{
		return _root->GetResult();
	}
	return 0.0;
}

void Expression::SetSource(const string& source)
{

	string temp = ExpressionParser::Clear(source);
	if (_source != temp)
	{
		_source = temp;
		try
		{
			_root = ExpressionParser::Parse(_source);
		}
		catch (...)
		{
			_root = nullptr;
			//e.printStackTrace();
		}
	}
}

string Expression::GetSource() const
{
	return _source;
}
