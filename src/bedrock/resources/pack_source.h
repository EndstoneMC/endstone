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

#pragma once

#include <functional>
#include <shared_mutex>

#include "bedrock/core/resource/resource_helper.h"
#include "bedrock/core/threading/lockbox.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/platform/brstd/function_ref.h"
#include "bedrock/resources/content_key_provider.h"
#include "bedrock/resources/pack.h"
#include "bedrock/resources/pack_command_pipeline.h"
#include "bedrock/resources/pack_manifest_factory.h"

using SharedPackCallback = brstd::function_ref<void(gsl::not_null<std::shared_ptr<Pack>>)>;
using PackCallback = std::function<void(Pack &)>;
using ConstPackCallback = std::function<void(const Pack &)>;
using PackSourcePacks = std::vector<gsl::not_null<std::shared_ptr<Pack>>>;

struct PackSourceOptions {
    PackSourceOptions() = default;
    PackSourceOptions(std::unique_ptr<IPackIOProvider> io) : io(std::move(io)){};
    PackSourceOptions(PackSourceOptions &&) noexcept = default;
    std::unique_ptr<IPackIOProvider> io;
};
static_assert(sizeof(PackSourceOptions) == 8);

class PackSourceReport {
public:
    PackSourceReport() = default;
    void addReport(PackIdVersion const &pack_id, PackReport &&report);
    [[nodiscard]] bool hasErrors() const;
    [[nodiscard]] std::unordered_map<PackIdVersion, PackReport> const &getReports() const;

private:
    std::unordered_map<PackIdVersion, PackReport> reports_;
};

struct PackSourceLoadResult {
    PackSourceReport report;
    PackSourcePacks packs;
};

struct PackSourceLoadOptions {
    gsl::not_null<IPackManifestFactory *> manifest_factory;
    Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> key_provider;
};

struct PackStorage {
    PackSourcePacks packs;
    PackSourceReport report;
};

class PackStorageContainer : public Bedrock::Threading::SharedLockbox<PackStorage> {};

class PackSource : public Bedrock::EnableNonOwnerReferences {
public:
    ~PackSource() override = default;
    virtual void forEachPackShared(SharedPackCallback callback);
    [[nodiscard]] virtual PackOrigin getPackOrigin() const = 0;
    [[nodiscard]] virtual PackType getPackType() const = 0;
    virtual void _buildSourcesForLoad(std::vector<gsl::not_null<PackSource *>> &);

    void forEachPackConst(ConstPackCallback callback) const;
    void forEachPack(PackCallback callback);

protected:
    PackSource(PackSourceOptions options);
    virtual PackSourceLoadResult _loadImpl(PackSourceLoadOptions &&) = 0;

    [[nodiscard]] PackSourcePacks _getPacks() const;
    void _setPacks(PackSourcePacks &&packs);
    void _setReport(PackSourceReport &&report);
    void _addPack(std::shared_ptr<Pack> pack);
    bool _removePack(brstd::function_ref<bool(const Pack &)>);

    const gsl::not_null<std::unique_ptr<IPackIOProvider>> io_;
    // gsl::not_null<std::unique_ptr<TaskGroup>> task_group_;
    std::unique_ptr<TaskGroup> task_group_;

private:
    gsl::not_null<std::unique_ptr<PackStorageContainer>> container_;
};

struct DirectoryPackSourceOptions;

class DirectoryPackSource : public PackSource {
public:
    DirectoryPackSource(DirectoryPackSourceOptions);

private:
    Core::HeapPathBuffer path_;
    const PackType pack_type_;
    const PackOrigin pack_origin_;
    bool discovered_;
    const bool is_rediscoverable_;
    const bool is_dev_directory_;
    Bedrock::NonOwnerPointer<PackCommand::IPackCommandPipeline> commands_;
};
class RealmsUnknownPackSource : public PackSource {};
class InPackagePackSource : public PackSource {};

class CompositePackSource : public PackSource {
public:
    void addPackSource(PackSource *pack_source);
    void clear();
    std::vector<PackSource *> getPackSources();

private:
    std::vector<PackSource *> pack_sources_;
};

class TreatmentPackSource : public PackSource {};
class WorldTemplatePackSource : public DirectoryPackSource {};
