// Gleam TextView
// By: Christian Gunderman

package main

type TextView interface {
	Control
	TextBuffer() TextBuffer
	Caret() Caret
}

type textView struct {
	Control
	textBuffer TextBuffer
	caret      Caret
}

func NewTextView(textBuffer TextBuffer) TextView {
	return &textView{
		caret:      NewCaret(textBuffer),
		textBuffer: textBuffer,
	}
}

func (textView *textView) TextBuffer() TextBuffer {
	return textView.textBuffer
}

func (textView *textView) Caret() Caret {
	return textView.caret
}

func (textView *textView) Key(action KeyAction, key Key, character rune) {
	textView.Caret().Key(action, key, character)
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

			// Draw caret if needed.
			caretPos := textView.Caret().Position()
			if caretPos.Line == i {

				// Determine the size of the text ahead of the caret.
				horizOffset, _, err := renderContext.ComputeTextDimensions(line[0:caretPos.Index], 20)

				if err == nil {
					instructions = append(
						instructions,
						rectInstruction(green, horizOffset, verticalOffset, 2, height))
				}
			}

			verticalOffset += height
		}
	}

	return instructions
}
