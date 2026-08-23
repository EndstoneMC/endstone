// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libdeflate.h>
#include <zlib.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace {

constexpr int kRawWindowBits = -15;
constexpr int kVanillaLevel = 7;
constexpr int kEndstoneLevel = 6;

std::string zlibDeflateRaw(const std::string &input, int level)
{
    z_stream stream{};
    EXPECT_EQ(deflateInit2(&stream, level, Z_DEFLATED, kRawWindowBits, 8, Z_DEFAULT_STRATEGY), Z_OK);
    std::string out(deflateBound(&stream, input.size()), '\0');
    stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input.data()));
    stream.avail_in = static_cast<uInt>(input.size());
    stream.next_out = reinterpret_cast<Bytef *>(out.data());
    stream.avail_out = static_cast<uInt>(out.size());
    EXPECT_EQ(deflate(&stream, Z_FINISH), Z_STREAM_END);
    out.resize(stream.total_out);
    deflateEnd(&stream);
    return out;
}

std::string zlibInflateRaw(const std::string &input, std::size_t expected_size)
{
    z_stream stream{};
    EXPECT_EQ(inflateInit2(&stream, kRawWindowBits), Z_OK);
    std::string out(expected_size, '\0');
    stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input.data()));
    stream.avail_in = static_cast<uInt>(input.size());
    stream.next_out = reinterpret_cast<Bytef *>(out.data());
    stream.avail_out = static_cast<uInt>(out.size());
    const auto status = inflate(&stream, Z_FINISH);
    EXPECT_EQ(status, Z_STREAM_END);
    out.resize(stream.total_out);
    inflateEnd(&stream);
    return out;
}

std::string libdeflateDeflateRaw(const std::string &input, int level)
{
    auto *compressor = libdeflate_alloc_compressor(level);
    EXPECT_NE(compressor, nullptr);
    std::string out(libdeflate_deflate_compress_bound(compressor, input.size()), '\0');
    const auto written = libdeflate_deflate_compress(compressor, input.data(), input.size(), out.data(), out.size());
    libdeflate_free_compressor(compressor);
    EXPECT_NE(written, 0U);
    out.resize(written);
    return out;
}

std::string libdeflateInflateRaw(const std::string &input, std::size_t expected_size)
{
    auto *decompressor = libdeflate_alloc_decompressor();
    EXPECT_NE(decompressor, nullptr);
    std::string out(expected_size, '\0');
    std::size_t written = 0;
    const auto result =
        libdeflate_deflate_decompress(decompressor, input.data(), input.size(), out.data(), out.size(), &written);
    libdeflate_free_decompressor(decompressor);
    EXPECT_EQ(result, LIBDEFLATE_SUCCESS);
    out.resize(written);
    return out;
}

std::vector<std::string> payloads()
{
    std::vector<std::string> out;
    out.emplace_back("");
    out.emplace_back("a");
    out.emplace_back(1024, 'x');
    out.emplace_back("MinecraftPacket");

    std::string repetitive;
    for (int i = 0; i < 512; ++i) {
        repetitive += "minecraft:stone";
    }
    out.push_back(repetitive);

    std::mt19937 rng{1234};
    std::string noise(64 * 1024, '\0');
    for (auto &c : noise) {
        c = static_cast<char>(rng() & 0xff);
    }
    out.push_back(noise);
    return out;
}

}  // namespace

TEST(PacketCompressionTest, LibdeflateOutputIsReadableByZlib)
{
    for (const auto &payload : payloads()) {
        for (const auto level : {0, kEndstoneLevel}) {
            const auto compressed = libdeflateDeflateRaw(payload, level);
            EXPECT_EQ(zlibInflateRaw(compressed, payload.size()), payload)
                << "level " << level << ", size " << payload.size();
        }
    }
}

TEST(PacketCompressionTest, VanillaOutputIsReadableByLibdeflate)
{
    for (const auto &payload : payloads()) {
        for (const auto level : {0, kVanillaLevel}) {
            const auto compressed = zlibDeflateRaw(payload, level);
            EXPECT_EQ(libdeflateInflateRaw(compressed, payload.size()), payload)
                << "level " << level << ", size " << payload.size();
        }
    }
}

TEST(PacketCompressionTest, LibdeflateStaysCloseToVanillaRatio)
{
    for (const auto &payload : payloads()) {
        if (payload.size() < 64) {
            continue;
        }
        const auto vanilla = zlibDeflateRaw(payload, kVanillaLevel);
        const auto endstone = libdeflateDeflateRaw(payload, kEndstoneLevel);
        // Measured at 0.40% over vanilla on a 39.7 MB capture of live gameplay batches.
        EXPECT_LE(static_cast<double>(endstone.size()), static_cast<double>(vanilla.size()) * 1.02)
            << "size " << payload.size();
    }
}

namespace {

std::vector<std::string> corpus()
{
    std::vector<std::string> batches;
    const auto *path = std::getenv("ENDSTONE_PACKET_CORPUS");
    if (path != nullptr && std::filesystem::is_regular_file(path)) {
        std::ifstream in{path, std::ios::binary};
        std::uint32_t size = 0;
        while (in.read(reinterpret_cast<char *>(&size), sizeof(size))) {
            std::string batch(size, ' ');
            if (!in.read(batch.data(), size)) {
                break;
            }
            batches.push_back(std::move(batch));
        }
    }
    else if (path != nullptr) {
        for (const auto &entry : std::filesystem::directory_iterator{path}) {
            if (!entry.is_regular_file()) {
                continue;
            }
            std::ifstream in{entry.path(), std::ios::binary};
            batches.emplace_back(std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{});
        }
    }
    if (batches.empty()) {
        batches = payloads();
    }
    return batches;
}

struct Measurement {
    double seconds;
    std::size_t bytes_in;
    std::size_t bytes_out;
};

template <typename Fn>
Measurement measure(const std::vector<std::string> &batches, int repeats, Fn &&compress)
{
    Measurement m{0, 0, 0};
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < repeats; ++i) {
        for (const auto &batch : batches) {
            m.bytes_out += compress(batch).size();
            m.bytes_in += batch.size();
        }
    }
    m.seconds = std::chrono::duration<double>{std::chrono::steady_clock::now() - start}.count();
    return m;
}

void report(const char *label, const Measurement &m)
{
    const auto mib = static_cast<double>(m.bytes_in) / (1024.0 * 1024.0);
    std::cout << "  " << std::left << std::setw(18) << label << std::right << std::fixed << std::setprecision(1)
              << std::setw(9) << (mib / m.seconds) << " MiB/s"
              << "   ratio " << std::setprecision(4)
              << (static_cast<double>(m.bytes_out) / static_cast<double>(m.bytes_in)) << "   out " << m.bytes_out
              << " B" << std::endl;
}

}  // namespace

TEST(PacketCompressionTest, DISABLED_Benchmark)
{
    const auto batches = corpus();
    std::size_t total = 0;
    for (const auto &b : batches) {
        total += b.size();
    }
    const auto repeats = total == 0 ? 1 : static_cast<int>(std::max<std::size_t>(1, (64u << 20) / total));

    std::cout << batches.size() << " batches, " << total << " B, " << repeats << " repeats" << std::endl;

    auto *compressor = libdeflate_alloc_compressor(kEndstoneLevel);
    ASSERT_NE(compressor, nullptr);

    z_stream stream{};
    ASSERT_EQ(deflateInit2(&stream, kVanillaLevel, Z_DEFLATED, kRawWindowBits, 8, Z_DEFAULT_STRATEGY), Z_OK);

    std::string scratch;
    const auto vanilla = measure(batches, repeats, [&](const std::string &in) -> const std::string & {
        deflateReset(&stream);
        scratch.resize(deflateBound(&stream, in.size()));
        stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(in.data()));
        stream.avail_in = static_cast<uInt>(in.size());
        stream.next_out = reinterpret_cast<Bytef *>(scratch.data());
        stream.avail_out = static_cast<uInt>(scratch.size());
        EXPECT_EQ(deflate(&stream, Z_FINISH), Z_STREAM_END);
        scratch.resize(stream.total_out);
        return scratch;
    });

    const auto endstone = measure(batches, repeats, [&](const std::string &in) -> const std::string & {
        scratch.resize(libdeflate_deflate_compress_bound(compressor, in.size()));
        const auto written =
            libdeflate_deflate_compress(compressor, in.data(), in.size(), scratch.data(), scratch.size());
        scratch.resize(written);
        return scratch;
    });

    deflateEnd(&stream);
    libdeflate_free_compressor(compressor);

    const auto fresh = measure(batches, repeats, [&](const std::string &in) -> const std::string & {
        scratch = zlibDeflateRaw(in, kVanillaLevel);
        return scratch;
    });

    std::cout << "libdeflate level sweep:" << std::endl;
    for (const auto level : {1, 4, 6, 8, 9, 10, 12}) {
        auto *c = libdeflate_alloc_compressor(level);
        ASSERT_NE(c, nullptr);
        const auto m = measure(batches, repeats, [&](const std::string &in) -> const std::string & {
            scratch.resize(libdeflate_deflate_compress_bound(c, in.size()));
            scratch.resize(libdeflate_deflate_compress(c, in.data(), in.size(), scratch.data(), scratch.size()));
            return scratch;
        });
        libdeflate_free_compressor(c);
        report(("libdeflate -" + std::to_string(level)).c_str(), m);
    }

    std::cout << "compress:" << std::endl;
    report("zlib -7 fresh", fresh);
    report("zlib -7 reused", vanilla);
    report("libdeflate -6", endstone);
    std::cout << "  vs vanilla (fresh stream per batch, as BDS does): " << std::fixed << std::setprecision(2)
              << (fresh.seconds / endstone.seconds) << "x" << std::endl;
    std::cout << "  vs zlib with a reused stream:                     " << std::fixed << std::setprecision(2)
              << (vanilla.seconds / endstone.seconds) << "x" << std::endl;
}
