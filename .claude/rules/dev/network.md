---
paths:
  - "src/bedrock/network/**"
  - "src/bedrock/deps/nethernet/**"
  - "src/endstone/core/network/**"
  - "src/endstone/runtime/bedrock_hooks/network_system.cpp"
  - "src/endstone/runtime/bedrock_hooks/packet.cpp"
  - "src/endstone/runtime/bedrock_hooks/rak_peer_helper.cpp"
  - "src/endstone/runtime/bedrock_hooks/lan_thread_manager.cpp"
  - "src/endstone/runtime/bedrock_hooks/nethernet_transport.cpp"
  - "src/endstone/runtime/bedrock_hooks/p2p_transport_channel.cpp"
  - "src/endstone/runtime/bedrock_hooks/simple_network_interface_impl.cpp"
---

# Bedrock Network

## Peer chain

- The per-connection peer chain is Batched -> Compressed -> [Encrypted] -> transport leaf, with a PacketTrace layer on top on Windows only.
- Attach per-connection state to `connections_.back()` after the original `onNewIncomingConnection` returns true; never look it up by `NetworkIdentifier`.
- Replace a named peer layer (Compressed, Batched, Encrypted) by deriving from the BDS class with identical layout and splicing it in; never replace `connection.peer` wholesale; BDS's own code for a spliced layer never runs.
- After a splice, repoint BDS's `compressed_peer` and `batched_peer` weak_ptrs and the BDS Batched's inner `peer_`.
- Never hook `Core::Compression::Zlib::compress` or `decompress` to change the codec; replace the peer layer.
- On every bump re-check the packet size asserts, the compression-enable store after NetworkSettings, the async-flush flag store in `enableAsyncFlush` and the chain order, and diff decompiled `sendPacket`, `_receivePacket` and flush against the reimplementations, mirroring any BDS change by hand.

## Packet send and receive

- Hold plugin-originated sends until PlayStatus(LoginSuccess) at the earliest.
- Copy the `data` string before firing `PacketSendEvent`; it aliases `NetworkSystem::mSendStream` and a nested send overwrites it.
- Build packets with `MinecraftPackets::createPacket`, assign the typed payload and send with `sendNetworkPacket`; never touch raw wire bytes.
- Hand-write a packet's wire bytes only where a faithful payload would mean reconstructing a whole subsystem, as `EndstonePlayer::spawnParticle` does for Molang, and name that subsystem.
- Every packet a plugin or proxy synthesises must satisfy the virtual `Packet::isValid()`; a client that decodes one that fails raises BadPacket (90) with no Disconnect; check `isValid` overrides before the wire format.
- Send `MovePlayerPacket` Teleport mode with `teleport_data` and every other mode without it.
- The packet header word is `id | recipientSubId << 10 | senderSubId << 12`.
- Take wire shape (varint vs fixed width, optional prefix, field presence) from the protocol references, never from the headers; headers settle only naming and field order.
- A cereal name-coded enum is written as its verbatim C++ member name, with case per enum; cereal struct serialization is positional.
- A cerealised variant entry carries the discriminant as `uvarint32` then the verbatim name.
- Every packet from protocol 2168 on is cerealised.

## Client behaviour

- A BossEvent `Add` is dropped unless `boss_id` is a live actor the client knows; first Add per actor id wins and `player_id` is unread; after a dimension change answer the client's `Query`.
- A client keeps chunks through three views (player, subscriber from `NetworkChunkPublisherUpdatePacket`, spawn); a publisher update alone does not unload chunks around the player; send it before the chunks it covers.
- Key map decorations by actor id, sent as a vector parallel to the decorations, not by position.
- Two separately serialized vectors can be parallel arrays the producer fills together; rewrite both.
- Behind a proxy, fill `SelfSignedId` with the player UUID or use a trusted token so MsaId is set; BDS writes no save when every id is empty.

## RakNet

- Patch RakNet parsing vulnerabilities in the incoming-datagram handler installed through `SetIncomingDatagramEventHandler` (return false to drop).
- Bound ACK and NAK ranges, which are 24-bit wrapping datagram numbers.
- Bound client input that reaches an Endstone reimplementation as BDS bounds it, such as NBT nesting depth and certificate chain sizes.
- Keep the RakNet hooks, datagram patches and `network.ipv6` while BDS still accepts `transport=raknet`; add NetherNet features alongside the RakNet path.
- Call the non-virtual `RakPeer::GetOfflinePingResponse` from the recipe's `RakPeer`, whose member layout must match Mojang's up to the locked mutex.
- Close connections with `NetworkSystem::setCloseConnection`, never `closeConnection`, which runs `onConnectionClosed` a second time over freed objects.

## Transport

- Determine the transport from the NetworkSession's `mTransportLayer`, not the connector.
- Never `static_cast` the remote connector or the innermost peer to RakNet types; under NetherNet they are `ServerNetherNetConnector` and a WebRTC peer.
- `NetworkIdentifier::getAddress` returns an empty string for NetherNet and RakNet ids; derive the address from the peer GUID (RakNet) or network id (NetherNet).
- Read ping from NetworkStatus `current_ping`, which is one-way milliseconds under NetherNet; never `average_ping`.
- `server.properties` `transport` accepts `raknet` and `nethernet`, parsed by FNV-1a hash of the value.
- On every bump re-check the `SendLanBroadcastResponse` `data` type, the `createTransportInterface` and `Port::SendBindingErrorResponse` patterns, whether BDS still links RakNet, `server-udp-ports` sharing, the `RakPeer` offline-ping layout and `isValid` on synthesised packets.

## NetherNet

- Signaling is one-shot HTTP: `GET /v1/join` and `POST /v1/join/{networkId}` only; the answer carries the full SDP and later signals are dropped.
- Never add real STUN servers to `RTCConfiguration::servers`.
- The server-side no-trickle `createAnswer` clears `servers`, not `static_stun_servers`.
- Never infer NetherNet ICE behaviour from upstream libwebrtc; Mojang's fork is namespaced `webrtc::` and adds a cross-session shared socket and `StaticStunServer`.
- Every answer carries the server identity assertion (`a=identity`, ES384 JWT with `cpk`); an offer without `a=identity` is rejected.
- The verifier checks only the token and the fingerprints JWS signed by `cpk`; self-signed login (AuthenticationType 2) needs online-mode=false.
- A NetherNet client's identity `cpk` must equal the key used in the login token; a mismatch gets Disconnect 46 (NotAuthenticated) after Login with no server log.
- The server identity key is `keys/server_identity_key.pem` (P-384) and signs NetherNet assertions and the login ServerToClientHandshake JWT; when absent BDS generates an in-memory key per start and only `/serveridentity save` persists it.
- iOS clients open TCP and send a TLS ClientHello, so put a TLS terminator in front of the signaling port; media still goes direct over UDP.
- Keep the CLI signaling-port pre-flight check in `endstone/cli/base.py`; BDS logs "Server started" with no listener when the signaling bind fails.
- On Linux a stopped server's FIN_WAIT sockets block rebinding of the signaling port for about 60 s; wait for the port before restarting.
- Call virtuals through the object instead of hooking them, such as `INetherNetTransportInterface::SetRelayConfig`, and apply such calls in `setLevel`, never from `EndstoneServer::init`.
- Hook `NetherNet::LanThreadManager::SendLanBroadcastResponse` for the LAN ping payload.
- Under `transport=nethernet` the only ping is LAN discovery; `HttpSignalingServer` carries no MOTD or event.
