use rfd::FileDialog;

#[macro_use]
extern crate conrod_core;
extern crate conrod_glium;
#[macro_use]
extern crate conrod_winit;
extern crate find_folder;
extern crate glium;

mod configuration;
mod editor;
mod logger;
mod support;

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

    editor::edit_file("Hello!");
}
