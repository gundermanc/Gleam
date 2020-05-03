package main

func main() {
	server := NewStdioServer()

	RegisterRenderHandlers(server)

	server.StartServer()
}
