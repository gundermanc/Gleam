// Gleam session logging facilities.
// (C) 2021 Christian Gunderman

use chrono::Datelike;
use chrono::Timelike;
use chrono::Utc;
use std::fs::File;
use std::fs::OpenOptions;
use std::io::Write;

/// Verbosity for Gleam logging.
pub enum Verbosity {
    /// Disables logging.
    Off = 0,

    /// Sets logging to most verbose level.
    Information = 1,

    /// Displays only warnings and errors.
    Warning = 2,

    /// Displays only errors.
    Error = 3,
}

/// Logging framework for Gleam.
pub struct Logger {
    verbosity: Verbosity,
    writer: File
}

impl Logger {

    /// Creates the session logger for Gleam.
    pub fn create(log_file: &str) -> Logger {

        // Open the log file for append. Since this is a hacky prototype,
        // we'll consider failure to start panic-worthy.
        let writer = OpenOptions::new()
            .create(true)
            .write(true)
            .append(true)
            .open(log_file)
            .unwrap();

        // By default, logger is given 'information' verbosity, until we read the
        // desired verbosity settings from configuration.
        return Logger { verbosity: Verbosity::Information, writer };
    }

    /// Sets the verbosity level for the logger.u32
    pub fn set_verbosity(&mut self, verbosity: Verbosity) {
        self.verbosity = verbosity;
    }

    /// Writes an informational message to the log.
    pub fn log_information(&mut self, line: &str) {
        match self.verbosity {
            Verbosity::Information => {
                self.log_line(INFO_HEADER, line);
            },
            _ => { }
        }
    }

    /// Writes an warning message to the log.
    pub fn log_warning(&mut self, line: &str) {
        match self.verbosity {
            Verbosity::Warning | Verbosity::Information => {
                self.log_line(WARNING_HEADER, line);
            },
            _ => { }
        }
    }

    /// Writes an warning message to the log.
    pub fn log_error(&mut self, line: &str) {
        match self.verbosity {
            Verbosity::Off => { },
            _ => self.log_line(WARNING_HEADER, line)
        }
    }

    fn log_line(&mut self, header: &str, line: &str) {
        // Unwrap all immediately. We consider log write failures to be fatal.

        let date_time = Utc::now();

        let formatted_date_time = format!(
            "{}/{}/{} {}:{}:{} ",
            date_time.month(), date_time.day(), date_time.year(),
            date_time.hour(), date_time.minute(), date_time.second());

        self.writer.write_all(header.as_bytes()).unwrap();
        self.writer.write_all(formatted_date_time.as_bytes()).unwrap();
        self.writer.write_all(line.as_bytes()).unwrap();
        self.writer.write_all(LINE_ENDING.as_bytes()).unwrap();
        self.writer.flush().unwrap();
    }
}

const INFO_HEADER: &str = "INFO: ";
const WARNING_HEADER: &str = "WARN: ";
const ERROR_HEADER: &str = "ERRR: ";
const LINE_ENDING: &str = "\r\n";