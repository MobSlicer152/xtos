target("boot")
    set_kind("binary")
    add_headerfiles("*.h")
    add_files("*.c")
    add_includedirs("../include", "../efi")
    add_deps("efi")

    if is_arch("x64") then
        add_includedirs("../efi/x86_64")
        add_files("x64/*.S")
    elseif is_arch("riscv64") then
        add_includedirs("../efi/riscv64")
        add_files("riscv64/*.S")
    elseif is_arch("loongarch64") then
        add_includedirs("../efi/loongarch64")
        add_files("loongarch64/*.S")
    elseif is_arch("aarch64") then
        add_includedirs("../efi/aarch64")
        add_files("arm64/*.S")
    end

    set_extension(".efi")

    add_ldflags("-Wl,/entry:EfiMain", {force = true})
    add_ldflags("-Wl,/subsystem:efi_application", {force = true})
