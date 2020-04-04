package main

import (
	"bufio"
	"encoding/json"
	"io"
	"os"
	"strings"
)

// LineReader is an abstraction for reading strings line by line.
type LineReader interface {
	ReadLine() (string, error)
}

// LineWriter is an abstraction for writing strings line by line.
type LineWriter interface {
	WriteLine(line string) error
}

// Server encapsulates the server application state.
type Server struct {
	reader LineReader
	writer LineWriter
}

// NewServer creates a new server instance, operating on the given reader/writer.
func NewServer(reader LineReader, writer LineWriter) *Server {
	return &Server{
		reader: reader,
		writer: writer,
	}
}

// StartServer begins running the server main loop, returning only
// after the exit message is received.
func (server *Server) StartServer() {
	// Write initialize message.
	writeMessage(server.writer, initializeMessageName, "Server initialized")

	for {
		message, err := readMessage(server.reader)
		if err != nil && err != io.EOF {
			writeMessage(server.writer, errorMessageName, err.Error())
		} else {
			switch message.Name {
			case exitMessageName:
				writeMessage(server.writer, exitingMessageName, "Exit message received")
				return
			default:
				writeMessage(server.writer, errorMessageName, "Unknown message "+message.Name)
			}
		}
	}
}

// StdinReader encapsulates read actions for stdio and
// adapts them for use in the server.
type StdinReader struct {
	scanner *bufio.Scanner
}

// NewStdinReader creates a new reader from stdio.
func NewStdinReader() *StdinReader {
	return &StdinReader{
		scanner: bufio.NewScanner(os.Stdin),
	}
}

// ReadLine reads a single line from the underlying stdin.
func (reader *StdinReader) ReadLine() (string, error) {
	if !reader.scanner.Scan() {
		return "", io.EOF
	}

	return reader.scanner.Text(), nil
}

// StdoutWriter encapsulates write actions for stdio and
// adapts them for use in the server.
type StdoutWriter struct {
	writer *os.File
}

// NewStdoutWriter creates a writer for stdout.
func NewStdoutWriter() *StdoutWriter {
	return &StdoutWriter{
		writer: os.Stdout,
	}
}

// WriteLine writes a single line of text to the underlying stdout
func (writer *StdoutWriter) WriteLine(line string) error {
	_, err := writer.writer.WriteString(line)
	if err != nil {
		return err
	}

	// Append a line ending if needed.
	if !strings.HasSuffix(line, "\n") {
		_, err = writer.writer.Write([]byte("\n"))
	}

	return err
}

// NewStdioServer creates a server instance from the current process's
// stdin and stdout streams
func NewStdioServer() *Server {
	return NewServer(NewStdinReader(), NewStdoutWriter())
}

func readMessage(reader LineReader) (*message, error) {

	var message message

	// Read in the next line, if any.
	line, err := reader.ReadLine()
	if err != nil {
		return nil, io.EOF
	}

	// Deserialize the message.
	if err := json.Unmarshal([]byte(line), &message); err != nil {
		return nil, err
	}

	return &message, nil
}

func writeMessage(writer LineWriter, message string, logString string) error {
	message, err := createMessage(message, logString)
	if err == nil {
		writer.WriteLine(message)
	}

	return err
}

func createMessage(messageName string, logString string) (string, error) {
	encodedMessage := &message{
		Name:      messageName,
		LogString: logString,
	}

	bytes, err := json.Marshal(&encodedMessage)

	return string(bytes), err
}

// Message names.

const errorMessageName = "gleam/editor/error"
const exitMessageName = "gleam/editor/exit"
const exitingMessageName = "gleam/editor/exiting"
const initializeMessageName = "gleam/editor/initialize"

type message struct {
	// Name of the message.
	Name string `json:"name"`

	// Human readable description for log.
	LogString string `json:"logString"`
}
