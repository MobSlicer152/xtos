target("gnuefi")
    set_kind("static")
    add_files("*.c")
    add_files("runtime/*.c")
    add_defines("__MINGW32__")
    add_includedirs(".")

    if is_arch("x86_64", "x64") then
        add_files("x86_64/*.S")
        add_files("x86_64/*.c")
    elseif is_arch("riscv64") then
        add_files("riscv64/*.S")
        add_files("riscv64/*.c")
    elseif is_arch("loongarch64") then
        add_files("loongarch64/*.S")
        add_files("loongarch64/*.c")
    elseif is_arch("aarch64", "arm64") then
        add_files("aarch64/*.S")
        add_files("aarch64/*.c")
    else
        add_files("ia32/*.S")
        add_files("ia32/*.c")
    end
