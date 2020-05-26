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
	TryHandleMessage(server *Server, message Message) error
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

// RequestClarification is a scoped callback mechanism enabling the server to request
// more information to aid in the fulfillment of a request. As oppose to WriteResponse,
// it does not serve as a response to the original request.
func (server *Server) RequestClarification(message string, params interface{}, logString string) (Message, error) {
	writeMessage(server.writer, message, params, logString)

	return readMessage(server.reader)
}

// StartServer begins running the server main loop, returning only
// after the exit message is received.
func (server *Server) StartServer() {
	// Write initialize message.
	server.WriteResponse(initializeMessageName, nil, "Server initialized")

	server.serverLoop()
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

func (server *Server) serverLoop() {
	for {
		server.responseSent = false

		var err error

		if message, err := readMessage(server.reader); err == nil {
			if handler := server.handlers[message.Name()]; handler != nil {
				err = server.dispatchMessage(handler, message)
			} else if message.Name() == exitMessageName {
				server.WriteResponse(exitMessageName, nil, "Exit message received")
				return
			} else {
				err = errors.New("Unknown message " + message.Name())
			}
		}

		// Report any errors
		if err != nil && err != io.EOF {
			writeMessage(server.writer, errorMessageName, nil, err.Error())
		}
	}
}

func (server *Server) dispatchMessage(handler MessageHandler, message Message) error {
	if err := handler.TryHandleMessage(server, message); err != nil {
		return err
	}

	// Our handshake is 1) request 2) response. Send a generic ACK for components
	// that choose not to explicitly respond.
	if !server.responseSent {
		server.WriteResponse(acknowledgeMessageName, nil, "Acknowledged")
	}

	return nil
}

func readMessage(reader LineReader) (Message, error) {
	var message message

	// Read in the next line, if any.
	line, err := reader.ReadLine()
	if err != nil {
		return nil, io.EOF
	}

	//line := "{\"name\":\"gleam/render/layout\",\"params\":{\"x\": 0, \"y\":0, \"width\":10,\"height\":11},\"logString\":\"Server initialized\"}"

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

	encodedMessage := &message{
		NameString: messageName,
		ParamsMap:  params,
		LogString:  logString,
	}

	bytes, err := json.Marshal(&encodedMessage)

	return string(bytes), err
}

// Message names.

const acknowledgeMessageName = "gleam/ack"
const errorMessageName = "gleam/error"
const exitMessageName = "gleam/exit"
const initializeMessageName = "gleam/initialize"

// Message is an interface for interacting with JSON messages.
type Message interface {
	Name() string
	Log() string
	Params() (map[string]interface{}, error)
	IntParam(name string) (int, error)
	UIntParam(name string) (uint, error)
}

type message struct {
	// Name of the message.
	NameString string `json:"name"`

	// Parameter object
	ParamsMap interface{} `json:"params"`

	// Human readable description for log.
	LogString string `json:"logString"`
}

func (message message) Name() string {
	return message.NameString
}

func (message message) Log() string {
	return message.LogString
}

func (message message) Params() (map[string]interface{}, error) {
	if paramsMap, ok := message.ParamsMap.(map[string]interface{}); ok {
		return paramsMap, nil
	}

	return nil, errors.New("No parameters provided")
}

func (message message) IntParam(name string) (int, error) {
	if paramsMap, err := message.Params(); err == nil {
		if rawParam, ok := paramsMap[name]; ok {
			if floatParam, ok := rawParam.(float64); ok {
				return int(floatParam), nil
			}
		}
	}

	return 0, errors.New("Missing or invalid type parameter: " + name)
}

func (message message) UIntParam(name string) (uint, error) {
	if paramsMap, err := message.Params(); err == nil {
		if rawParam, ok := paramsMap[name]; ok {
			if floatParam, ok := rawParam.(float64); ok {
				return uint(floatParam), nil
			}
		}
	}

	return 0, errors.New("Missing or invalid type parameter: " + name)
}
