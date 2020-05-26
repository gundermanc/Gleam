// Gleam caret
// By: Christian Gunderman

package main

type Caret interface {
	Key(action KeyAction, key Key, character rune)
	MoveLeft()
	MoveRight()
	MoveUp()
	MoveDown()
	Position() Position
}

type caret struct {
	buffer   TextBuffer
	position Position
}

func NewCaret(buffer TextBuffer) Caret {
	return &caret{
		buffer,
		Position{
			Line:  0,
			Index: 0,
		},
	}
}

func (caret *caret) Position() Position {
	return caret.position
}

func (caret *caret) MoveLeft() {
	if caret.Position().Index > 0 {
		caret.position.Index--
	} else if caret.Position().Line > 0 {
		curLine := caret.Position().Line
		newLine := curLine - 1
		caret.position.Line = newLine
		caret.position.Index = uint(len(caret.buffer.Line(newLine)))
	}
}

func (caret *caret) MoveRight() {
	curLine := caret.Position().Line
	curLineLength := uint(len(caret.buffer.Line(curLine)))

	if caret.Position().Index < curLineLength {
		caret.position.Index++
	} else if curLine < caret.buffer.Lines()-1 {
		caret.position.Line = curLine + 1
		caret.position.Index = 0
	}
}

func (caret *caret) MoveUp() {
	if caret.Position().Line > 0 {
		caret.position.Line--

		curLineLength := uint(len(caret.buffer.Line(caret.Position().Line)))

		// Adjust offset to be within the current line.
		if caret.position.Index > curLineLength {
			caret.position.Index = curLineLength
		}
	}
}

func (caret *caret) MoveDown() {
	if caret.Position().Line < caret.buffer.Lines()-1 {
		caret.position.Line++

		curLineLength := uint(len(caret.buffer.Line(caret.Position().Line)))

		// Adjust offset to be within the current line.
		if caret.position.Index > curLineLength {
			caret.position.Index = curLineLength
		}
	}
}

func (caret *caret) Key(action KeyAction, key Key, character rune) {

	if action != KeyActionPress && action != KeyActionRepeat {
		return
	}

	switch key {
	case KeyLeft:
		caret.MoveLeft()
	case KeyRight:
		caret.MoveRight()
	case KeyUp:
		caret.MoveUp()
	case KeyDown:
		caret.MoveDown()
	}
}
