// Gleam colors
// By: Christian Gunderman

package main

type color struct {
	R float32
	G float32
	B float32
	A float32
}

var black = color{
	R: 0,
	G: 0,
	B: 0,
	A: 1,
}

var white = color{
	R: 1,
	G: 1,
	B: 1,
	A: 1,
}

var transparent = color{
	R: 0,
	G: 0,
	B: 0,
	A: 0,
}

var red = color{
	R: 1,
	G: 0,
	B: 0,
	A: 1,
}

var green = color{
	R: 0,
	G: 1,
	B: 0,
	A: 1,
}

var blue = color{
	R: 0,
	G: 0,
	B: 1,
	A: 1,
}
