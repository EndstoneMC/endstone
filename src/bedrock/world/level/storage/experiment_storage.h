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

class ExperimentStorage {
public:
    using ExperimentData = std::vector<bool>;
    [[nodiscard]] const ExperimentStorage::ExperimentData &getExperimentData() const;
    [[nodiscard]] bool isAnyToggleEnabled() const;
    [[nodiscard]] bool wereAnyExperimentsEverToggled() const;
    [[nodiscard]] bool wereAnyDeprecatedExperimentsToggled() const;
    void setAnyExperimentsEverToggled(bool);

protected:
    ExperimentStorage();
    ExperimentData experiment_data_;
    ExperimentData deprecated_data_;
    bool experiments_ever_toggled_;
};
