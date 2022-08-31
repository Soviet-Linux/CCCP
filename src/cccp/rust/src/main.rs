use libspm_rs::*;

use clap::{Arg, ArgAction, Command};

const FRONT_END_VERSION: &str = env!("CARGO_PKG_VERSION");

fn main() {
    let version: String = format!("\nRust front-end version: {}\nlibspm-rs version: {}",
        FRONT_END_VERSION,
        libspm_rs::get_version().to_string()
    ); // this implementation is ugly, but couldn't get it to work as a const

    let matches = Command::new("cccp")
        .about("CCCP Crafter of Controlled Packages")
        .version(&*version)
        .subcommand_required(true)
        .arg_required_else_help(true) // if no args are supplied, then print help
        .author("https://git.sovietlinux.ml/sovietlinux/CCCP/")
        .subcommand(
            Command::new("install") // install
                .short_flag('i') // -i 
                .long_flag("install") // --install
                .about("Install a package from the repos")
                .arg(
                    Arg::new("package")
                        .help("the package names you want to install")
                        .required(true)
                        .action(ArgAction::Set)
                        .multiple_values(true)
                )
        )
        .get_matches();

    match matches.subcommand() {
        Some(("install", install_matches)) => {
            let packages: Vec<_> = install_matches
                .get_many::<String>("package")
                .expect("is present")
                .map(|s| s.as_str())
                .collect();
            // this ^ packages vector would be used probably for the function calls
            let values = packages.join(", "); // a comma separated string with each package name 
            println!("Installing {}...", values);
        }
        _ => {
            unreachable!()
        }
    }
}
