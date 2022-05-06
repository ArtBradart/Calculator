#include "ExpressionParser.h"

#include "FConstantNode.h"
#include "FBinaryNode.h"
#include "FUnaryNode.h"

string ExpressionParser::_errorMsg = "";

string ExpressionParser::Clear(const string& source)
{
	string str = source;
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	return str;
}

FNodePtr ExpressionParser::Parse(const string& source)
{
	string src = Clear(source);
	if (src.empty())
	{
		return FNodePtr(new FConstantNode("", 0.0));
	}

	FNodePtr node = PlusMinus(src);
	if (node == nullptr)
	{
		PushErrorMsg("Can't created node for '" + src + "'.");
		return nullptr;
	}
	
	if (src != node->GetSource())
	{
		PushErrorMsg("Can't parse of '" + RemoveSubString(src, node->GetSource()) + "'.");
		return nullptr;
	}

	return node;
}

bool ExpressionParser::IsError()
{
	return !_errorMsg.empty();
}

string ExpressionParser::PopErrorMsg()
{
	string msg = _errorMsg;
	_errorMsg = "";
	return msg;
}

FNodePtr ExpressionParser::PlusMinus(const string& s)
{
	FNodePtr current = MultDiv(s);
	if (current == nullptr)
	{
		PushErrorMsg("Can't created node for '" + s + "'.");
		return nullptr; 
	}
	
	string rest = RemoveSubString(s, current->GetSource());

	while (!rest.empty())
	{
		char sign = rest.front();
		if ((sign != '+') && (sign != '-'))
		{
			return current;
		}

		string next = rest.substr(1);
		if (next.empty())
		{
			PushErrorMsg("Can't parse of '" + s + "'.");
			return nullptr;
		}

		FNodePtr right = MultDiv(next);
		if (right == nullptr)
		{
			PushErrorMsg("Can't created node for '" + next + "'.");
			return nullptr;
		}

		FBinaryNode::EOperation oper = (sign == '+') ?
			FBinaryNode::EOperation::Add :
			FBinaryNode::EOperation::Subtract;

		current = FNodePtr(new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper));
		rest = RemoveSubString(s, current->GetSource());
	}

	return current;
}

FNodePtr ExpressionParser::MultDiv(const string& s)
{
	FNodePtr current = Power(s);
	if (current == nullptr)
	{
		PushErrorMsg("Can't created node for '" + s + "'.");
		return nullptr;
	}

	string rest = RemoveSubString(s, current->GetSource());
	while (!rest.empty())
	{
		char sign = rest.front();
		if ((sign != '*') && (sign != '/'))
		{
			return current;
		}

		string next = rest.substr(1);
		if (next.empty())
		{
			PushErrorMsg("Can't parse of '" + s + "'.");
			return nullptr;
		}

		FNodePtr right = Power(next);
		if (right == nullptr)
		{
			PushErrorMsg("Can't created node for '" + next + "'.");
			return nullptr;
		}

		FBinaryNode::EOperation oper = (sign == '*') ?
			FBinaryNode::EOperation::Multiply :
			FBinaryNode::EOperation::Divide;

		current = FNodePtr(new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper));
		rest = RemoveSubString(s, current->GetSource());
	}

	return current;
}

FNodePtr ExpressionParser::Power(const string& s)
{
	FNodePtr current = Contain(s);
	if (current == nullptr)
	{
		PushErrorMsg("Can't created node for '" + s + "'.");
		return nullptr;
	}

	string rest = RemoveSubString(s, current->GetSource());
	while (!rest.empty())
	{
		char sign = rest.front();
		if (sign != '^')
		{
			return current;
		}

		string next = rest.substr(1);
		if (next.empty())
		{
			PushErrorMsg("Can't parse of '" + s + "'.");
			return nullptr;
		}

		FNodePtr right = Contain(next);
		if (right == nullptr)
		{
			PushErrorMsg("Can't created node for '" + next + "'.");
			return nullptr;
		}

		FBinaryNode::EOperation oper = FBinaryNode::EOperation::Power;
		current = FNodePtr(new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper));
		rest = RemoveSubString(s, current->GetSource());
	}

	return current;
}

FNodePtr ExpressionParser::Contain(const string& s)
{
	if (s.empty())
	{
		PushErrorMsg("Can't created node for '" + s + "'.");
		return nullptr;
	}

	char zeroChar = s.front();
	if (zeroChar == '(')
	{
		string into = s.substr(1);
		if (!into.empty() && (into.front() == ')'))
		{
			PushErrorMsg("Contain is empty in '" + s + "'.");
			return nullptr;
		}

		FNodePtr node = PlusMinus(into);
		if (node == nullptr)
		{
			PushErrorMsg("Can't created node for '" + into + "'.");
			return nullptr;
		}

		string src = node->GetSource();
		string rest = RemoveSubString(into, src);
		if (rest.empty() || (rest.front() != ')'))
		{
			PushErrorMsg("Parse error: Not found bracked for close '" + s + "'.");
			return nullptr;
		}

		return FNodePtr(new FUnaryNode("(" + src + ")", node, FUnaryNode::EOperation::Contain));
	}

	return FuncOrValue(s);
}

FNodePtr ExpressionParser::FuncOrValue(const string& s)
{
	if (s.empty())
	{
		PushErrorMsg("Input is empty.");
		return nullptr;
	}

	if (isdigit(s.front()))
	{
		return Num(s);
	}

	string str = "";
	size_t index = 0;
	for (index = 0; index < s.length(); index++)
	{
		char c = s[index];
		if (!isalpha(c)) break;
		str += c;
	}

	if (!str.empty())
	{
		if ((s.length() > index) && (s[index] == '('))
		{
			FNodePtr node = Contain(s.substr(str.length()));
			if (node == nullptr)
			{
				PushErrorMsg("Input is empty.");
				return nullptr;
			}

			return Function(str, node);
		}
		
		PushErrorMsg("Not found enter to function '" + str + "'.");
		return nullptr;
	}

	return Num(s);
}

FNodePtr ExpressionParser::Num(const string& s)
{
	// Check denial.
	if (s.front() == '-')
	{
		FNodePtr node = Contain(s.substr(1));
		if (node == nullptr)
		{
			PushErrorMsg("Can't created node for '" + s.substr(1) + "'.");
			return nullptr;
		}

		return FNodePtr(new FUnaryNode("-" + node->GetSource(), node, FUnaryNode::EOperation::Not));
	}

	// Number recognize.
	auto isCharPoint = [](char c)->bool {
		return c == '.' || c == ',';
	};
	auto isCharBelongNumber = [isCharPoint](char c)->bool {
		return isdigit(c) || isCharPoint(c);
	};

	int digitCount = 0;
	int pointCount = 0;
	while (digitCount < s.length())
	{
		char symbol = s[digitCount];
		// Check end number.
		if (!isCharBelongNumber(symbol))
		{
			break;
		}

		// Check double points.
		if (isCharPoint(symbol))
		{
			pointCount++;
			if (pointCount > 1)
			{
				PushErrorMsg("Not valid number '" + s.substr(0, digitCount + 1) + "'");
				return nullptr;
			}
		}

		digitCount++;
	}

	if (digitCount == 0)
	{
		PushErrorMsg("Can't get number in '" + s + "'");
		return nullptr;
	}

	double value = atof(s.substr(0, digitCount).c_str());
	return FNodePtr(new FConstantNode(s.substr(0, digitCount), value));
}

FNodePtr ExpressionParser::Function(const string& func, FNodePtr node)
{
	string src = func + node->GetSource();
	FUnaryNode::EOperation oper;
	if (func == "sin") oper = FUnaryNode::EOperation::Sin;
	else if (func == "cos") oper = FUnaryNode::EOperation::Cos;
	else if (func == "tan") oper = FUnaryNode::EOperation::Tan;
	else if (func == "abs") oper = FUnaryNode::EOperation::Abs;
	else
	{
		PushErrorMsg("Function '" + func + "' is not defined.");
		return nullptr;
	}

	return FNodePtr(new FUnaryNode(src, node, oper));
}

string ExpressionParser::RemoveSubString(const string& source, const string& fragment)
{
	size_t srcLen = source.length();
	size_t fragLen = fragment.length();
	size_t diff = srcLen - fragLen;
	if (diff < 0)
	{
		// Fragment size bigger then source size. 
		return source;
	}

	if ((diff == 0) && source == fragment)
	{
		// Fragment and source equals.
		return "";
	}

	for (size_t i = 0; i < diff; i++)
	{
		bool isValid = true;
		for (size_t j = 0; j < fragLen; j++)
		{
			if (source[i + j] != fragment[j])
			{
				isValid = false;
			}
		}

		if (isValid)
		{
			return source.substr(0, i) + source.substr(i + fragLen);
		}
	}

	return source;
}

void ExpressionParser::PushErrorMsg(const string& msg)
{
	_errorMsg += "\n" + msg;
}
