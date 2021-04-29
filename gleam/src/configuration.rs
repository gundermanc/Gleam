use std::fs::File;
use std::io::Read;

use crate::logger::Logger;

/// Represents a configuration file context.
pub struct Configuration {
    configuration_map: toml::map::Map<String, toml::Value>,
}

impl Configuration {

    /// Creates a new configuration file context.
    pub fn open(config_file: &str, logger: &mut Logger) -> Option<Configuration> {
        logger.log_information(format!("Loading configuration file '{}'", config_file).as_str());

        match File::open(config_file) {
            Ok(mut file) => load_configuration(&mut file, logger),

            Err(err) => {
                logger.log_error(
                    format!(
                        "Unable to open configuration file '{}', {}",
                        config_file,
                        err.to_string()).as_str());

                return None;
            }
        }
    }

    /// Gets a String value from a configuration file.
    pub fn get_str(&self, key: &str, default: &str, logger: &mut Logger) -> String {
        match self.configuration_map.get(key) {
            Some(value) => match value.as_str() {
                Some(str_value) => String::from(str_value),
                None => {
                    logger.log_information(
                        format!(
                            "Using default value {} for key {}", default, key).as_str());
    
                    return String::from(default);
                }
            }
            None => {
                logger.log_information(
                    format!(
                        "Using default value {} for key {}", default, key).as_str());

                return String::from(default);
            }
        }
    }
}

fn load_configuration(file: &mut File, logger: &mut Logger) -> Option<Configuration> {
    let mut content = String::new();
    match file.read_to_string(&mut content) {
        Ok(result) => {
            match toml::from_str(&content) {
                Ok(result) => {
                    let map: toml::map::Map<String, toml::Value> = result;

                    return Some(Configuration { configuration_map: map })
                }
                Err(err) => {
                    logger.log_error(format!("Unable to parse TOML: {}", err.to_string()).as_str());

                    return None;
                }
            }
        }

        Err(err) => {
            logger.log_error("Unable to read from configuration file");

            return None
        }
    }
}