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

    PlayerEditBookEvent(const NotNull<Player> &player, int slot, const NotNull<BookMeta> &previous_book_meta,
                        const NotNull<BookMeta> &new_book_meta, bool signing)
        : Cancellable(player), slot_(slot), previous_book_meta_(previous_book_meta->clone().cast<BookMeta>()),
          new_book_meta_(new_book_meta->clone().cast<BookMeta>()), signing_(signing)
    {
    }

    /**
     * Gets the inventory slot containing the book.
     *
     * @return the inventory slot containing the book
     */
    [[nodiscard]] int getSlot() const { return slot_; }

    /**
     * Gets a copy of the metadata before the edit.
     *
     * @note This is a copy: changes made to it are not written back to the book.
     *
     * @return the book metadata before the edit
     */
    [[nodiscard]] NotNull<BookMeta> getPreviousBookMeta() const
    {
        return previous_book_meta_->clone().cast<BookMeta>();
    }

    /**
     * Gets a copy of the metadata that the player is attempting to add to the book.
     *
     * @note This is a copy: use setNewBookMeta() to change what will actually be added to the book.
     *
     * A title, an author and a generation are only written when the book is being signed. On a plain edit the book
     * stays a book and quill, which holds none of them, and they are dropped. See isSigning().
     *
     * @return the book metadata after the edit
     */
    [[nodiscard]] NotNull<BookMeta> getNewBookMeta() const { return new_book_meta_->clone().cast<BookMeta>(); }

    /**
     * Replaces the metadata that will be applied after the edit.
     *
     * @param new_book_meta the metadata to apply after the edit
     */
    void setNewBookMeta(const NotNull<BookMeta> &new_book_meta)
    {
        new_book_meta_ = new_book_meta->clone().cast<BookMeta>();
    }

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
    int slot_;
    NotNull<BookMeta> previous_book_meta_;
    NotNull<BookMeta> new_book_meta_;
    bool signing_;
};

}  // namespace endstone
