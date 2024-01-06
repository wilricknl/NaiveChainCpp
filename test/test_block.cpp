#include <catch.hpp>

#include "block.hpp"

// ----------------------------------------------------------------------------
TEST_CASE("Genesis block", "[Block]")
{
    const auto genesis = GetGenesisBlock();

    REQUIRE(genesis.index == 0);
    REQUIRE(genesis.previous_hash == "0");
    REQUIRE(genesis.timestamp == 1231006505);
    REQUIRE(genesis.data == "wilricknl's naive chain");
    REQUIRE(genesis.hash == Hash(genesis));
}

// ----------------------------------------------------------------------------
TEST_CASE("Block comparison", "[Block]")
{
    Block block{
        .index = 0,
        .timestamp = 1231006505,
        .previous_hash = "0",
        .data = "wilricknl's naive chain",
        .hash = "fb3a3138771b4994b54726b187e2a7e01680ac9a8031277c0d019ac990e4a519"
    };
    REQUIRE(block == GetGenesisBlock());

    block.index = 1;
    REQUIRE(block != GetGenesisBlock());
}
