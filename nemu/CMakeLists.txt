
cmake_minimum_required(VERSION 3.5)
project(nemu)
add_executable(x86-nemu)
target_include_directories(x86-nemu PUBLIC "include")
target_compile_options(x86-nemu PUBLIC -Wno-restrict -Wall -Werror -ggdb3)
target_link_libraries(x86-nemu PRIVATE readline SDL2 dl)
target_sources(x86-nemu PRIVATE "src/memory/memory.c"
    "src/isa/x86/decode/decode.c"
    "src/monitor/diff-test/ref.c"
    "src/device/device.c"
    "src/isa/x86/diff-test.c"
    "src/monitor/debug/log.c"
    "src/isa/x86/exec/exec.c"
    "src/isa/x86/intr.c"
    "src/isa/x86/exec/cc.c"
    "src/monitor/cpu-exec.c"
    "src/monitor/debug/expr.c"
    "src/device/io/mmio.c"
    "src/isa/x86/exec/arith.c"
    "src/isa/x86/exec/control.c"
    "src/device/keyboard.c"
    "src/isa/x86/logo.c"
    "src/monitor/debug/ui.c"
    "src/isa/x86/decode/modrm.c"
    "src/isa/x86/exec/logic.c"
    "src/isa/x86/exec/data-mov.c"
    "src/isa/x86/init.c"
    "src/device/intr.c"
    "src/isa/x86/exec/system.c"
    "src/device/vga.c"
    "src/isa/x86/exec/prefix.c"
    "src/monitor/monitor.c"
    "src/device/io/port-io.c"
    "src/cpu/cpu.c"
    "src/cpu/inv.c"
    "src/isa/x86/exec/special.c"
    "src/device/serial.c"
    "src/cpu/relop.c"
    "src/monitor/debug/watchpoint.c"
    "src/main.c"
    "src/isa/x86/reg.c"
    "src/monitor/diff-test/diff-test.c"
    "src/device/timer.c"
    "src/isa/x86/mmu.c"
    "src/device/io/map.c")
target_include_directories(x86-nemu PRIVATE "src/isa/x86/include")

add_subdirectory(tools)
