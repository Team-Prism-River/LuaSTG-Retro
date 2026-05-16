#pragma once

namespace luastg::DiscordRPC
{
    bool Init();
    bool IsInitialized();
    void RunCallbacks();
    void Shutdown();
}
