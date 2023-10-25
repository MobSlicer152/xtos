set_project("xtos")
set_version("0.0.0")

set_allowedplats("windows")
set_allowedarchs("x64", "riscv64", "loongarch64", "aarch64")

add_rules("mode.debug", "mode.release")

toolchain("xtfsclang")
    -- mark as standalone toolchain
    set_kind("standalone")

    -- set toolset
    set_toolset("cc", "clang")
    set_toolset("cxx", "clang", "clang++")
    set_toolset("ld", "clang++", "clang")
    set_toolset("sh", "clang++", "clang")
    set_toolset("ar", "llvm-ar")
    set_toolset("ex", "llvm-ar")
    set_toolset("strip", "llvm-strip")
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
            if is_arch("x64") then
                return "-target x86_64-unknown-windows"
            elseif is_arch("riscv64") then
                return "-target riscv64-unknown-windows"
            elseif is_arch("loongarch64") then
                return "-target loongarch64-unknown-windows"
            elseif is_arch("aarch64", "arm64") then
                return "-target aarch64-unknown-windows"
            end

            return nil
        end

        local march = get_march()

        -- init flags for c/c++
        toolchain:add("cflags", "-std=gnu2x")
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

includes("include")

includes("efi")
includes("boot")

includes("xtos")

target("bootimage")
    set_kind("phony")

    add_deps("boot", "xtos")

    on_build(function (target)
        local outdir = path.absolute(path.join(target:targetdir(), "img"))
        os.mkdir(outdir)
        local efidir = path.absolute(path.join(outdir, "/EFI/BOOT"))
        os.mkdir(efidir)

        function get_bootloader_name()
            if is_arch("x64") then
                return "BOOTX64.EFI"
            elseif is_arch("riscv64") then
                return "BOOTRISCV64.EFI"
            elseif is_arch("loongarch64") then
                return "BOOTLOONGARCH64.EFI"
            elseif is_arch("aarch64", "arm64") then
                return "BOOTAARCH64.EFI"
            end

            return nil
        end

        local origcd = os.workingdir()

        local boot = target:dep("boot")
        local boot = path.relative(path.absolute(boot:targetfile()), path.absolute(efidir))
        os.cd(efidir)
        local bootloader = get_bootloader_name()
        os.trycp(boot, path.join(efidir, bootloader))

        local xtos = target:dep("xtos")
        local xtos = path.relative(path.absolute(xtos:targetfile()), path.absolute(outdir))
        os.cd(outdir)
        os.trycp(xtos, path.join(outdir, "xtos.exe"))

        os.cd(origcd)
    end)

