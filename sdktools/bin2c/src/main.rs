use std::{
    env, fs,
    io::{Seek, Write},
    path::PathBuf,
};

fn main() {
    let mut args = env::args();
    let input = args.nth(1).expect("Missing input file");
    let output = args.nth(2).unwrap_or(input.clone() + ".c");

    println!("Converting file {input} to C file {output}");

    let data = fs::read(&input).unwrap();
    let mut file = fs::File::create(&output).unwrap();

    // from chatgpt
    let symbol = args.nth(3).unwrap_or({
        let input_path = PathBuf::from(input.clone());
        input_path
            .file_name()
            .unwrap()
            .to_str()
            .unwrap()
            .chars()
            .map(|c| if c.is_alphanumeric() { c } else { '_' })
            .collect::<String>()
    });

    file.write_all(format!("#include <basetsd.h>\n\nBYTE {symbol}_start[] = {{").as_bytes())
        .unwrap();
    // Limit line length to less than either 80 or the 4th argument
    let max_column: usize = args.nth(4).unwrap_or_default().parse().unwrap_or(80);
    // Immediately trigger the max length code, which works out
    let mut column = max_column;
    for b in data {
        let byte = format!(
            "{}0x{b:02X},",
            if column + 4 >= max_column {
                column = 0;
                "\n    "
            } else {
                " "
            }
        );
        column += byte.len();
        file.write_all(byte.as_bytes()).unwrap();
    }
    file.write_all(format!("\n}};\nSIZE_T {symbol}_size = sizeof({symbol}_start);\nPBYTE {symbol}_end = {symbol}_start + sizeof({symbol}_start);").as_bytes()).unwrap();

    println!("Wrote {} bytes to {output}, you can use {symbol}_start, {symbol}_end, and {symbol}_size to refer to the data.", file.stream_position().unwrap());
}
