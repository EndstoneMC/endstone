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

#include "endstone/core/packs/endstone_pack_source.h"

#include <utility>

#include <boost/algorithm/string/trim.hpp>

#include "endstone/core/server.h"

namespace fs = std::filesystem;

namespace endstone::core {

EndstonePackSource::EndstonePackSource(EndstonePackSourceOptions options)
    : PackSource(std::move(options.base)), path_(std::move(options.path)), pack_type_(options.pack_type)
{
}

PackOrigin EndstonePackSource::getPackOrigin() const
{
    return PackOrigin::User;
}

PackType EndstonePackSource::getPackType() const
{
    return pack_type_;
}

PackSourceLoadResult EndstonePackSource::_loadImpl(PackSourceLoadOptions &&options)
{
    if (discovered_) {
        return {};
    }

    auto &server = entt::locator<EndstoneServer>::value_or();
    server.getLogger().info("Loading packs from '{}' ({})", path_, magic_enum::enum_name(pack_type_));

    PackSourceLoadResult result;
    for (const auto &entry : fs::directory_iterator(path_)) {
        fs::path file;

        if (!is_regular_file(entry.status())) {
            continue;
        }

        file = entry.path();
        if (std::regex_search(file.filename().string(), std::regex("\\.(mcpack|zip)$", std::regex::icase))) {
            // Check if the key file exists, in which case the pack would've been encrypted
            std::string key;
            auto key_path = file.string() + ".key";
            if (std::filesystem::exists(key_path)) {
                try {
                    std::ifstream file_stream(key_path);
                    if (!file_stream.is_open()) {
                        server.getLogger().error("Could not open encryption key file: '{}'.", key_path);
                        continue;
                    }

                    std::ostringstream ss;
                    ss << file_stream.rdbuf();
                    key = ss.str();
                    boost::algorithm::trim(key);

                    if (key.length() != 32) {
                        server.getLogger().error("Could not open encryption key file: '{}'. Invalid encryption key "
                                                 "length, must be exactly 32 bytes.",
                                                 key_path);
                        continue;
                    }
                }
                catch (const std::exception &e) {
                    server.getLogger().error("Could not open encryption key file:  '{}'. {}.", key_path, e.what());
                    continue;
                }
            }

            const auto file_location = ResourceLocation(Core::Path(file.string()), ResourceFileSystem::Raw);
            auto pack = Pack::createPack(*io_, file_location, getPackType(), getPackOrigin(), *options.manifest_factory,
                                         options.key_provider, &result.report, Core::Path::EMPTY);
            if (!pack) {
                server.getLogger().error("Could not load pack from '{}':",
                                         file_location.getRelativePath().getContainer());
                continue;
            }

            if (!key.empty()) {
                content_keys_[pack->getManifest().getIdentity()] = key;
            }
            result.packs.emplace_back(std::move(pack));
        }
    }

    for (const auto &[pack_id, report] : result.report.getReports()) {
        if (report.hasErrors()) {
            server.getLogger().error("Could not load pack from '{}':",
                                     report.getLocation().getRelativePath().getContainer());
            for (const auto &pack_error : report.getErrors()) {
                server.getLogger().error(pack_error->getLocErrorMessage());
            }
        }
    }

    discovered_ = true;
    _setPacks(PackSourcePacks(result.packs));
    _setReport(PackSourceReport(result.report));
    return result;
}

std::unordered_map<PackIdVersion, std::string> EndstonePackSource::getContentKeys() const
{
    return content_keys_;
}

}  // namespace endstone::core
