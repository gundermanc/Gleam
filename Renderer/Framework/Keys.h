// Based on https://www.glfw.org/docs/latest/group__keys.html
// However, since GLFW is an implementation detail, we don't
// take any direct dependency on it.

enum class KeyAction
{
    Release = 0,     // GLFW_RELEASE
    Press = 1,       // GLFW_PRESS
    Repeat = 2,      // GFLW_REPEAT

    TypeCharacter = 3
};

enum class Key
{
    None = 0,

    Backspace = 259, // GLFW_KEY_BACKSPACE 

    Right = 262,     // GLFW_KEY_RIGHT.
    Left = 263,      // GLFW_KEY_LEFT.
    Down = 264,      // GLFW_KEY_DOWN.
    Up = 265,        // GLFW_KEY_UP.
};
