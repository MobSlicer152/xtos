target("gnuefi")
    set_kind("static")
    add_files("*.c", "runtime/*.c")
    add_defines("__MINGW32__")
    add_includedirs(".")

    if is_arch("x86_64", "x64") then
        add_files("x86_64/*.S", "x86_64/*.c")
    elseif is_arch("riscv64") then
        add_files("riscv64/*.S", "riscv64/*.c")
    elseif is_arch("loongarch64") then
        add_files("loongarch64/*.S", "loongarch64/*.c")
    elseif is_arch("aarch64", "arm64") then
        add_files("aarch64/*.S", "aarch64/*.c")
    else
        add_files("ia32/*.S", "ia32/*.c")
    end
