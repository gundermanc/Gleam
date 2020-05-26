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

	renderer.root = NewTextView(NewTextBuffer("Hello world :)\r\nThis is a demo project by Christian."))
}

// Request message names:

const dimensionsMessageName = "gleam/render/dimensions"
const layoutMessageName = "gleam/render/layout"

// Response message names:

const doLayoutMessageName = "gleam/render/doLayout"
const textDimensionsMessageName = "gleam/render/textDimensions"

type renderer struct {
	width  uint
	height uint

	root Control
}

type RenderContext interface {
	ComputeTextDimensions(text string, size uint) (uint, uint, error)
}

type renderContext struct {
	server *Server
}

type textDimensionsParams struct {
	// Name of the message.
	Text string `json:"text"`

	Size uint `json:"size"`
}

func (renderContext *renderContext) ComputeTextDimensions(text string, size uint) (uint, uint, error) {
	response, err := renderContext.server.RequestClarification(
		textDimensionsMessageName,
		textDimensionsParams{
			Text: text,
			Size: size,
		},
		"Request text dimensions")

	if err == nil {
		if width, err := response.UIntParam("width"); err == nil {
			if height, err := response.UIntParam("height"); err == nil {
				return width, height, nil
			}
		}
	}

	return 0, 0, err
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

type layout struct {
	Instructions []instruction `json:"instructions"`
}

func (renderer *renderer) doLayout(
	server *Server,
	message Message,
	x uint,
	y uint,
	width uint,
	height uint) error {

	layout := layout{
		Instructions: renderer.root.Render(
			&renderContext{server},
			width,
			height),
	}

	server.WriteResponse(doLayoutMessageName, layout, "Requesting draw text")
	return nil
}
