#include "platform.h"

#if KPLATFORM_LINUX == 1

#include "core/logger.h"

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>  // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>  // sudo apt-get install libxkbcommon-x11-dev
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>  // nanosleep
#else
#include <unistd.h>  // usleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct internal_state {
    Display* display;
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;
} internal_state;

b8 platform_startup(
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
    // Create the internal state.
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state* state = (internal_state*)plat_state->internal_state;

    // Connect to X
    state->display = XOpenDisplay(NULL);

    // Turn off key repeats.
    XAutoRepeatOff(state->display);

    // Retrieve the connection from the display.
    state->connection = XGetXCBConnection(state->display);

    if (xcb_connection_has_error(state->connection)) {
        KFATAL("Failed to connect to X server via XCB.");
        return FALSE;
    }

    // Get data from the X server
    const struct xcb_setup_t* setup = xcb_get_setup(state->connection);

    // Loop through screens using iterator
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    int screen_p = 0;
    for (i32 s = screen_p; s > 0; s--) {
        xcb_screen_next(&it);
    }

    // After screens have been looped through, assign it.
    state->screen = it.data;

    // Allocate a XID for the window to be created.
    state->window = xcb_generate_id(state->connection);

    // Register event types.
    // XCB_CW_BACK_PIXEL = filling then window bg with a single colour
    // XCB_CW_EVENT_MASK is required.
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    // Listen for keyboard and mouse buttons
    u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                       XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    // Values to be sent over XCB (bg colour, events)
    u32 value_list[] = {state->screen->black_pixel, event_values};

    // Create the window
    xcb_void_cookie_t cookie = xcb_create_window(
        state->connection,
        XCB_COPY_FROM_PARENT,  // depth
        state->window,
        state->screen->root,            // parent
        x,                              //x
        y,                              //y
        width,                          //width
        height,                         //height
        0,                              // No border
        XCB_WINDOW_CLASS_INPUT_OUTPUT,  //class
        state->screen->root_visual,
        event_mask,
        value_list);

    // Change the title
    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,  // data should be viewed 8 bits at a time
        strlen(application_name),
        application_name);

    // Tell the server to notify when the window manager
    // attempts to destroy the window.
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
        state->connection,
        0,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW");
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
        state->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
        state->connection,
        wm_delete_cookie,
        NULL);
    xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
        state->connection,
        wm_protocols_cookie,
        NULL);
    state->wm_delete_win = wm_delete_reply->atom;
    state->wm_protocols = wm_protocols_reply->atom;

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        wm_protocols_reply->atom,
        4,
        32,
        1,
        &wm_delete_reply->atom);

    // Map the window to the screen
    xcb_map_window(state->connection, state->window);

    // Flush the stream
    i32 stream_result = xcb_flush(state->connection);
    if (stream_result <= 0) {
        KFATAL("An error occurred when flusing the stream: %d", stream_result);
        return FALSE;
    }

    return TRUE;
}

void platform_shutdown(platform_state* plat_state) {
    // Simply cold-cast to the known type.
    internal_state* state = (internal_state*)plat_state->internal_state;

    // Turn key repeats back on since this is global for the OS... just... wow.
    XAutoRepeatOn(state->display);

    xcb_destroy_window(state->connection, state->window);
}

b8 platform_pump_messages(platform_state* plat_state) {
    // Simply cold-cast to the known type.
    internal_state* state = (internal_state*)plat_state->internal_state;

    xcb_generic_event_t* event;
    xcb_client_message_event_t* cm;
    xcb_key_press_event_t* kp;
    xcb_key_release_event_t* kr;
    xcb_button_press_event_t* bp;
    xcb_button_release_event_t* br;
    xcb_motion_notify_event_t* mn;
    xcb_configure_notify_event_t* cn;

    b8 quit_flagged = FALSE;

    // Poll for events until null is returned.
    while (event != 0) {
        event = xcb_poll_for_event(state->connection);
        if (event == 0) {
            break;
        }

        // Input events
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
            {
                kp = (xcb_key_press_event_t*)event;
                printf("[KeyPress] detail(keycode)=%u\n", kp->detail);
                // 这里为了简单，ESC 常见 keycode 可能是 9，但这不是跨布局可靠方案
                // 真正要可靠做键盘映射，需要 xcb-keysyms 或 xkb。
                if (kp->detail == 9)
                {
                    quit_flagged = TRUE;
                }
                break;
            }
            case XCB_KEY_RELEASE: {
                kr = (xcb_key_release_event_t*)event;
                printf("[KeyRelease] detail(keycode)=%u\n", kr->detail);
                break;
            }
            case XCB_BUTTON_PRESS:
            {
                bp = (xcb_button_press_event_t*)event;
                printf("[ButtonPress] (%u), pos=(%d,%d)\n",
                                bp->detail,
                                bp->event_x,
                                bp->event_y);
                break;
            }
            case XCB_BUTTON_RELEASE: {
                br = (xcb_button_release_event_t*)event;
                printf("[ButtonRelease] (%u), pos=(%d,%d)\n",
                        br->detail,
                        br->event_x,
                        br->event_y);
                break;
            }
            case XCB_MOTION_NOTIFY:
            {
                mn = (xcb_motion_notify_event_t*)event;
                printf("[MotionNotify] pos=(%d,%d)\n", mn->event_x, mn->event_y);
                break;
            }
            case XCB_CONFIGURE_NOTIFY: {
                cn = (xcb_configure_notify_event_t*)event;
                printf("[ConfigureNotify] x=%d y=%d w=%d h=%d\n",
                            cn->x, cn->y, cn->width, cn->height);
                break;
            }
            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t*)event;

                // Window close
                if (cm->data.data32[0] == state->wm_delete_win) {
                    quit_flagged = TRUE;
                }
                break;
            }
            default:
                // Something else
                break;
        }

        free(event);
    }
    return !quit_flagged;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}
void platform_free(void* block, b8 aligned) {
    free(block);
}
void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}
void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}
void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

/**
 * @brief 向控制台输出带颜色的日志信息（基于 ANSI Escape Code）
 *
 * 该函数使用 ANSI 转义序列（ANSI Escape Sequences）在终端中输出带颜色的文本，
 * 常用于日志系统（Logger）中区分不同级别的信息，例如：
 *   - FATAL / ERROR / WARN / INFO / DEBUG / TRACE
 *
 * ----------------------------------------------------------------------------------
 * 【ANSI 颜色原理说明】
 *
 * ANSI 转义序列格式：
 *   \033[<style>;<color>m
 *
 * 说明：
 *   \033        -> ESC 控制字符（八进制表示）
 *   [           -> 控制序列起始
 *   <...>       -> 样式参数
 *   m           -> 设置图形模式（SGR: Select Graphic Rendition）
 *
 * 示例：
 *   "\033[1;31m" -> 高亮红色文本
 *   "\033[0m"    -> 重置颜色（恢复默认）
 *
 * ----------------------------------------------------------------------------------
 * 【颜色映射说明】
 *
 * 当前实现将日志等级映射为如下 ANSI 样式字符串：
 *
 *   index | 含义   | ANSI码  | 效果
 *   ------|--------|---------|----------------
 *     0   | FATAL  | 0;41    | 红底（背景红）
 *     1   | ERROR  | 1;31    | 高亮红（前景）
 *     2   | WARN   | 1;33    | 高亮黄
 *     3   | INFO   | 1;32    | 高亮绿
 *     4   | DEBUG  | 1;34    | 高亮蓝
 *     5   | TRACE  | 1;30    | 高亮黑（灰色）
 *
 * ----------------------------------------------------------------------------------
 * @param message  要输出的日志字符串（必须为有效的以 '\0' 结尾的字符串）
 * @param color    日志等级（对应上方颜色数组索引）
 *
 * ----------------------------------------------------------------------------------
 * 【重要注意事项】
 *
 * 1. 参数范围
 *    - color 必须在 [0, 5] 范围内，否则会发生数组越界（未定义行为）
 *
 * 2. 平台兼容性
 *    - Linux / macOS 终端：原生支持 ANSI 颜色（OK）
 *    - Windows：
 *        - Windows 10 之前：默认不支持 ANSI，需要调用 WinAPI 启用
 *        - Windows 10+：需要开启虚拟终端模式（ENABLE_VIRTUAL_TERMINAL_PROCESSING）
 *
 * 3. 线程安全
 *    - printf 本身不是严格线程安全（多线程日志可能交错）
 *    - 建议在 logger 层做锁（mutex / spinlock）
 *
 * 4. 输出原子性
 *    - 该函数一次 printf 输出完整字符串，尽量避免被打断
 *
 * 5. 性能
 *    - ANSI 转义开销极低，可安全用于实时日志（如引擎帧日志）
 *
 * ----------------------------------------------------------------------------------
 * 【使用示例】
 *
 *   platform_console_write("Hello World\n", 3); // INFO -> 绿色
 *
 * ----------------------------------------------------------------------------------
 */
void platform_console_write(const char* message, u8 color) {
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_strings[] = {
        "0;41", // FATAL  -> 红底
        "1;31", // ERROR  -> 红色
        "1;33", // WARN   -> 黄色
        "1;32", // INFO   -> 绿色
        "1;34", // DEBUG  -> 蓝色
        "1;30"  // TRACE  -> 灰色
    };

    printf("\033[%sm%s\033[0m", colour_strings[color], message);
}
void platform_console_write_error(const char* message, u8 color) {
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    // TODO: 清添加详细注释
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colour_strings[color], message);
}

f64 platform_get_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void platform_sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, 0);
#else
    if (ms >= 1000) {
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

#endif