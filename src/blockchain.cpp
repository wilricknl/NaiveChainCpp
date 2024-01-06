#include "blockchain.hpp"

#include <iostream>

// ----------------------------------------------------------------------------
Blockchain::Blockchain()
{
	m_blocks.push_back(GetGenesisBlock());
}

// ----------------------------------------------------------------------------
Block Blockchain::CreateBlock(const std::string& data)
{
	const auto& previous_block = GetLastBlock();

	Block block{
		.index = previous_block.index + 1,
		.timestamp = GetTimestamp(),
		.previous_hash = previous_block.hash,
		.data = data,
		.hash = ""
	};
	block.hash = Hash(block);

	m_blocks.push_back(block);

	return block;
}

// ----------------------------------------------------------------------------
bool Blockchain::AddBlock(const Block& block)
{
	const auto& previous_block = GetLastBlock();

	if (block.index != (previous_block.index + 1))
	{
		std::cerr << "Failed to add block with index '" << block.index << "' as last known index is '" << previous_block.index << "'" << std::endl;
		return false;
	}

	if (block.previous_hash != previous_block.hash)
	{
		std::cerr << "The new block does not point to the hash of the last block in the chain" << std::endl;
		return false;
	}

	if (block.hash != Hash(block))
	{
		std::cerr << "The new block has an invalid hash" << std::endl;
		return false;
	}

	m_blocks.push_back(block);
	return true;
}

// ----------------------------------------------------------------------------
const std::vector<Block>& Blockchain::GetBlocks() const
{
	return m_blocks;
}

// ----------------------------------------------------------------------------
const Block& Blockchain::GetLastBlock() const
{
	return m_blocks.at(m_blocks.size() - 1);
}

// ----------------------------------------------------------------------------
size_t Blockchain::GetLength() const
{
	return m_blocks.size();
}
