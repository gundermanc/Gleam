// Gleam rendering server
// By: Christian Gunderman

package main

import "errors"

// RegisterRenderHandlers registers server message handlers
// for render related messages.
func RegisterRenderHandlers(server *Server) {
	renderer := renderer{}
	server.AddHandler(dimensionsMessageName, &renderer)
	server.AddHandler(layoutMessageName, &renderer)
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

func (renderer *renderer) TryHandleMessage(server *Server, message Message) error {
	switch message.Name() {
	case dimensionsMessageName:
		return renderer.handleDimensionsMessage(server, message)
	case layoutMessageName:
		return renderer.handleLayoutMessage(server, message)
	}

	return nil
}

func (renderer *renderer) handleDimensionsMessage(server *Server, message Message) error {

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

func (renderer *renderer) handleLayoutMessage(server *Server, message Message) error {
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
	Color color  `json:"color"`
	Text  string `json:"text"`
	Size  uint   `json:"size"`
}

type drawRectInstructionParams struct {
	Color color `json:"color"`
}

type drawRectOutlineInstructionParams struct {
	Color     color   `json:"color"`
	Thickness float32 `json:"thickness"`
}

type instructionType uint

const (
	// TextInstruction indicates that the renderer should be drawing text.
	TextInstruction instructionType = iota

	// RectInstruction indicates that the renderer should draw a filled rectangle.
	RectInstruction instructionType = 1

	// RectOutlineInstruction indicates that the renderer should draw an outline of a rectangle.
	RectOutlineInstruction instructionType = 2
)

type instruction struct {
	Type   instructionType `json:"type"`
	X      uint            `json:"x"`
	Y      uint            `json:"y"`
	Width  uint            `json:"width"`
	Height uint            `json:"height"`
	Params interface{}     `json:"params"`
}

type layout struct {
	Instructions []instruction `json:"instructions"`
}

func textInstruction(text string, color color, size uint, x uint, y uint, width uint, height uint) instruction {
	return instruction{
		Type:   TextInstruction,
		X:      x,
		Y:      y,
		Width:  width,
		Height: height,
		Params: drawTextInstructionParams{
			Color: color,
			Text:  text,
			Size:  size,
		},
	}
}

func rectInstruction(color color, x uint, y uint, width uint, height uint) instruction {
	return instruction{
		Type:   RectInstruction,
		X:      x,
		Y:      y,
		Width:  width,
		Height: height,
		Params: drawRectInstructionParams{
			Color: color,
		},
	}
}

func rectOutlineInstruction(color color, thickness float32, x uint, y uint, width uint, height uint) instruction {
	return instruction{
		Type:   RectOutlineInstruction,
		X:      x,
		Y:      y,
		Width:  width,
		Height: height,
		Params: drawRectOutlineInstructionParams{
			Color:     color,
			Thickness: thickness,
		},
	}
}

func (renderer *renderer) doLayout(
	server *Server,
	message Message,
	x uint,
	y uint,
	width uint,
	height uint) error {

	layout := layout{}

	layout.Instructions = append(
		layout.Instructions,
		rectInstruction(blue, 0, 0, renderer.width, renderer.height))

	layout.Instructions = append(
		layout.Instructions,
		rectOutlineInstruction(red, 10, 0, 0, width, height))

	layout.Instructions = append(
		layout.Instructions,
		textInstruction("Hello world :)", green, 10, 0, 0, width, height))

	server.WriteResponse(doLayoutMessageName, layout, "Requesting draw text")
	return nil
}
