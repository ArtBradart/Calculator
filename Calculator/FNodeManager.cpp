#include "FNodeManager.h"

FNodeManager& FNodeManager::Instance()
{
	static FNodeManager inst;
	return inst;
}

FNodePtr FNodeManager::GetConstantNode(const std::string& key, double value)
{
	FNodePtr node = FindNode(key);
	if (node) return node;

	node = FNodePtr(new FConstantNode(key, value));
	RegisterNode(key, node);

	return node;
}

FNodePtr FNodeManager::GetUnaryNode(const std::string& key, FNodePtr child, FUnaryNode::EOperation oper)
{
	FNodePtr node = FindNode(key);
	if (node) return node;

	node = FNodePtr(new FUnaryNode(key, child, oper));
	RegisterNode(key, node);

	return node;
}

FNodePtr FNodeManager::GetBinaryNode(const std::string& key, FNodePtr left, FNodePtr right, FBinaryNode::EOperation oper)
{
	FNodePtr node = FindNode(key);
	if (node) return node;
	
	node = FNodePtr(new FBinaryNode(key, left, right, oper));
	RegisterNode(key, node);

	return node;
}

bool FNodeManager::RegisterNode(const std::string& key, FNodePtr node)
{
	if (FindNode(key)) return false;
	_nodes[key] = node;
	return true;
}

bool FNodeManager::UnregisterNode(const std::string& key)
{
	if (FindNode(key)) return false;
	_nodes.erase(key);
	return true;
}

FNodePtr FNodeManager::FindNode(const std::string& key) const
{
	auto node = _nodes.find(key);
	return node != _nodes.end() ? node->second : nullptr;
}

std::string FNodeManager::DebugString() const
{
	std::string str;
	for (const auto& node : _nodes)
		str += node.first + "\n";
	return str;
}
