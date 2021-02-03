#include "../other/defdef.h"
#include "../template/arraymethods.hpp"
#include "../string/cstr.h"
#include "../container/vector.hpp"
#include "../other/term.h"
#include "../debug/tests_run.h"

#include <unistd.h>
#include <wait.h>

#define SUBSHELL_VERSION "0.1"

const str def_paths[] = {"/bin", "/usr/bin", "."};

void show_version()
{
    printf("subshell: version %s\n", SUBSHELL_VERSION);
}

void show_help()
{
    puts("very simple shell");
    show_version();
    puts("enter program name to run or exit to exit");
}

void print_prompt()
{
    printf("$ ");
    fflush(stdin);
}

void show_error(const str& error_msg)
{
    color_printf(term_color::RED, error_msg.c_str());
    putchar('\n');
    fflush(stdin);
}

bool in_quotes(const str& s)
{
    char quote = '\0';
    bool escaped = false;
    for(char c: s)
        if(c == '\'')
        {
            if(quote == '\'')
                quote = '\0';
            elif(quote == '\0' && !escaped)
            quote = '\'';
        }
    elif(c == '\\' && quote != '\'')
    escaped = !escaped;
    elif(c == '"' && !escaped)
    {
        if(quote == '"')
            quote = '\0';
        elif(quote == '\0')
        quote = '"';
    }
    return (quote != '\0');
}

str readline()
{
    str line = read_str();
    while(true)
        if(in_quotes(line))
            line += "\n" + read_str();
        else
        {
            unsigned k = 0;
            unsigned long i = line.length();
            while(i > 0)
                if(line.at(--i) == '\\')
                    k++;
                else
                    break;
            if(k & 1u)
                line += read_str();
            else
                break;
        }
    return line;
}

str clear_quotes_backslashes(str& s)
{
    str r;
    bool escaped = false;
    for(const char c: s)
    {
        if(escaped || c != '\\')
        {
            r += c;
            escaped = false;
        }
        else
            escaped = true;
    }
    return r.compact();
}

vect<str> parse_command(str& line)
{
    vect<str> ans;
    char quote = '\0';
    bool escaped = false;
    unsigned long previous_index = 0;
    str arg;
    for(unsigned long i = 0; i < line.length(); i++)
    {
        const char c = line[i];
        if(c == '\'')
        {
            if(quote == '\'')
                quote = '\0';
            elif(quote == '\0' && !escaped)
                quote = '\'';
            else
                arg += c;
        }
        elif(c == '\\' && quote != '\'')
        {
            if(escaped)
                arg += c;
            escaped = !escaped;
        }
        elif(c == '"' && !escaped)
        {
            if(quote == '"')
                quote = '\0';
            elif(quote == '\0')
                quote = '"';
            else
                arg += c;
        }
        elif(c == ' ' && !escaped && quote == '\0')
        {
            if(i != previous_index)
                ans.push(arg);
            arg = "";
            previous_index = i + 1;
        }
        else
            arg += c;
    }
    if(previous_index != line.length())
        ans.push(arg);
    return ans;
}

bool is_command(str& path)
{
    return (access(path.c_str(), X_OK) == 0);
}

str get_command_path(str& command)
{
    for(const str& dir: def_paths)
    {
        str path = dir + "/" + command;
        if(is_command(path))
            return path;
    }
    show_error("No such command: " + command);
    return str();
}

void run_command(str& command_path, vect<str>& arguments)
{
    char **args = new char*[arguments.size() + 1];
    args[0] = arguments[0];
    args[arguments.size()] = nullptr;
    for(unsigned i = 1;i < arguments.size();i++)
        args[i] = arguments[i];

    int status;

    if(fork() == 0)
        execv(command_path, args);

    wait(&status);

    printf("%d\n", WEXITSTATUS(status));
    fflush(stdin);
}

int shell_main(int argc, char **argv)
{
    for(int i = 1;i < argc;i++)
        if(c_str_equals(argv[i], "--help") || c_str_equals(argv[i], "-h"))
        {
            show_help();
            return 0;
        }
        elif(c_str_equals(argv[i], "--version") || c_str_equals(argv[i], "-v"))
        {
            show_version();
            return 0;
        }
        else
        {
            show_error(str("Unknown argument: ") + argv[i]);
            return 1;
        }
    for(;;)
    {
        if(check_eof())
            break;
        print_prompt();
        str line = readline();
        vect<str> command = parse_command(line);
        if(command.size() == 0)
            continue;
        if(command[0] == "exit")
            break;
        str command_path = get_command_path(command[0]);
        if(command_path.length())
            run_command(command_path, command);
    }
}