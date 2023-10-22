function add_efi_lib(name)
    target(name)
        set_kind("static")
        add_files(name .. "/*.c")
        add_includedirs(".")
        add_forceincludes("Base.h")
        add_forceincludes("PcdDummy.h")

        if is_arch("x86_64", "x64") then
            add_files(name .. "/X64/*.asm")
            add_asflags("-PBase.inc", {force = true})
            add_includedirs("X64")
        elseif is_arch("riscv64") then
            add_files(name .. "/RiscV64/*.S")
            add_includedirs("RiscV64")
        elseif is_arch("loongarch64") then
            add_files(name .. "/LoongArch64/*.S")
            add_includedirs("LoongArch64")
        elseif is_arch("aarch64", "arm64") then
            add_files(name .. "/AArch64/*.S")
            add_includedirs("AArch64")
        else
            add_files(name .. "/Ia32/*.nasm")
            add_includedirs("Ia32")
        end
end

add_efi_lib("BaseLib")
add_efi_lib("BasePeCoffLib")
add_efi_lib("BasePrintLib")
add_efi_lib("UefiLib")

