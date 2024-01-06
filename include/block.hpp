#pragma once

#include <chrono>
#include <string>

// ----------------------------------------------------------------------------
/**
 * Block structure
 */
struct Block
{
	/// Index of the block
	unsigned int index;

	/// Timestamp of block creation
	long long timestamp;

	/// Hash of the previous block
	std::string previous_hash;

	/// The data contained in this block
	std::string data;

	/// The block's hash
	std::string hash;

	bool operator==(const Block& other) const;
	bool operator!=(const Block& other) const;
};

// ----------------------------------------------------------------------------
/**
 * Hash the contents of a block
 * @param block The block data
 * @return The block's hash
 */
std::string Hash(const Block& block);

// ----------------------------------------------------------------------------
/**
 * Hash the contents of a block
 * @param index The index of the block
 * @param timestamp The time of creation of the block
 * @param previous_hash The hash of the previous block
 * @param data The block data
 * @return The block's hash
 */
std::string Hash(
	unsigned int index,
	long long timestamp,
	const std::string& previous_hash,
	const std::string& data);

// ----------------------------------------------------------------------------
/**
 * Get the current timestamp
 * @return The timestamp
 */
long long GetTimestamp();

// ----------------------------------------------------------------------------
/**
 * Get the genesis block
 * @return The genesis block
 */
Block GetGenesisBlock();
