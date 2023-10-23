target("boot")
    set_kind("binary")
    add_files("*.c")
    add_includedirs("../include", "../efi")
    add_deps("gnuefi")

    if is_arch("x64") then
        add_includedirs("../efi/x86_64")
    elseif is_arch("riscv64") then
        add_includedirs("../efi/riscv64")
    elseif is_arch("loongarch64") then
        add_includedirs("../efi/loongarch64")
    elseif is_arch("aarch64", "arm64") then
        add_includedirs("../efi/aarch64")
    end

    set_extension(".efi")
    
    add_ldflags("-Wl,/entry:EfiMain", {force = true})
    add_ldflags("-Wl,/subsystem:efi_application", {force = true})
