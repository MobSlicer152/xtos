target("xtos")
    set_kind("binary")
    add_includedirs("../efi", "../include")
    add_headerfiles("*.h")
    add_files("*.c")

    add_ldflags("-Wl,/kernel", {force = true})
    add_ldflags("-Wl,/entry:KernelMain", {force = true})
    add_ldflags("-Wl,/subsystem:native", {force = true})
    add_ldflags("-Wl,/base:0x14000000", {force = true})
