// Gleam TextBuffer
// By: Christian Gunderman

// TODO: this is a stand-in for the final text buffer.
// It is designed to have a similar interface, but is
// slower than one based on a balanced binary tree, for example.

package main

import (
	"errors"
	"strings"
)

type Position struct {
	Line  uint
	Index uint
}

type TextBuffer interface {
	Insert(position Position, text string) error
	Delete(position Position, count uint) error
	Line(lineNumber uint) string
	Lines() uint
	String() string
}

type textBuffer struct {
	length uint
	lines  []textLine
}

type textLine struct {
	lineText   string
	lineEnding string
}

func NewTextBuffer(text string) TextBuffer {

	var lines []textLine
	var length uint
	var lineStart uint
	var currentRune rune

	for i, currentRune := range text {
		switch currentRune {
		case '\r':
		case '\n':
			currentLine := textLine{
				lineText:   text[lineStart:i],
				lineEnding: string(currentRune), // TODO: does not handle compound line endings (\r\n)
			}
			length += uint(i) - lineStart
			lines = append(lines, currentLine)
			lineStart = uint(i) + 1
		}
	}

	if textLen := uint(len(text)); lineStart != textLen {
		currentLine := textLine{
			lineText:   text[lineStart:textLen],
			lineEnding: string(currentRune), // TODO: does not handle compound line endings (\r\n)
		}
		length += textLen - lineStart
		lines = append(lines, currentLine)
		lineStart = textLen + 1
	}

	return &textBuffer{
		length: length,
		lines:  lines,
	}
}

func (textBuffer *textBuffer) Insert(position Position, text string) error {

	// Make sure that this position exists in the buffer.
	if position.Line >= textBuffer.Lines() ||
		position.Index >= uint(len(textBuffer.Line(position.Line))) {

		return errors.New("Invalid position in TextBuffer")
	}

	oldLineText := textBuffer.Line(position.Line)
	newLineText := oldLineText[0:position.Index] + text + oldLineText[position.Index:]

	textBuffer.lines[position.Line] = textLine{
		newLineText,
		textBuffer.lines[position.Line].lineEnding,
	}

	return nil
}

func (textBuffer *textBuffer) Delete(position Position, count uint) error {
	// Make sure that this position exists in the buffer.
	if position.Line >= textBuffer.Lines() ||
		position.Index >= uint(len(textBuffer.Line(position.Line))) {

		return errors.New("Invalid position in TextBuffer")
	}

	if count < 0 {
		return errors.New("Count must be positive")
	}

	oldLineText := textBuffer.Line(position.Line)
	newLineText := oldLineText[0:position.Index] + oldLineText[position.Index+count:]

	textBuffer.lines[position.Line] = textLine{
		newLineText,
		textBuffer.lines[position.Line].lineEnding,
	}

	return nil
}

func (textBuffer *textBuffer) Line(lineNumber uint) string {
	return textBuffer.lines[lineNumber].lineText
}

func (textBuffer *textBuffer) Lines() uint {
	return uint(len(textBuffer.lines))
}

func (textBuffer *textBuffer) String() string {
	var builder strings.Builder

	for _, textLine := range textBuffer.lines {
		builder.WriteString(textLine.lineText)
		builder.WriteString(textLine.lineEnding)
	}

	return builder.String()
}
