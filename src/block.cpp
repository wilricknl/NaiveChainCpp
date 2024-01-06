#include "block.hpp"

#include <picosha2.h>

#include <chrono>

// ----------------------------------------------------------------------------
bool Block::operator==(const Block& other) const
{
	return index == other.index
		&& timestamp == other.timestamp
		&& previous_hash == other.previous_hash
		&& data == other.data
		&& hash == other.hash;
}

// ----------------------------------------------------------------------------
bool Block::operator!=(const Block& other) const
{
	return !(*this == other);
}

// ----------------------------------------------------------------------------
std::string Hash(const Block& block)
{
	return Hash(block.index, block.timestamp, block.previous_hash, block.data);
}

// ----------------------------------------------------------------------------
std::string Hash(
	unsigned index,
	long long timestamp,
	const std::string& previous_hash,
	const std::string& data)
{
	std::string contents = std::to_string(index)
		+ std::to_string(timestamp)
		+ previous_hash
		+ data;
	return picosha2::hash256_hex_string(contents);
}

// ----------------------------------------------------------------------------
long long GetTimestamp()
{
	return std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
}

// ----------------------------------------------------------------------------
Block GetGenesisBlock()
{
	return Block{
		.index = 0,
		.timestamp = 1231006505,
		.previous_hash = "0",
		.data = "wilricknl's naive chain",
		.hash = "fb3a3138771b4994b54726b187e2a7e01680ac9a8031277c0d019ac990e4a519"
	};
}
