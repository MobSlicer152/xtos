target("xtos")
    set_kind("binary")
    add_includedirs("../include")
    add_files("*.c")

    add_ldflags("-Wl,/entry:KernelMain", {force = true})
    add_ldflags("-Wl,/subsystem:native", {force = true})
