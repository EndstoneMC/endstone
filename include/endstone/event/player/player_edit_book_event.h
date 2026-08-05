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

#include <memory>
#include <stdexcept>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/meta/book_meta.h"

namespace endstone {

/**
 * Called when a player edits or signs a book.
 */
class PlayerEditBookEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerEditBookEvent);

    PlayerEditBookEvent(Player &player, int slot, const BookMeta &previous_book_meta, const BookMeta &new_book_meta,
                        bool signing)
        : Cancellable(player), slot_(slot), previous_book_meta_(cloneBookMeta(previous_book_meta)),
          new_book_meta_(cloneBookMeta(new_book_meta)), signing_(signing)
    {
    }

    /**
     * Gets the inventory slot containing the book.
     *
     * @return the inventory slot containing the book
     */
    [[nodiscard]] int getSlot() const { return slot_; }

    /**
     * Gets the metadata before the edit.
     *
     * @return the book metadata before the edit
     */
    [[nodiscard]] const BookMeta &getPreviousBookMeta() const { return *previous_book_meta_; }

    /**
     * Gets the metadata after the edit.
     *
     * @return the mutable book metadata after the edit
     */
    [[nodiscard]] BookMeta &getNewBookMeta() { return *new_book_meta_; }

    /**
     * Gets the metadata after the edit.
     *
     * @return the book metadata after the edit
     */
    [[nodiscard]] const BookMeta &getNewBookMeta() const { return *new_book_meta_; }

    /**
     * Replaces the metadata that will be applied after the edit.
     *
     * @param new_book_meta the metadata to apply after the edit
     */
    void setNewBookMeta(const BookMeta &new_book_meta) { new_book_meta_ = cloneBookMeta(new_book_meta); }

    /**
     * Gets whether the book should be signed.
     *
     * @return true if the book should be signed
     */
    [[nodiscard]] bool isSigning() const { return signing_; }

    /**
     * Sets whether the book should be signed.
     *
     * @param signing true to sign the book, false to leave it unsigned
     */
    void setSigning(bool signing) { signing_ = signing; }

private:
    [[nodiscard]] static std::unique_ptr<BookMeta> cloneBookMeta(const BookMeta &meta)
    {
        auto clone = meta.clone();
        auto *book_meta = clone->as<BookMeta>();
        if (book_meta == nullptr) {
            throw std::invalid_argument("Book metadata must implement BookMeta");
        }
        clone.release();
        return std::unique_ptr<BookMeta>(book_meta);
    }

    int slot_;
    std::unique_ptr<BookMeta> previous_book_meta_;
    std::unique_ptr<BookMeta> new_book_meta_;
    bool signing_;
};

}  // namespace endstone
