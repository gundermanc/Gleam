// Gleam input primitives
// By: Christian Gunderman

package main

// Based on https://www.glfw.org/docs/latest/group__keys.html
// However, since GLFW is an implementation detail, we don't
// take any direct dependency on it.

type KeyAction uint

const (
	KeyActionRelease       KeyAction = 0 // GLFW_RELEASE
	KeyActionPress         KeyAction = 1 // GLFW_PRESS
	KeyActionRepeat        KeyAction = 2 // GFLW_REPEAT
	KeyActionTypeCharacter KeyAction = 3
)

type Key uint

const (
	KeyNone Key = 0

	KeyBackspace Key = 259 // GLFW_KEY_BACKSPACE

	KeyRight Key = 262 // GLFW_KEY_RIGHT.
	KeyLeft  Key = 263 // GLFW_KEY_LEFT.
	KeyDown  Key = 264 // GLFW_KEY_DOWN.
	KeyUp    Key = 265 // GLFW_KEY_UP.
)
