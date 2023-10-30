use std::fmt;

#[derive(Debug)]
pub enum Architecture {
    Amd64,
    LoongArch64,
    RiscV64,
    AArch64,
}

impl fmt::Display for Architecture {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", match self {
            Self::Amd64 => "amd64",
            Self::LoongArch64 => "loongarch64",
            Self::RiscV64 => "riscv64",
            Self::AArch64 => "aarch64",
        })
    }
}

impl From<&str> for Architecture {
    fn from(value: &str) -> Self {
        Self::from(String::from(value))
    }
}

impl From<String> for Architecture {
    fn from(value: String) -> Self {
        let value = value.to_lowercase();
        match value.as_str() {
            "amd64" => Self::Amd64,
            "x64" => Self::Amd64,
            "x86_64" => Self::Amd64,
            "loongson" => Self::LoongArch64,
            "loongson64" => Self::LoongArch64,
            "loongarch" => Self::LoongArch64,
            "loongarch64" => Self::LoongArch64,
            "riscv" => Self::RiscV64,
            "riscv64" => Self::RiscV64,
            "arm" => Self::AArch64,
            "arm64" => Self::AArch64,
            "aa64" => Self::AArch64,
            "aarch64" => Self::AArch64,
            &_ => panic!("Unknown architecture {value}")
        }
    }
}

