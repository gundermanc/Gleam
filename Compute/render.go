// Gleam rendering server
// By: Christian Gunderman

package main

// RegisterRenderHandlers registers server message handlers
// for render related messages.
func RegisterRenderHandlers(server *Server) {
	renderer := renderer{}
	server.AddHandler(layoutMessageName, renderer)
}

// Message names:

const layoutMessageName = "gleam/render/layout"
const noChangeMessageName = "gleam/render/nochange"

type renderer struct {
}

func (renderer) TryHandleMessage(server *Server, message message) error {
	switch message.Name {
	case layoutMessageName:
		server.WriteResponse(noChangeMessageName, nil, "No change required at this time")
	}

	return nil
}
