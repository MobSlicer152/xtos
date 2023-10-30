use sdktools::architecture::Architecture;
use fatfs;
use fscommon;
use gpt::{self, disk, GptConfig};
use std::{env, fs, io::Write, path::PathBuf};

fn main() {
    let mut args = env::args();
    args.next();
    let target_dir = PathBuf::from(args.next().expect("Missing target directory"));
    let arch = Architecture::from(args.next().expect("Missing architecture"));
    let image = target_dir.join(format!("xtos-{arch}.img"));

    println!("Making boot image {} for {arch}", image.display());

    const TOTAL_SIZE: usize = 64 * 1024 * 1024;
    const BLOCK_SIZE: disk::LogicalBlockSize = disk::LogicalBlockSize::Lb512;
    fs::write(&image, &[0_u8; TOTAL_SIZE]).unwrap();
 
    let mut file = fs::OpenOptions::new().read(true).write(true).open(&image).unwrap();   
    let mbr = gpt::mbr::ProtectiveMBR::with_lb_size(u32::try_from((TOTAL_SIZE / 512) - 1).unwrap_or(0xFFFFFFFF));
    mbr.overwrite_lba0(&mut file).unwrap();
    file.flush().unwrap();

    let mut gdisk = GptConfig::default()
        .writable(true)
        .logical_block_size(BLOCK_SIZE)
        .create(&image)
        .unwrap();

    gdisk.add_partition("ESP", 63 * 1024 * 1024, gpt::partition_types::EFI, 0, None).unwrap();

    gdisk.write_inplace().unwrap();

    let partition = &gdisk.partitions()[&1];
    let partition_offset = partition.bytes_start(BLOCK_SIZE).unwrap();
    let partition_len = partition.bytes_len(BLOCK_SIZE).unwrap();

    let mut partition_stream = fscommon::StreamSlice::new(&mut file, partition_offset, partition_offset + partition_len).unwrap();
    fatfs::format_volume(&mut partition_stream, fatfs::FormatVolumeOptions::new().fat_type(fatfs::FatType::Fat32)).unwrap();
    let fat = fatfs::FileSystem::new(&mut partition_stream, fatfs::FsOptions::new()).unwrap();
    let root = fat.root_dir();

    let bootloader_path = format!("EFI/BOOT/BOOT{}.EFI", match arch {
        Architecture::Amd64 => "x64",
        Architecture::LoongArch64 => "LOONGARCH64",
        Architecture::RiscV64 => "RISCV64",
        Architecture::AArch64 => "AA64",
    });

    let files = vec![
        (target_dir.join("xtos.exe"), PathBuf::from("xtos.exe")),
        (target_dir.join("boot.efi"), PathBuf::from(bootloader_path)),
    ];

    for file in files {
        let source = file.0;
        let dest = file.1;

        let dir = dest.parent().unwrap();

        println!("{} -> {}/{}", source.display(), image.display(), dest.display());

        let mut dirs = PathBuf::new();
        for dir in dir.iter() {
            dirs.push(dir);
            let dir = dirs.to_str().unwrap();
            root.create_dir(dir).unwrap();
        }

        let content = fs::read(source).unwrap();
        let mut file = root.create_file(dest.to_str().unwrap()).unwrap();
        file.write_all(content.as_slice()).unwrap();
    }
}
