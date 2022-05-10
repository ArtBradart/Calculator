#pragma once

#include <unordered_map>
#include "FConstantNode.h"
#include "FUnaryNode.h"
#include "FBinaryNode.h"

class FNodeManager
{
public:
	static FNodeManager& Instance();

	FNodePtr GetConstantNode(const std::string& key, double value);
	FNodePtr GetUnaryNode(const std::string& key, FNodePtr child, FUnaryNode::EOperation oper);
	FNodePtr GetBinaryNode(const std::string& key, FNodePtr left, FNodePtr right, FBinaryNode::EOperation oper);

	bool RegisterNode(const std::string& key, FNodePtr node);
	bool UnregisterNode(const std::string& key);
	FNodePtr FindNode(const std::string& key) const;

	std::string DebugString() const;

private:
	FNodeManager() {};

private:
	std::unordered_map<std::string, FNodePtr> _nodes;

};

