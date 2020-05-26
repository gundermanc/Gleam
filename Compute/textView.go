// Gleam TextView
// By: Christian Gunderman

package main

type TextView interface {
	Control
	TextBuffer() TextBuffer
}

type textView struct {
	Control
	textBuffer TextBuffer
}

func NewTextView(textBuffer TextBuffer) TextView {
	return &textView{
		textBuffer: textBuffer,
	}
}

func (textView *textView) TextBuffer() TextBuffer {
	return textView.textBuffer
}

func (textView *textView) Render(renderContext RenderContext, maxWidth uint, maxHeight uint) []instruction {
	var instructions []instruction
	var verticalOffset uint

	for i := uint(0); i < textView.TextBuffer().Lines(); i++ {

		line := textView.TextBuffer().Line(i)

		width, height, err := renderContext.ComputeTextDimensions(line, 20)

		// TODO: report err?
		if err == nil {
			instructions = append(
				instructions,
				textInstruction(line, green, 20, 0, verticalOffset, width, height))

			verticalOffset += height
		}
	}

	return instructions
}
