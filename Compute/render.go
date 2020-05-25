// Gleam rendering server
// By: Christian Gunderman

package main

import "errors"

// RegisterRenderHandlers registers server message handlers
// for render related messages.
func RegisterRenderHandlers(server *Server) {
	renderer := renderer{}
	server.AddHandler(dimensionsMessageName, renderer)
	server.AddHandler(layoutMessageName, renderer)
}

// Request message names:

const dimensionsMessageName = "gleam/render/dimensions"
const layoutMessageName = "gleam/render/layout"

// Response message names:

const doLayoutMessageName = "gleam/render/doLayout"

type renderer struct {
	width  uint
	height uint
}

func (renderer renderer) TryHandleMessage(server *Server, message Message) error {
	switch message.Name() {
	case dimensionsMessageName:
		return renderer.handleDimensionsMessage(server, message)
	case layoutMessageName:
		return renderer.handleLayoutMessage(server, message)
	}

	return nil
}

func (renderer renderer) handleDimensionsMessage(server *Server, message Message) error {

	var err error

	if width, err := message.UIntParam("width"); err == nil {
		if height, err := message.UIntParam("height"); err == nil {
			if width < 10 || height < 10 {
				return errors.New("Invalid dimensions")
			}
			renderer.width = width
			renderer.height = height
		}
	}

	return err
}

func (renderer renderer) handleLayoutMessage(server *Server, message Message) error {
	var err error

	if x, err := message.UIntParam("x"); err == nil {
		if y, err := message.UIntParam("y"); err == nil {
			if width, err := message.UIntParam("width"); err == nil {
				if height, err := message.UIntParam("height"); err == nil {
					return renderer.doLayout(server, message, x, y, width, height)
				}
			}
		}
	}

	return err
}

type drawTextInstructionParams struct {
	Text string `json:"text"`
	Size uint   `json:"size"`
}

type instruction struct {
	Type   uint        `json:"type"`
	X      uint        `json:"x"`
	Y      uint        `json:"y"`
	Width  uint        `json:"width"`
	Height uint        `json:"height"`
	Params interface{} `json:"params"`
}

type layout struct {
	Instructions []instruction `json:"instructions"`
}

func (renderer renderer) doLayout(
	server *Server,
	message Message,
	x uint,
	y uint,
	width uint,
	height uint) error {

	layout := layout{}

	layout.Instructions = append(layout.Instructions, instruction{
		Type:   0,
		X:      x,
		Y:      y,
		Width:  width,
		Height: height,
		Params: drawTextInstructionParams{
			Text: "My placeholder text",
			Size: 50,
		},
	})

	server.WriteResponse(doLayoutMessageName, layout, "Requesting draw text")
	return nil
}
