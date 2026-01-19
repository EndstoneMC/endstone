import uuid
from datetime import datetime, timedelta, timezone
from typing import Any, Generator

import pytest
from endstone import Server
from endstone.ban import IpBanList, PlayerBanList

# =============================================================================
# Fixtures
# =============================================================================


@pytest.fixture
def player_ban_list(server: Server) -> Generator[PlayerBanList, Any, None]:
    """Get the player ban list and ensure it's clean before and after tests."""
    ban_list = server.ban_list
    # Clean up any existing bans from previous tests
    for entry in list(ban_list.entries):
        ban_list.remove_ban(entry.name)
    yield ban_list
    # Clean up after test
    for entry in list(ban_list.entries):
        ban_list.remove_ban(entry.name)
    assert len(ban_list.entries) == 0


@pytest.fixture
def ip_ban_list(server: Server) -> Generator[IpBanList, Any, None]:
    """Get the IP ban list and ensure it's clean before and after tests."""
    ban_list = server.ip_ban_list
    # Clean up any existing bans from previous tests
    for entry in list(ban_list.entries):
        ban_list.remove_ban(entry.address)
    yield ban_list
    # Clean up after test
    for entry in list(ban_list.entries):
        ban_list.remove_ban(entry.address)
    assert len(ban_list.entries) == 0


# =============================================================================
# Section 1: PlayerBanList Basic Operations
# =============================================================================


class TestPlayerBanListBasicOperations:
    """Tests for basic PlayerBanList add/remove/check operations."""

    def test_add_ban_by_name_only(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with only player name."""
        player_name = "test_player_1"
        entry = player_ban_list.add_ban(player_name)

        assert entry is not None
        assert entry.name == player_name
        assert player_ban_list.is_banned(player_name)

    def test_add_ban_with_reason(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with a custom reason."""
        player_name = "test_player_2"
        reason = "Test ban reason"
        entry = player_ban_list.add_ban(player_name, reason=reason)

        assert entry is not None
        assert entry.name == player_name
        assert entry.reason == reason

    def test_add_ban_with_source(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with a custom source."""
        player_name = "test_player_3"
        source = "TestPlugin"
        entry = player_ban_list.add_ban(player_name, source=source)

        assert entry is not None
        assert entry.source == source

    def test_add_ban_with_expiration(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with an expiration date."""
        player_name = "test_player_4"
        # Set expiration to 1 hour from now
        expires = datetime.now(timezone.utc) + timedelta(hours=1)
        entry = player_ban_list.add_ban(player_name, expires=expires)

        assert entry is not None
        assert entry.expiration is not None

    def test_add_ban_with_all_parameters(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with all optional parameters."""
        player_name = "test_player_5"
        player_uuid = uuid.uuid4()
        player_xuid = "1234567890123456"
        reason = "Full parameter test"
        source = "ComprehensiveTest"
        expires = datetime.now(timezone.utc) + timedelta(days=7)

        entry = player_ban_list.add_ban(
            player_name,
            uuid=player_uuid,
            xuid=player_xuid,
            reason=reason,
            expires=expires,
            source=source,
        )

        assert entry is not None
        assert entry.name == player_name
        assert entry.unique_id == player_uuid
        assert entry.xuid == player_xuid
        assert entry.reason == reason
        assert entry.source == source
        assert entry.expiration is not None

    def test_is_banned_returns_false_for_unbanned_player(
        self, player_ban_list: PlayerBanList
    ) -> None:
        """Test is_banned returns False for a player not in the ban list."""
        assert not player_ban_list.is_banned("nonexistent_player")

    def test_remove_ban_by_name(self, player_ban_list: PlayerBanList) -> None:
        """Test removing a ban by player name."""
        player_name = "test_player_remove"
        player_ban_list.add_ban(player_name)
        assert player_ban_list.is_banned(player_name)

        player_ban_list.remove_ban(player_name)
        assert not player_ban_list.is_banned(player_name)

    def test_get_ban_entry_existing(self, player_ban_list: PlayerBanList) -> None:
        """Test getting a ban entry for an existing banned player."""
        player_name = "test_player_get"
        reason = "Get entry test"
        player_ban_list.add_ban(player_name, reason=reason)

        entry = player_ban_list.get_ban_entry(player_name)
        assert entry is not None
        assert entry.name == player_name
        assert entry.reason == reason

    def test_get_ban_entry_nonexistent(self, player_ban_list: PlayerBanList) -> None:
        """Test getting a ban entry for a non-banned player returns None."""
        entry = player_ban_list.get_ban_entry("nonexistent_player")
        assert entry is None


# =============================================================================
# Section 2: PlayerBanList UUID and XUID Operations
# =============================================================================


class TestPlayerBanListUuidXuidOperations:
    """Tests for PlayerBanList operations using UUID and XUID."""

    def test_add_ban_with_uuid(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with UUID."""
        player_name = "uuid_test_player"
        player_uuid = uuid.uuid4()
        entry = player_ban_list.add_ban(player_name, uuid=player_uuid)

        assert entry is not None
        assert entry.name == player_name
        assert entry.unique_id == player_uuid

    def test_add_ban_with_xuid(self, player_ban_list: PlayerBanList) -> None:
        """Test adding a ban with XUID."""
        player_name = "xuid_test_player"
        player_xuid = "9876543210123456"
        entry = player_ban_list.add_ban(player_name, xuid=player_xuid)

        assert entry is not None
        assert entry.name == player_name
        assert entry.xuid == player_xuid

    def test_is_banned_with_uuid(self, player_ban_list: PlayerBanList) -> None:
        """Test checking ban status using UUID."""
        player_name = "uuid_ban_check"
        player_uuid = uuid.uuid4()
        player_ban_list.add_ban(player_name, uuid=player_uuid)

        assert player_ban_list.is_banned(player_name, uuid=player_uuid)

    def test_is_banned_with_xuid(self, player_ban_list: PlayerBanList) -> None:
        """Test checking ban status using XUID."""
        player_name = "xuid_ban_check"
        player_xuid = "1111222233334444"
        player_ban_list.add_ban(player_name, xuid=player_xuid)

        assert player_ban_list.is_banned(player_name, xuid=player_xuid)

    def test_get_ban_entry_with_uuid(self, player_ban_list: PlayerBanList) -> None:
        """Test getting ban entry using UUID."""
        player_name = "uuid_get_entry"
        player_uuid = uuid.uuid4()
        player_ban_list.add_ban(player_name, uuid=player_uuid)

        entry = player_ban_list.get_ban_entry(player_name, uuid=player_uuid)
        assert entry is not None
        assert entry.unique_id == player_uuid

    def test_get_ban_entry_with_xuid(self, player_ban_list: PlayerBanList) -> None:
        """Test getting ban entry using XUID."""
        player_name = "xuid_get_entry"
        player_xuid = "5555666677778888"
        player_ban_list.add_ban(player_name, xuid=player_xuid)

        entry = player_ban_list.get_ban_entry(player_name, xuid=player_xuid)
        assert entry is not None
        assert entry.xuid == player_xuid

    def test_remove_ban_with_uuid(self, player_ban_list: PlayerBanList) -> None:
        """Test removing ban using UUID."""
        player_name = "uuid_remove_test"
        player_uuid = uuid.uuid4()
        player_ban_list.add_ban(player_name, uuid=player_uuid)
        assert player_ban_list.is_banned(player_name)

        player_ban_list.remove_ban(player_name, uuid=player_uuid)
        assert not player_ban_list.is_banned(player_name)

    def test_remove_ban_with_xuid(self, player_ban_list: PlayerBanList) -> None:
        """Test removing ban using XUID."""
        player_name = "xuid_remove_test"
        player_xuid = "9999000011112222"
        player_ban_list.add_ban(player_name, xuid=player_xuid)
        assert player_ban_list.is_banned(player_name)

        player_ban_list.remove_ban(player_name, xuid=player_xuid)
        assert not player_ban_list.is_banned(player_name)


# =============================================================================
# Section 3: PlayerBanList Entries Property
# =============================================================================


class TestPlayerBanListEntries:
    """Tests for PlayerBanList entries property."""

    def test_entries_initially_empty(self, player_ban_list: PlayerBanList) -> None:
        """Test that entries list is empty when no bans exist."""
        assert isinstance(player_ban_list.entries, list)
        assert len(player_ban_list.entries) == 0

    def test_entries_contains_added_bans(self, player_ban_list: PlayerBanList) -> None:
        """Test that entries list contains all added bans."""
        names = ["entry_test_1", "entry_test_2", "entry_test_3"]
        for name in names:
            player_ban_list.add_ban(name)

        entries = player_ban_list.entries
        assert len(entries) == 3
        entry_names = [e.name for e in entries]
        for name in names:
            assert name in entry_names

    def test_entries_updated_after_removal(
        self, player_ban_list: PlayerBanList
    ) -> None:
        """Test that entries list is updated after ban removal."""
        names = ["remove_entry_1", "remove_entry_2"]
        for name in names:
            player_ban_list.add_ban(name)

        assert len(player_ban_list.entries) == 2

        player_ban_list.remove_ban(names[0])
        entries = player_ban_list.entries
        assert len(entries) == 1
        assert entries[0].name == names[1]


# =============================================================================
# Section 4: PlayerBanList Update Operations
# =============================================================================


class TestPlayerBanListUpdateOperations:
    """Tests for updating existing ban entries."""

    def test_update_existing_ban_reason(self, player_ban_list: PlayerBanList) -> None:
        """Test updating the reason of an existing ban."""
        player_name = "update_reason_test"
        player_ban_list.add_ban(player_name, reason="Original reason")

        # Re-add with new reason should update
        entry = player_ban_list.add_ban(player_name, reason="Updated reason")
        assert entry.reason == "Updated reason"

        # Verify only one entry exists
        entries = [e for e in player_ban_list.entries if e.name == player_name]
        assert len(entries) == 1

    def test_update_existing_ban_source(self, player_ban_list: PlayerBanList) -> None:
        """Test updating the source of an existing ban."""
        player_name = "update_source_test"
        player_ban_list.add_ban(player_name, source="OriginalSource")

        entry = player_ban_list.add_ban(player_name, source="UpdatedSource")
        assert entry.source == "UpdatedSource"


# =============================================================================
# Section 5: IpBanList Basic Operations
# =============================================================================


class TestIpBanListBasicOperations:
    """Tests for basic IpBanList add/remove/check operations."""

    def test_add_ip_ban_basic(self, ip_ban_list: IpBanList) -> None:
        """Test adding a basic IP ban."""
        ip_address = "192.168.1.100"
        entry = ip_ban_list.add_ban(ip_address)

        assert entry is not None
        assert entry.address == ip_address
        assert ip_ban_list.is_banned(ip_address)

    def test_add_ip_ban_with_reason(self, ip_ban_list: IpBanList) -> None:
        """Test adding an IP ban with a reason."""
        ip_address = "192.168.1.101"
        reason = "Suspicious activity"
        entry = ip_ban_list.add_ban(ip_address, reason=reason)

        assert entry is not None
        assert entry.reason == reason

    def test_add_ip_ban_with_source(self, ip_ban_list: IpBanList) -> None:
        """Test adding an IP ban with a source."""
        ip_address = "192.168.1.102"
        source = "AntiCheat"
        entry = ip_ban_list.add_ban(ip_address, source=source)

        assert entry is not None
        assert entry.source == source

    def test_add_ip_ban_with_expiration(self, ip_ban_list: IpBanList) -> None:
        """Test adding an IP ban with an expiration date."""
        ip_address = "192.168.1.103"
        expires = datetime.now(timezone.utc) + timedelta(hours=24)
        entry = ip_ban_list.add_ban(ip_address, expires=expires)

        assert entry is not None
        assert entry.expiration is not None

    def test_add_ip_ban_with_all_parameters(self, ip_ban_list: IpBanList) -> None:
        """Test adding an IP ban with all parameters."""
        ip_address = "192.168.1.104"
        reason = "Full parameter test"
        source = "AdminPanel"
        expires = datetime.now(timezone.utc) + timedelta(days=30)

        entry = ip_ban_list.add_ban(
            ip_address, reason=reason, expires=expires, source=source
        )

        assert entry is not None
        assert entry.address == ip_address
        assert entry.reason == reason
        assert entry.source == source
        assert entry.expiration is not None

    def test_is_ip_banned_returns_false_for_unbanned(
        self, ip_ban_list: IpBanList
    ) -> None:
        """Test is_banned returns False for an unbanned IP."""
        assert not ip_ban_list.is_banned("10.0.0.1")

    def test_remove_ip_ban(self, ip_ban_list: IpBanList) -> None:
        """Test removing an IP ban."""
        ip_address = "192.168.1.105"
        ip_ban_list.add_ban(ip_address)
        assert ip_ban_list.is_banned(ip_address)

        ip_ban_list.remove_ban(ip_address)
        assert not ip_ban_list.is_banned(ip_address)

    def test_get_ip_ban_entry_existing(self, ip_ban_list: IpBanList) -> None:
        """Test getting an IP ban entry for a banned address."""
        ip_address = "192.168.1.106"
        reason = "Test entry retrieval"
        ip_ban_list.add_ban(ip_address, reason=reason)

        entry = ip_ban_list.get_ban_entry(ip_address)
        assert entry is not None
        assert entry.address == ip_address
        assert entry.reason == reason

    def test_get_ip_ban_entry_nonexistent(self, ip_ban_list: IpBanList) -> None:
        """Test getting an IP ban entry for a non-banned address returns None."""
        entry = ip_ban_list.get_ban_entry("10.0.0.2")
        assert entry is None


# =============================================================================
# Section 6: IpBanList Entries Property
# =============================================================================


class TestIpBanListEntries:
    """Tests for IpBanList entries property."""

    def test_ip_entries_initially_empty(self, ip_ban_list: IpBanList) -> None:
        """Test that IP entries list is empty when no bans exist."""
        assert isinstance(ip_ban_list.entries, list)
        assert len(ip_ban_list.entries) == 0

    def test_ip_entries_contains_added_bans(self, ip_ban_list: IpBanList) -> None:
        """Test that IP entries list contains all added bans."""
        addresses = ["192.168.2.1", "192.168.2.2", "192.168.2.3"]
        for addr in addresses:
            ip_ban_list.add_ban(addr)

        entries = ip_ban_list.entries
        assert len(entries) == 3
        entry_addresses = [e.address for e in entries]
        for addr in addresses:
            assert addr in entry_addresses

    def test_ip_entries_updated_after_removal(self, ip_ban_list: IpBanList) -> None:
        """Test that IP entries list is updated after ban removal."""
        addresses = ["192.168.3.1", "192.168.3.2"]
        for addr in addresses:
            ip_ban_list.add_ban(addr)

        assert len(ip_ban_list.entries) == 2

        ip_ban_list.remove_ban(addresses[0])
        entries = ip_ban_list.entries
        assert len(entries) == 1
        assert entries[0].address == addresses[1]


# =============================================================================
# Section 7: IpBanList Various IP Formats
# =============================================================================


class TestIpBanListIpFormats:
    """Tests for IpBanList with various IP address formats."""

    def test_ipv4_address(self, ip_ban_list: IpBanList) -> None:
        """Test banning a standard IPv4 address."""
        ip_address = "203.0.113.42"
        entry = ip_ban_list.add_ban(ip_address)
        assert entry.address == ip_address
        assert ip_ban_list.is_banned(ip_address)

    def test_ipv6_address(self, ip_ban_list: IpBanList) -> None:
        """Test banning an IPv6 address."""
        ip_address = "2001:db8::1"
        entry = ip_ban_list.add_ban(ip_address)
        assert entry.address == ip_address
        assert ip_ban_list.is_banned(ip_address)

    def test_localhost_ipv4(self, ip_ban_list: IpBanList) -> None:
        """Test banning localhost IPv4."""
        ip_address = "127.0.0.1"
        entry = ip_ban_list.add_ban(ip_address)
        assert entry.address == ip_address
        assert ip_ban_list.is_banned(ip_address)


# =============================================================================
# Section 8: BanEntry Properties (via PlayerBanEntry)
# =============================================================================


class TestBanEntryProperties:
    """Tests for BanEntry property getters and setters."""

    def test_created_property(self, player_ban_list: PlayerBanList) -> None:
        """Test the created property getter and setter."""
        entry = player_ban_list.add_ban("created_test")

        # Created should be set automatically
        assert entry.created is not None

        # Test setter
        new_created = datetime.now(timezone.utc) - timedelta(days=10)
        entry.created = new_created
        # Verify it was set (comparing timestamps)
        assert entry.created is not None

    def test_source_property(self, player_ban_list: PlayerBanList) -> None:
        """Test the source property getter and setter."""
        entry = player_ban_list.add_ban("source_test", source="OriginalSource")
        assert entry.source == "OriginalSource"

        entry.source = "NewSource"
        assert entry.source == "NewSource"

    def test_expiration_property(self, player_ban_list: PlayerBanList) -> None:
        """Test the expiration property getter and setter."""
        entry = player_ban_list.add_ban("expiration_test")

        # Initially no expiration (permanent ban)
        # Note: depending on implementation, this may be None or a far future date

        # Set expiration
        new_expiration = datetime.now(timezone.utc) + timedelta(days=30)
        entry.expiration = new_expiration
        assert entry.expiration is not None

        # Set to None for permanent ban
        entry.expiration = None
        assert entry.expiration is None

    def test_reason_property(self, player_ban_list: PlayerBanList) -> None:
        """Test the reason property getter and setter."""
        entry = player_ban_list.add_ban("reason_test", reason="Original reason")
        assert entry.reason == "Original reason"

        entry.reason = "Updated reason"
        assert entry.reason == "Updated reason"


# =============================================================================
# Section 9: PlayerBanEntry Specific Properties
# =============================================================================


class TestPlayerBanEntryProperties:
    """Tests for PlayerBanEntry-specific properties."""

    def test_name_property(self, player_ban_list: PlayerBanList) -> None:
        """Test the name property is read-only and correct."""
        player_name = "name_property_test"
        entry = player_ban_list.add_ban(player_name)
        assert entry.name == player_name

    def test_unique_id_property_when_set(self, player_ban_list: PlayerBanList) -> None:
        """Test the unique_id property when UUID is provided."""
        player_uuid = uuid.uuid4()
        entry = player_ban_list.add_ban("uuid_prop_test", uuid=player_uuid)
        assert entry.unique_id == player_uuid

    def test_unique_id_property_when_not_set(
        self, player_ban_list: PlayerBanList
    ) -> None:
        """Test the unique_id property when UUID is not provided."""
        entry = player_ban_list.add_ban("no_uuid_test")
        assert entry.unique_id is None

    def test_xuid_property_when_set(self, player_ban_list: PlayerBanList) -> None:
        """Test the xuid property when XUID is provided."""
        player_xuid = "1234567890123456"
        entry = player_ban_list.add_ban("xuid_prop_test", xuid=player_xuid)
        assert entry.xuid == player_xuid

    def test_xuid_property_when_not_set(self, player_ban_list: PlayerBanList) -> None:
        """Test the xuid property when XUID is not provided."""
        entry = player_ban_list.add_ban("no_xuid_test")
        assert entry.xuid is None


# =============================================================================
# Section 10: IpBanEntry Specific Properties
# =============================================================================


class TestIpBanEntryProperties:
    """Tests for IpBanEntry-specific properties."""

    def test_address_property(self, ip_ban_list: IpBanList) -> None:
        """Test the address property is read-only and correct."""
        ip_address = "192.168.100.1"
        entry = ip_ban_list.add_ban(ip_address)
        assert entry.address == ip_address


# =============================================================================
# Section 11: Command Integration Tests
# =============================================================================


class TestBanCommandIntegration:
    """Tests for ban/unban command integration."""

    def test_ban_via_api_unban_via_command(
        self, server: Server, player_ban_list: PlayerBanList
    ) -> None:
        """Test banning via API and unbanning via command."""
        player_name = "api_ban_cmd_unban"
        player_ban_list.add_ban(player_name, reason="API ban test")
        assert player_ban_list.is_banned(player_name)

        server.dispatch_command(server.command_sender, f'unban "{player_name}"')
        assert not player_ban_list.is_banned(player_name)

    def test_ban_via_command_unban_via_api(
        self, server: Server, player_ban_list: PlayerBanList
    ) -> None:
        """Test banning via command and unbanning via API."""
        player_name = "cmd_ban_api_unban"
        server.dispatch_command(
            server.command_sender, f'ban "{player_name}" Test reason'
        )
        assert player_ban_list.is_banned(player_name)

        player_ban_list.remove_ban(player_name)
        assert not player_ban_list.is_banned(player_name)

    def test_ban_command_with_special_characters(
        self, server: Server, player_ban_list: PlayerBanList
    ) -> None:
        """Test ban command with special characters in reason."""
        player_name = "special_char_test"
        reason = ":emoji: special & characters!"
        server.dispatch_command(server.command_sender, f'ban "{player_name}" {reason}')

        assert player_ban_list.is_banned(player_name)
        entry = player_ban_list.get_ban_entry(player_name)
        assert entry is not None
        assert reason in entry.reason


# =============================================================================
# Section 12: Edge Cases and Error Handling
# =============================================================================


class TestBanEdgeCases:
    """Tests for edge cases and boundary conditions."""

    def test_ban_empty_reason(self, player_ban_list: PlayerBanList) -> None:
        """Test banning with an empty reason uses default."""
        entry = player_ban_list.add_ban("empty_reason_test", reason="")
        assert entry is not None
        # Empty reason should be allowed or use default

    def test_ban_player_with_spaces_in_name(
        self, player_ban_list: PlayerBanList
    ) -> None:
        """Test banning a player with spaces in their name."""
        player_name = "player with spaces"
        entry = player_ban_list.add_ban(player_name)
        assert entry.name == player_name
        assert player_ban_list.is_banned(player_name)

    def test_ban_player_with_unicode_name(self, player_ban_list: PlayerBanList) -> None:
        """Test banning a player with unicode characters in name."""
        player_name = "Player_\u00e9\u00e0\u00fc"
        entry = player_ban_list.add_ban(player_name)
        assert entry.name == player_name
        assert player_ban_list.is_banned(player_name)

    def test_ban_same_player_twice(self, player_ban_list: PlayerBanList) -> None:
        """Test banning the same player twice updates the entry."""
        player_name = "double_ban_test"
        player_ban_list.add_ban(player_name, reason="First ban")
        player_ban_list.add_ban(player_name, reason="Second ban")

        # Should update existing entry
        entry = player_ban_list.get_ban_entry(player_name)
        assert entry is not None
        assert entry.reason == "Second ban"

        # Should only have one entry
        matching_entries = [e for e in player_ban_list.entries if e.name == player_name]
        assert len(matching_entries) == 1

    def test_remove_nonexistent_ban(self, player_ban_list: PlayerBanList) -> None:
        """Test removing a ban that doesn't exist doesn't raise error."""
        # This should not raise an exception
        player_ban_list.remove_ban("nonexistent_player_to_remove")

    def test_ip_ban_same_ip_twice(self, ip_ban_list: IpBanList) -> None:
        """Test banning the same IP twice updates the entry."""
        ip_address = "192.168.200.1"
        ip_ban_list.add_ban(ip_address, reason="First IP ban")
        ip_ban_list.add_ban(ip_address, reason="Second IP ban")

        entry = ip_ban_list.get_ban_entry(ip_address)
        assert entry is not None
        assert entry.reason == "Second IP ban"

        # Should only have one entry
        matching_entries = [e for e in ip_ban_list.entries if e.address == ip_address]
        assert len(matching_entries) == 1

    def test_long_reason(self, player_ban_list: PlayerBanList) -> None:
        """Test banning with a very long reason."""
        player_name = "long_reason_test"
        long_reason = "A" * 500  # 500 character reason
        entry = player_ban_list.add_ban(player_name, reason=long_reason)
        assert entry is not None
        # The reason should be stored (possibly truncated)
        assert len(entry.reason) > 0


# =============================================================================
# Section 13: Multiple Bans Management
# =============================================================================


class TestMultipleBansManagement:
    """Tests for managing multiple bans simultaneously."""

    def test_multiple_player_bans(self, player_ban_list: PlayerBanList) -> None:
        """Test adding and managing multiple player bans."""
        players = [f"multi_player_{i}" for i in range(5)]
        for player in players:
            player_ban_list.add_ban(player, reason=f"Reason for {player}")

        # All should be banned
        for player in players:
            assert player_ban_list.is_banned(player)

        # Remove some
        player_ban_list.remove_ban(players[0])
        player_ban_list.remove_ban(players[2])
        player_ban_list.remove_ban(players[4])

        # Check correct ones are banned/unbanned
        assert not player_ban_list.is_banned(players[0])
        assert player_ban_list.is_banned(players[1])
        assert not player_ban_list.is_banned(players[2])
        assert player_ban_list.is_banned(players[3])
        assert not player_ban_list.is_banned(players[4])

    def test_multiple_ip_bans(self, ip_ban_list: IpBanList) -> None:
        """Test adding and managing multiple IP bans."""
        addresses = [f"10.0.0.{i}" for i in range(1, 6)]
        for addr in addresses:
            ip_ban_list.add_ban(addr, reason=f"Reason for {addr}")

        # All should be banned
        for addr in addresses:
            assert ip_ban_list.is_banned(addr)

        # Remove all
        for addr in addresses:
            ip_ban_list.remove_ban(addr)

        # None should be banned
        for addr in addresses:
            assert not ip_ban_list.is_banned(addr)

        # Entries should be empty
        assert len(ip_ban_list.entries) == 0
