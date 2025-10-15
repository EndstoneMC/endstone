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

#include "bedrock/resources/pack_source.h"

void PackSourceReport::addReport(PackIdVersion const &pack_id, PackReport &&report)
{
    reports_[pack_id] = std::move(report);
}

bool PackSourceReport::hasErrors() const
{
    return std::any_of(reports_.begin(), reports_.end(), [](const auto &pair) {
        const auto &[pack_id, report] = pair;
        return report.hasErrors();
    });
}

std::unordered_map<PackIdVersion, PackReport> const &PackSourceReport::getReports() const
{
    return reports_;
}

void PackSource::forEachPackShared(SharedPackCallback callback)
{
    for (const auto &pack : _getPacks()) {
        callback(pack);
    }
}

void PackSource::_buildSourcesForLoad(std::vector<gsl::not_null<PackSource *>> &out)
{
    out.emplace_back(this);
}

void PackSource::forEachPackConst(ConstPackCallback callback) const
{
    for (const auto &pack : _getPacks()) {
        callback(*pack);
    }
}

void PackSource::forEachPack(PackCallback callback)
{
    for (const auto &pack : _getPacks()) {
        callback(*pack);
    }
}

PackSourcePacks PackSource::_getPacks() const
{
    return container_->lockShared([&](const PackStorage &storage) { return storage.packs; });
}

void PackSource::_setPacks(PackSourcePacks &&packs)
{
    container_->lockShared([&](PackStorage &storage) { storage.packs = packs; });
}

void PackSource::_setReport(PackSourceReport &&report)
{
    container_->lockShared([&](PackStorage &storage) { storage.report = report; });
}

PackSource::PackSource(PackSourceOptions options)
    : io_(std::move(options.io)), container_(std::make_unique<PackStorageContainer>())
{
}

void CompositePackSource::addPackSource(PackSource *pack_source)
{
    pack_sources_.emplace_back(pack_source);
}

void CompositePackSource::clear()
{
    pack_sources_.clear();
}

std::vector<PackSource *> CompositePackSource::getPackSources()
{
    return pack_sources_;
}
