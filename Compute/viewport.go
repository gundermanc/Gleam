// Gleam viewport
// By: Christian Gunderman

package main

// Control is the base type for all Gleam UI elements.
type Control interface {
	Parent() *Control
	X() uint
	Y() uint
	Width() uint
	Height() uint
	Render(renderContext RenderContext, maxWidth uint, maxHeight uint) []instruction
}

type control struct {
	parent   *control
	x        uint
	y        uint
	width    uint
	height   uint
	children []control
}

func (control *control) X() uint {
	return control.x
}

func (control *control) Y() uint {
	return control.y
}

func (control *control) Width() uint {
	return control.width
}

func (control *control) Height() uint {
	return control.height
}
