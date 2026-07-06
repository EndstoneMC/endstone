#include <cstdio>
#include <cstdlib>

#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <RakNetVersion.h>

int main() {
    RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
    if (peer == nullptr) {
        std::fprintf(stderr, "RakPeerInterface::GetInstance returned null\n");
        return 1;
    }

    // Bind to an OS-assigned port so the test never collides with anything else.
    RakNet::SocketDescriptor sd(0, "");
    RakNet::StartupResult sr = peer->Startup(1, &sd, 1);
    if (sr != RakNet::RAKNET_STARTED) {
        std::fprintf(stderr, "Startup failed: %d\n", static_cast<int>(sr));
        RakNet::RakPeerInterface::DestroyInstance(peer);
        return 2;
    }

    RakNet::RakNetGUID guid = peer->GetMyGUID();
    if (guid == RakNet::UNASSIGNED_RAKNET_GUID || guid.g == 0) {
        std::fprintf(stderr, "GetMyGUID returned an unassigned/zero GUID\n");
        peer->Shutdown(0);
        RakNet::RakPeerInterface::DestroyInstance(peer);
        return 3;
    }

    std::printf("raknet ok: version=%s guid=%llu\n",
                RAKNET_VERSION,
                static_cast<unsigned long long>(guid.g));

    peer->Shutdown(0);
    RakNet::RakPeerInterface::DestroyInstance(peer);
    return 0;
}
