// Gleam render instructions set
// By: Christian Gunderman

package main

type instruction struct {
	Type   instructionType `json:"type"`
	X      uint            `json:"x"`
	Y      uint            `json:"y"`
	Width  uint            `json:"width"`
	Height uint            `json:"height"`
	Params interface{}     `json:"params"`
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
	TextInstruction instructionType = 0

	// RectInstruction indicates that the renderer should draw a filled rectangle.
	RectInstruction instructionType = 1

	// RectOutlineInstruction indicates that the renderer should draw an outline of a rectangle.
	RectOutlineInstruction instructionType = 2
)

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
