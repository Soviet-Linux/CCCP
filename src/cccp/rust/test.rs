use std::os::raw::c_float;

// rust stuff version
const VERSION: &str = "0.01";

//  import libspm.rs
extern "C" {
    pub fn version() -> c_float;
}




// create a main function and call the version function

fn main() {
    print_version();
}

fn print_version() {
    unsafe 
    {
        println!("[+] Libspm C version => {}", version());
        println!("[+] CCCP Rust version => {}", VERSION);        
    }
}