#include "ExpressionParser.h"

#include "FConstantNode.h"
#include "FBinaryNode.h"
#include "FUnaryNode.h"

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
	/*
	if (node == null)
	throw new Exception("Can't created node for '" + src + "'.");
	if (!src.equals(node.GetSource()))
	throw new Exception("Can't parse of '" + RemoveOfString(src, node.GetSource()) + "'.");
	*/

	return node;
}

FNodePtr ExpressionParser::PlusMinus(const string& s)
{
	FNodePtr current = MultDiv(s);
	if (current == nullptr) return nullptr;
	//	throw new Exception("Can't created node for '" + s + "'.");
	string rest = RemoveOfString(s, current->GetSource());

	while (!rest.empty())
	{
		char sign = rest.front();
		if ((sign != '+') && (sign != '-'))
		{
			return current;
		}

		string next = rest.substr(1);
		if (next.empty()) return nullptr;
		//	throw new Exception("Can't parse of '" + s + "'.");
		FNodePtr right = MultDiv(next);
		if (right == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + next + "'.");

		FBinaryNode::EOperation oper = (sign == '+') ?
			FBinaryNode::EOperation::Add :
			FBinaryNode::EOperation::Subtract;

		current = FNodePtr(new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper));

		rest = RemoveOfString(s, current->GetSource());
	}

	return current;
}

FNodePtr ExpressionParser::MultDiv(const string& s)
{
	FNodePtr current = Power(s);
	if (current == nullptr) return nullptr;
	//	throw new Exception("Can't created node for '" + s + "'.");
	string rest = RemoveOfString(s, current->GetSource());
	while (!rest.empty())
	{
		char sign = rest.front();
		if ((sign != '*') && (sign != '/'))
		{
			return current;
		}

		string next = rest.substr(1);
		if (next.empty()) return nullptr;
		//	throw new Exception("Can't parse of '" + s + "'.");
		FNodePtr right = Power(next);
		if (right == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + next + "'.");

		FBinaryNode::EOperation oper = (sign == '*') ?
			FBinaryNode::EOperation::Multiply :
			FBinaryNode::EOperation::Divide;

		current = FNodePtr(new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper));
		rest = RemoveOfString(s, current->GetSource());
	}

	return current;
}

FNodePtr ExpressionParser::Power(const string& s)
{
	FNodePtr current = Contain(s);

	if (current == nullptr) return nullptr;
	//	throw new Exception("Can't created node for '" + s + "'.");
	string rest = RemoveOfString(s, current->GetSource());
	while (!rest.empty())
	{
		char sign = rest.front();
		if (sign != '^')
		{
			return current;
		}

		string next = rest.substr(1);
		if (next.empty()) return nullptr;
		//	throw new Exception("Can't parse of '" + s + "'.");
		FNodePtr right = Contain(next);
		if (right == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + next + "'.");

		FBinaryNode::EOperation oper = FBinaryNode::EOperation::Power;
		current = FNodePtr(new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper));
		rest = RemoveOfString(s, current->GetSource());
	}

	return current;
}

FNodePtr ExpressionParser::Contain(const string& s)
{
	if (s.empty()) return nullptr;
	//	throw new Exception("Can't created node for '" + s + "'.");

	char zeroChar = s.front();
	if (zeroChar == '(')
	{
		string into = s.substr(1);
		if (!into.empty() && (into.front() == ')')) return nullptr;
		//	throw new Exception("Contain is empty in '" + s + "'.");

		FNodePtr node = PlusMinus(into);
		if (node == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + into + "'.");

		string src = node->GetSource();
		string rest = RemoveOfString(into, src);
		if (rest.empty() || (rest.front() != ')')) return nullptr;
		//	throw new Exception("Parse error: Not found bracked for close '" + s + "'.");

		return FNodePtr(new FUnaryNode("(" + src + ")", node, FUnaryNode::EOperation::Contain));
	}

	return FuncOrValue(s);
}

FNodePtr ExpressionParser::FuncOrValue(const string& s)
{
	if (s.empty()) return nullptr;
	//	throw new Exception("Input is empty.");

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
			if (node == nullptr) return nullptr;
			//	throw new Exception("Input is empty.");
			return Function(str, node);
		}
		return nullptr;
		//throw new Exception("Not found enter to function '" + str + "'.");
	}
	return Num(s);
}

FNodePtr ExpressionParser::Num(const string& s)
{
	int i = 0;
	int pointCount = 0;
	if (s.front() == '-')
	{
		FNodePtr node = Contain(s.substr(1));
		if (node == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + s.substring(1) + "'.");

		return FNodePtr(new FUnaryNode("-" + node->GetSource(), node, FUnaryNode::EOperation::Not));
	}
	while ((i < s.length()) && (isdigit(s[i]) || (s[i] == '.')))
	{
		if ((s[i] == '.') && (++pointCount > 1)) return nullptr;
		//	throw new Exception("Not valid number '" + s.substring(0, i + 1) + "'");

		i++;
	}
	if (i == 0) return nullptr;
	//	throw new Exception("Can't get number in '" + s + "'");

	double value = atof(s.substr(0, i).c_str());
	return FNodePtr(new FConstantNode(s.substr(0, i), value));
}

FNodePtr ExpressionParser::Function(const string& func, FNodePtr node)
{
	string src = func + node->GetSource();
	FUnaryNode::EOperation oper;
	if (func == "sin") oper = FUnaryNode::EOperation::Sin;
	else if (func == "cos") oper = FUnaryNode::EOperation::Cos;
	else if (func == "tan") oper = FUnaryNode::EOperation::Tan;
	else if (func == "abs") oper = FUnaryNode::EOperation::Abs;
	else return nullptr;
	//else throw new Exception("Function '" + func + "' is not defined.");
	return FNodePtr(new FUnaryNode(src, node, oper));
}

string ExpressionParser::RemoveOfString(const string& source, const string& fragment)
{
	int srcLen = source.length();
	int fragLen = fragment.length();
	int diff = srcLen - fragLen;
	if ((diff == 0) && source == fragment)
	{
		return source.substr(srcLen);
	}

	for (int i = 0; i < diff; i++)
	{
		bool isValid = true;
		for (int j = 0; j < fragLen; j++)
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
