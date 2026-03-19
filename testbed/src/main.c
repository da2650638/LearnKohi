#include <core/logger.h>
#include <core/asserts.h>

// TODO: test
#include <platform/platform.h>

int main() {
    KFATAL("fatal %d", 1);
    KERROR("error %s", "E");
    KWARN("warn %d", 2);
    KINFO("info %d", 3);
    KDEBUG("debug %d", 4);
    KTRACE("trace %d", 5);

    KASSERT(1 == 1);

    platform_state state;
    if(platform_startup(&state, "Kohi Platform Layer Test", 100, 100, 800, 600))
    {
        while(TRUE)
        {
            platform_pump_messages(&state);
        }
    }
    platform_shutdown(&state);
    return 0;
}