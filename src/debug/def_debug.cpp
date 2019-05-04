//
// Created by cynder on 20.10.17.
//
#include "def_debug.h"

#include <cstring>

assert_failed::assert_failed(const char *s, const char *file, unsigned long line, int err_code):
    message(s), error_code(err_code), allocated((file) || (line))
{
    if(allocated)
    {
        char *line_s = nullptr;
        unsigned long line_l = 0, file_l = 0, s_len = strlen(s);
        if(file)
            file_l = strlen(file) + 6;
        if(line)
        {
            line_s = new char[(sizeof(unsigned long) & 1) * 3 + (sizeof(unsigned long) >> 1) * 5 + 1];
            sprintf(line_s, "%lu", line);
            line_l = strlen(line_s) + 6;
            if(file_l)
                file_l += 2;
        }
        char *msg = new char[s_len + file_l + line_l + 4];
        strcpy(msg, s);
        strcpy(msg + s_len, " (");
        if(file)
        {
            strcpy(msg + s_len + 2, "file: ");
            strcpy(msg + s_len + 8, file);
            if(line)
                strcpy(msg + s_len + file_l, ", ");
        }
        if(line_s)
        {
            strcpy(msg + s_len + 2 + file_l, "line: ");
            strcpy(msg + s_len + 8 + file_l, line_s);
            delete[] line_s;
        }
        strcpy(msg + s_len + 2 + file_l + line_l, ")");
        message = msg;
    }
}

assert_failed::~assert_failed()
{
    if(allocated)
        delete[] message;
}

const char *assert_failed::what() const noexcept
{
    return message;
}

int assert_failed::err_code() const noexcept
{
    return error_code;
}
