mod logger;

use logger::Logger;

fn main() {
    // Print welcome messages
    println!("Gleam");
    println!("(C) 2021 Christian Gunderman");
    println!();

    // Set up logging.
    println!("Setting up logging...");
    let mut logger = Logger::create("gleam.log");
    logger.log_information("Started session with logging");
}
