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

FNode* ExpressionParser::Parse(const string& source)
{
	string src = Clear(source);
	if (src.empty())
	{
		return new FConstantNode("", 0.0);
	}

	FNode* node = PlusMinus(src);
	/*
	if (node == null)
	throw new Exception("Can't created node for '" + src + "'.");
	if (!src.equals(node.GetSource()))
	throw new Exception("Can't parse of '" + RemoveOfString(src, node.GetSource()) + "'.");
	*/

	return node;
}

FNode* ExpressionParser::PlusMinus(const string& s)
{
	FNode* current = MultDiv(s);
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
		FNode* right = MultDiv(next);
		if (right == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + next + "'.");

		FBinaryNode::EOperation oper = (sign == '+') ?
			FBinaryNode::EOperation::Add :
			FBinaryNode::EOperation::Subtract;

		current = new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper);

		rest = RemoveOfString(s, current->GetSource());
	}

	return current;
}

FNode* ExpressionParser::MultDiv(const string& s)
{
	FNode* current = Power(s);
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
		FNode* right = Power(next);
		if (right == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + next + "'.");

		FBinaryNode::EOperation oper = (sign == '*') ?
			FBinaryNode::EOperation::Multiply :
			FBinaryNode::EOperation::Divide;

		current = new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper);
		rest = RemoveOfString(s, current->GetSource());
	}

	return current;
}

FNode* ExpressionParser::Power(const string& s)
{
	FNode* current = Contain(s);

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
		FNode* right = Contain(next);
		if (right == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + next + "'.");

		FBinaryNode::EOperation oper = FBinaryNode::EOperation::Power;
		current = new FBinaryNode(current->GetSource() + sign + right->GetSource(), current, right, oper);
		rest = RemoveOfString(s, current->GetSource());
	}

	return current;
}

FNode* ExpressionParser::Contain(const string& s)
{
	if (s.empty()) return nullptr;
	//	throw new Exception("Can't created node for '" + s + "'.");

	char zeroChar = s.front();
	if (zeroChar == '(')
	{
		string into = s.substr(1);
		if (!into.empty() && (into.front() == ')')) return nullptr;
		//	throw new Exception("Contain is empty in '" + s + "'.");

		FNode* node = PlusMinus(into);
		if (node == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + into + "'.");

		string src = node->GetSource();
		string rest = RemoveOfString(into, src);
		if (rest.empty() || (rest.front() != ')')) return nullptr;
		//	throw new Exception("Parse error: Not found bracked for close '" + s + "'.");

		return new FUnaryNode("(" + src + ")", node, FUnaryNode::EOperation::Contain);
	}

	return FuncOrValue(s);
}

FNode* ExpressionParser::FuncOrValue(const string& s)
{
	if (s.empty()) return nullptr;
	//	throw new Exception("Input is empty.");

	// TODO: if (Character.isDigit(s.front()))
	{
		return Num(s);
	}

	string str = "";
	int index = 0;
	for (index = 0; index < s.length(); index++)
	{
		char c = s[index];
		// TODO: if (!Character.isLetter(c))
		{
			break;
		}
		str += c;
	}

	if (!str.empty())
	{
		if ((s.length() > index) && (s[index] == '('))
		{
			FNode* node = Contain(s.substr(str.length()));
			if (node == nullptr) return nullptr;
			//	throw new Exception("Input is empty.");
			return Function(str, node);
		}
		return nullptr;
		//throw new Exception("Not found enter to function '" + str + "'.");
	}
	return Num(s);
}

FNode* ExpressionParser::Num(const string& s)
{
	int i = 0;
	int pointCount = 0;
	if (s.front() == '-')
	{
		FNode* node = Contain(s.substr(1));
		if (node == nullptr) return nullptr;
		//	throw new Exception("Can't created node for '" + s.substring(1) + "'.");

		return new FUnaryNode("-" + node->GetSource(), node, FUnaryNode::EOperation::Not);
	}
	// TODO: while ((i < s.length()) && (Character.isDigit(s.charAt(i)) || (s.charAt(i) == '.')))
	{
		if ((s[i] == '.') && (++pointCount > 1)) return nullptr;
		//	throw new Exception("Not valid number '" + s.substring(0, i + 1) + "'");

		i++;
	}
	//if (i == 0)
	//	throw new Exception("Can't get number in '" + s + "'");

	double value = atof(s.substr(0, i).c_str());
	return new FConstantNode(s.substr(0, i), value);
}

FNode* ExpressionParser::Function(const string& func, FNode* node)
{
	string src = func + node->GetSource();
	FUnaryNode::EOperation oper;
	if (func == "sin") oper = FUnaryNode::EOperation::Sin;
	else if (func == "cos") oper = FUnaryNode::EOperation::Cos;
	else if (func == "tan") oper = FUnaryNode::EOperation::Tan;
	else if (func == "abs") oper = FUnaryNode::EOperation::Abs;
	else return nullptr;
	//else throw new Exception("Function '" + func + "' is not defined.");
	return new FUnaryNode(src, node, oper);
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
