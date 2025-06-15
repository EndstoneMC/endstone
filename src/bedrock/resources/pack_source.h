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

#include "bedrock/core/resource/resource_helper.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/resources/content_key_provider.h"
#include "bedrock/resources/pack.h"
#include "bedrock/resources/pack_manifest_factory.h"

using PackCallback = std::function<void(Pack &)>;
using ConstPackCallback = std::function<void(const Pack &)>;
using PackSourcePacks = std::vector<gsl::not_null<std::shared_ptr<Pack>>>;

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

class PackSource {
public:
    virtual ~PackSource() = default;
    virtual void forEachPackConst(ConstPackCallback callback) const = 0;
    virtual void forEachPack(PackCallback callback) = 0;
    [[nodiscard]] virtual PackOrigin getPackOrigin() const = 0;
    [[nodiscard]] virtual PackType getPackType() const = 0;
    virtual PackSourceReport load(IPackManifestFactory &factory,
                                  Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> const &) = 0;
    virtual PackSourceLoadResult loadImmediate(IPackManifestFactory &,
                                               const Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> &);
    virtual void _buildSourcesForLoad(std::vector<gsl::not_null<PackSource *>> &);

protected:
    PackSource(std::unique_ptr<IPackIOProvider> io) : io_(std::move(io)) {}  // Endstone

    virtual PackSourceLoadResult _loadImpl(PackSourceLoadOptions &&) = 0;
    const gsl::not_null<std::unique_ptr<IPackIOProvider>> io_;

private:
    PackSourcePacks packs_;
    PackSourceReport report_;
};

class DirectoryPackSource : public PackSource {};
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
