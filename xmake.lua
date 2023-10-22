add_rules("mode.debug", "mode.release")

toolchain("xtfsclang")

    -- mark as standalone toolchain
    set_kind("standalone")

    -- set toolset
    set_toolset("cc", "clang")
    set_toolset("cxx", "clang", "clang++")
    set_toolset("ld", "clang++", "clang")
    set_toolset("sh", "clang++", "clang")
    set_toolset("ar", "ar")
    set_toolset("ex", "ar")
    set_toolset("strip", "strip")
    set_toolset("mm", "clang")
    set_toolset("mxx", "clang", "clang++")
    set_toolset("as", "clang")

    add_defines("_XT")

    -- check toolchain
    on_check(function (toolchain)
        return import("lib.detect.find_tool")("clang")
    end)

    -- on load
    on_load(function (toolchain)

        -- get march
        function get_march()
            if is_arch("x86_64", "x64") then
                return "-target x86_64-unknown-windows"
            elseif is_arch("riscv64") then
                return "-target riscv64-unknown-windows"
            elseif is_arch("loongarch64") then
                return "-target loongarch64-unknown-windows"
            elseif is_arch("aarch64", "arm64") then
                return "-target aarch64-unknown-windows"
            else
                return "-target i386-unknown-windows"
            end
        end

        local march = get_march()

        -- init flags for c/c++
        toolchain:add("cxflags", march)
        toolchain:add("cxflags", "-ffreestanding")
        toolchain:add("cxflags", "-fno-stack-protector")
        toolchain:add("cxflags", "-fshort-wchar")
        toolchain:add("cxflags", "-mno-red-zone")
        toolchain:add("ldflags", "-fuse-ld=lld")
        toolchain:add("ldflags", march)
        toolchain:add("ldflags", "-nostdlib")
        toolchain:add("ldflags", "-Wl,/filealign:16")
        toolchain:add("shflags", march)

        -- init flags for objc/c++  (with ldflags and shflags)
        toolchain:add("mxflags", march)

        -- init flags for asm
        toolchain:add("asflags", march)
    end)

includes("efi")
includes("boot")

includes("xtos")
    
