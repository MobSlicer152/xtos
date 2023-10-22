target("boot")
    set_kind("binary")
    add_files("*.c")
    add_includedirs("../efi")
    add_deps("BaseLib")
    add_deps("BasePeCoffLib")
    add_deps("UefiLib")

    if is_arch("x86_64", "x64") then
        add_includedirs("../efi/X64")
    elseif is_arch("riscv64") then
        add_includedirs("../efi/RiscV64")
    elseif is_arch("loongarch64") then
        add_includedirs("../efi/LoongArch64")
    elseif is_arch("aarch64", "arm64") then
        add_includedirs("../efi/AArch64")
    else
        add_includedirs("../efi/Ia32")
    end

    set_extension(".efi")
    
    add_ldflags("-Wl,/entry:EfiMain", {force = true})
    add_ldflags("-Wl,/subsystem:efi_application", {force = true})
