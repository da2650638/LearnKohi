#include <core/logger.h>
#include <core/asserts.h>

int main() {
    KFATAL("fatal %d", 1);
    KERROR("error %s", "E");
    KWARN("warn %d", 2);
    KINFO("info %d", 3);
    KDEBUG("debug %d", 4);
    KTRACE("trace %d", 5);

    KASSERT(1 == 1);
    return 0;
}