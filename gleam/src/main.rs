mod configuration;
mod logger;

use crate::configuration::Configuration;
use crate::logger::Logger;

fn main() {
    // Print welcome messages
    println!("Gleam");
    println!("(C) 2021 Christian Gunderman");
    println!();

    // Set up logging.
    println!("Setting up logging...");
    let mut logger = Logger::create("gleam.log");
    logger.log_information("Started session with logging");

    // Load global configuration. Unwrap immediately, this is a fatal error.
    let config = Configuration::open("global.toml", &mut logger).unwrap();

    // Read verbosity configuration.
    println!("{}", config.get_str("Verbosity", "Foobar", &mut logger));
}
