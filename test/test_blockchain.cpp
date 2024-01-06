#include <catch.hpp>

#include "blockchain.hpp"

// ----------------------------------------------------------------------------
TEST_CASE("Default blockchain", "[Blockchain]")
{
    Blockchain blockchain;
    const auto& blocks = blockchain.GetBlocks();

    REQUIRE(blocks.size() == blockchain.GetLength());
    REQUIRE(blocks.size() == 1);

    const auto& genesis = blocks.at(0);
    REQUIRE(genesis == GetGenesisBlock());
}

// ----------------------------------------------------------------------------
TEST_CASE("Create new blocks", "[Blockchain]")
{
    Blockchain blockchain;

    // initial block
	REQUIRE(blockchain.GetLength() == 1);
    const auto first_block = blockchain.GetLastBlock();

    // second block
    const std::string second_block_data{ "Another block" };
    const auto second_block = blockchain.CreateBlock(second_block_data);

    REQUIRE(blockchain.GetLength() == 2);
    REQUIRE(second_block.index == 1);

    REQUIRE(second_block.previous_hash == first_block.hash);
    REQUIRE(second_block.data == second_block_data);

    // third block
    const std::string third_block_data{ "Only you" };
    const auto third_block = blockchain.CreateBlock(third_block_data);

    REQUIRE(blockchain.GetLength() == 3);
    REQUIRE(third_block.index == 2);
    REQUIRE(third_block.previous_hash == second_block.hash);
    REQUIRE(third_block.data == third_block_data);
}

// ----------------------------------------------------------------------------
TEST_CASE("Add blocks to the chain", "[Blockchain]")
{
    Blockchain blockchain;

    // initial block
    REQUIRE(blockchain.GetLength() == 1);
    const auto first_block = blockchain.GetLastBlock();

    // successfully add custom block
    Block second_block{
        .index = first_block.index + 1,
        .timestamp = GetTimestamp(),
        .previous_hash = first_block.hash,
        .data = "Some data",
        .hash = ""
    };
    second_block.hash = Hash(second_block);

    REQUIRE(blockchain.AddBlock(second_block));
    REQUIRE(blockchain.GetLength() == 2);
    REQUIRE(second_block == blockchain.GetLastBlock());

    // fail due to index
    second_block.index = 4;
    REQUIRE(!blockchain.AddBlock(second_block));
    REQUIRE(blockchain.GetLength() == 2);

    // fix index, but fail due to previous hash
    second_block.index = 2;
    REQUIRE(!blockchain.AddBlock(second_block));
    REQUIRE(blockchain.GetLength() == 2);

    // fix previous hash, fail due to wrong hash of the block
    second_block.previous_hash = second_block.hash;
    REQUIRE(!blockchain.AddBlock(second_block));
    REQUIRE(blockchain.GetLength() == 2);
}

// ----------------------------------------------------------------------------
TEST_CASE("Sync two chains", "[Blockchain]")
{
    Blockchain blockchain;
    auto second_block = blockchain.CreateBlock("A second block");
    auto third_block = blockchain.CreateBlock("A third block");
    REQUIRE(blockchain.GetLength() == 3);

    Blockchain second_blockchain;
    REQUIRE(second_blockchain.AddBlock(second_block));
    REQUIRE(second_blockchain.GetLastBlock() == second_block);

    REQUIRE(second_blockchain.AddBlock(third_block));
    REQUIRE(second_blockchain.GetLastBlock() == third_block);

    REQUIRE(blockchain.GetLength() == second_blockchain.GetLength());
}
