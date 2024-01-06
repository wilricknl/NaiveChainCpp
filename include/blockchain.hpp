#pragma once

#include <vector>

#include "block.hpp"

// ----------------------------------------------------------------------------
/**
 * Blockchain structure
 */
class Blockchain
{
public:
	/**
	 * Constructor
	 */
	Blockchain();

	/**
	 * Create a new block
	 * @return The created block
	 */
	Block CreateBlock(const std::string& data);

	/**
	 * Add an existing block to the chain
	 * @return `true` upon success, else `false`
	 */
	bool AddBlock(const Block& block);

	/**
	 * Get all blocks
	 * @return The block contained within the chain
	 */
	const std::vector<Block>& GetBlocks() const;

	/**
	 * Get last block in the chain
	 * @return The last block
	 */
	const Block& GetLastBlock() const;

	/**
	 * Get length of the blockchain
	 * @return The blockchain length
	 */
	size_t GetLength() const;

private:
	/// Storage of all blocks
	std::vector<Block> m_blocks;
};
