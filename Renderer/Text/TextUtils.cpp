#include "TextUtils.h"

int GetLengthOfLineBreak(const std::string& str, size_t position)
{
    switch (str[position])
    {
    case '\r':
    {
        if ((position + 1) < str.size() && str[position + 1] == '\n')
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
    case '\n':
        return 1;
    default:
        return 0;
    }
}
