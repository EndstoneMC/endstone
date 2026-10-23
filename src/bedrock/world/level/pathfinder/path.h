#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/level/block_pos.h"

class Path {
public:
    class Node {
    private:
        BlockPos position_;
        std::uint32_t node_type_;
    };

    [[nodiscard]] std::size_t getSize() const { return nodes_.size(); }

private:
    std::vector<Node> nodes_;
    std::size_t index_;
    std::uint8_t completion_;
};

BEDROCK_STATIC_ASSERT_SIZE(Path::Node, 16, 16);
BEDROCK_STATIC_ASSERT_SIZE(Path, 40, 40);
BEDROCK_STATIC_ASSERT_SIZE(std::vector<Path::Node>, 24, 24);
BEDROCK_STATIC_ASSERT_SIZE(std::unique_ptr<Path>, 8, 8);
static_assert(alignof(Path::Node) == 4);
static_assert(alignof(Path) == 8);
