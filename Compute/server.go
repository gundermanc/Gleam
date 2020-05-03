package main

import (
	"bufio"
	"encoding/json"
	"errors"
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

// MessageHandler is an abstraction for code that handles a message.
type MessageHandler interface {
	TryHandleMessage(server *Server, message message) error
}

// Server encapsulates the server application state.
type Server struct {
	handlers     map[string]MessageHandler
	reader       LineReader
	writer       LineWriter
	responseSent bool
}

// NewServer creates a new server instance, operating on the given reader/writer.
func NewServer(reader LineReader, writer LineWriter) *Server {
	return &Server{
		handlers: make(map[string]MessageHandler),
		reader:   reader,
		writer:   writer,
	}
}

// AddHandler registers a MessageHandler to handling calls from the renderer process.
func (server *Server) AddHandler(messageName string, handler MessageHandler) error {
	if server.handlers[messageName] != nil {
		return errors.New("Duplicate message handler for " + messageName)
	}

	server.handlers[messageName] = handler

	return nil
}

// WriteResponse replies to the server and is intended for use by message
// handlers in response to a message that was received.
func (server *Server) WriteResponse(message string, params interface{}, logString string) error {
	server.responseSent = true
	return writeMessage(server.writer, message, params, logString)
}

// StartServer begins running the server main loop, returning only
// after the exit message is received.
func (server *Server) StartServer() {
	// Write initialize message.
	writeMessage(server.writer, initializeMessageName, nil, "Server initialized")

	for {
		server.responseSent = false

		if message, err := readMessage(server.reader); err != nil && err != io.EOF {
			writeMessage(server.writer, errorMessageName, nil, err.Error())
		} else {

			// Check and see if there's a registered message handler...
			if handler := server.handlers[message.Name]; handler != nil {

				// There is a handler, dispatch to it.
				if err := handler.TryHandleMessage(server, *message); err != nil {
					writeMessage(server.writer, errorMessageName, nil, err.Error())
				} else if !server.responseSent {
					writeMessage(server.writer, errorMessageName, nil, "A handler failed to reply")
				}
			} else if message.Name == exitMessageName {
				writeMessage(server.writer, exitingMessageName, nil, "Exit message received")
				return
			} else {
				writeMessage(server.writer, errorMessageName, nil, "Unknown message "+message.Name)
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

func writeMessage(writer LineWriter, message string, params interface{}, logString string) error {
	message, err := createMessage(message, params, logString)
	if err == nil {
		writer.WriteLine(message)
	}

	return err
}

func createMessage(messageName string, params interface{}, logString string) (string, error) {

	var encodedMessage interface{}

	baseMessage := &message{
		Name:      messageName,
		LogString: logString,
	}

	if params != nil {
		encodedMessage = &messageWithParams{
			message: baseMessage,
			Params:  params,
		}
	} else {
		encodedMessage = baseMessage
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

type messageWithParams struct {
	*message

	// Parameter object
	Params interface{} `json:"params"`
}
