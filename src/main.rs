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
        .author("https://github.com/Soviet-Linux/CCCP/")
        .subcommand(
            Command::new("install") // install
                .short_flag('i') // -i 
                .long_flag("install") // --install
                .about("Install a package from the OUR")
                .arg_required_else_help(true)
                .arg(
                    Arg::new("package")
                        .help("the package names you want to install")
                        .required(true)
                        .action(ArgAction::Set)
                        .multiple_values(true)
                )
        )
        .subcommand(
            Command::new("package") // package
                .short_flag('p') // -p 
                .long_flag("package") // --package
                .about("Install a package file")
                .arg_required_else_help(true)
                .arg(
                    Arg::new("package")
                        .help("The package file you want to install")
                        .required(true)
                        .action(ArgAction::Set)
                        .multiple_values(true)
                )
        )
        .subcommand(
            Command::new("remove") // remove
                .short_flag('r') // -p 
                .long_flag("remove") // --remove
                .about("Uninstall a package")
                .arg_required_else_help(true)
                .arg(
                    Arg::new("package")
                        .help("The name of the package you want to uninstall")
                        .required(true)
                        .action(ArgAction::Set)
                        .multiple_values(true)
                )
        )
        .subcommand(
            Command::new("check") // check
                .short_flag('c') // -p 
                .long_flag("check") // --check
                .about("Check if a package is insalled")
                .arg_required_else_help(true)
                .arg(
                    Arg::new("package")
                        .help("The name of the package you want to check")
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
        },
        Some(("package", package_mathces)) => {
            let packagefiles: Vec<_> = package_mathces
                .get_many::<String>("package")
                .expect("is present")
                .map(|s| s.as_str())
                .collect();
            println!("Installing package files {}", packagefiles.join(", "));
        	for file in packagefiles {
                init();
				install_spm_file(file.to_string(),0);
            }

        },
        Some(("remove", remove_matches)) => {
            let packages: Vec<_> = remove_matches
                .get_many::<String>("package")
                .expect("is present")
                .map(|s| s.as_str())
                .collect();
            println!("Uninstalling {} ...", packages.join(", "));
        },
        Some(("check", remove_matches)) => {
            let packages: Vec<_> = remove_matches
                .get_many::<String>("package")
                .expect("is present")
                .map(|s| s.as_str())
                .collect();
            println!("checking if {} is installed...", packages.join(", "));
        },
        _ => {
            unreachable!()
        }
    }
}
