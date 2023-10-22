target("BaseLib")
    set_kind("static")
    add_files("BaseLib/*.c")
    add_includedirs(".")
    add_forceincludes("Base.h")
    add_forceincludes("PcdDummy.h")

    if is_arch("x86_64", "x64") then
        add_includedirs("X64")
    elseif is_arch("riscv64") then
        add_includedirs("RiscV64")
    elseif is_arch("loongarch64") then
        add_includedirs("LoongArch64")
    elseif is_arch("aarch64", "arm64") then
        add_includedirs("AArch64")
    else
        add_includedirs("Ia32")
    end

target("BasePeCoffLib")
    set_kind("static")
    add_files("BasePeCoffLib/*.c")
    add_includedirs(".")
    add_forceincludes("Base.h")
    add_forceincludes("PcdDummy.h")

    if is_arch("x86_64", "x64") then
        add_includedirs("X64")
    elseif is_arch("riscv64") then
        add_includedirs("RiscV64")
    elseif is_arch("loongarch64") then
        add_includedirs("LoongArch64")
    elseif is_arch("aarch64", "arm64") then
        add_includedirs("AArch64")
    else
        add_includedirs("Ia32")
    end

target("UefiLib")
    set_kind("static")
    add_files("UefiLib/*.c")
    add_includedirs(".")
    --add_forceincludes("Base.h")
    add_forceincludes("PcdDummy.h")

    if is_arch("x86_64", "x64") then
        add_includedirs("X64")
    elseif is_arch("riscv64") then
        add_includedirs("RiscV64")
    elseif is_arch("loongarch64") then
        add_includedirs("LoongArch64")
    elseif is_arch("aarch64", "arm64") then
        add_includedirs("AArch64")
    else
        add_includedirs("Ia32")
    end

