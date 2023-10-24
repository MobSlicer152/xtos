target("efi")
    set_kind("static")
    add_headerfiles("*.h")
    add_files("*.c", "runtime/*.c")
    add_defines("__MINGW32__")
    add_includedirs(".")

    if is_arch("x64") then
        add_headerfiles("x86_64/*.h")
        add_files("x86_64/*.S", "x86_64/*.c")
    elseif is_arch("riscv64") then
        add_headerfiles("riscv64/*.h")
        add_files("riscv64/*.S", "riscv64/*.c")
    elseif is_arch("loongarch64") then
        add_headerfiles("loongarch64/*.h")
        add_files("loongarch64/*.S", "loongarch64/*.c")
    elseif is_arch("aarch64", "arm64") then
        add_headerfiles("aarch64/*.h")
        add_files("aarch64/*.S", "aarch64/*.c")
    end
