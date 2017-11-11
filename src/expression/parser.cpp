#include "parser.h"

static unsigned parsePosition, bracketsLevel;

const char *operations = "-+*/^";
const char *spaces = " \t\n";

parseException::parseException(string msg, unsigned position) : msg(msg.c_str()), position(position)
{
    message();
}

void parseException::message()
{
    fputs(msg, stderr);
    fprintf(stderr, " at %u\n", position);
}

static bool inRange(char c, const char *s, unsigned len)
{
    for(unsigned i = 0; i < len; i++)
        if(c == s[i])
            return true;
    return false;
}

static void skipSpaces(string expression)
{
    while(parsePosition < expression.length() && inRange(expression[parsePosition], spaces, 3))
        parsePosition++;
}

static Expression parseString(string expression, int priority);

static Expression parseOneValue(string expression)
{
    bool unaryMinus = false;
    Expression result;
    while(parsePosition < expression.length() && expression[parsePosition] == '-')
    {
        unaryMinus = !unaryMinus;
        parsePosition++;
    }
    if(parsePosition >= expression.length())
        throw new parseException("Unexpected end of expression", 0);
    if('0' <= expression[parsePosition] && expression[parsePosition] <= '9')
    {
        unsigned startPosition = parsePosition;
        double number = expression[parsePosition++] - '0';
        if(number == 0 && parsePosition < expression.length() && '0' <= expression[parsePosition] &&
           expression[parsePosition] <= '9')
            throw new parseException("Number can't contain following zeroes", startPosition);
        while(parsePosition < expression.length() && '0' <= expression[parsePosition] &&
              expression[parsePosition] <= '9')
            number = number * 10 + (expression[parsePosition++] - '0');
        if(parsePosition < expression.length() && expression[parsePosition] == '.')
        {
            parsePosition++;
            if(parsePosition == expression.length() || expression[parsePosition] < '0' ||
               expression[parsePosition] > '9')
                throw new parseException("There must be at least one digit after point", parsePosition);
            double numberF = (expression[parsePosition++] - '0') / 10;
            while(parsePosition < expression.length() && '0' <= expression[parsePosition] &&
                  expression[parsePosition] <= '9')
                numberF = (numberF + expression[parsePosition++] - '0') / 10;
            number += numberF;
        }
        result = new Const(unaryMinus ? -number : number);
        unaryMinus = false;
    } elif('a' <= expression[parsePosition] && expression[parsePosition] <= 'z')
    {
        unsigned startPosition = parsePosition++;
        while(parsePosition < expression.length() && 'a' <= expression[parsePosition] &&
              expression[parsePosition] <= 'z')
            parsePosition++;
        string varFuncName = expression.substr(startPosition, parsePosition - startPosition);
        if(varFuncName == "abs")
            result = new Abs(parseString(expression, 4));
        elif(varFuncName == "sqrt")
            result = new Sqrt(parseString(expression, 4));
        elif(varFuncName == "sin")
            result = new Sin(parseString(expression, 4));
        elif(varFuncName == "cos")
            result = new Cos(parseString(expression, 4));
        elif(varFuncName == "tg")
            result = new Tg(parseString(expression, 4));
        elif(varFuncName == "ctg")
            result = new Ctg(parseString(expression, 4));
        elif(varFuncName == "ln")
            result = new Ln(parseString(expression, 4));
        elif(varFuncName == "exp")
            result = new Exp(parseString(expression, 4));
        elif(varFuncName != "x")
            throw new parseException("Undefined function/variable " + varFuncName, startPosition);
        else
            result = new Var();
    } elif(expression[parsePosition] == '(')
    {
        unsigned bracketPosition = parsePosition++;
        bracketsLevel++;
        result = parseString(expression, 0);
        if(parsePosition >= expression.length() || expression[parsePosition++] != ')')
            throw new parseException("Can't find closing bracket for bracket", bracketPosition);
        bracketsLevel--;
    } else
        throw new parseException("Expression expected", parsePosition);
    skipSpaces(expression);
    return unaryMinus ? Expression(new Negate(result)) : result;
}

static int parseBinaryOperation(string expression, int priority)
{
    unsigned startPosition = parsePosition;
    if(!inRange(expression[parsePosition], operations, 5))
        throw new parseException("Expexted binary operation", parsePosition);
    if(expression[parsePosition++] != '-')
        while(parsePosition < expression.length() && expression[parsePosition] == expression[startPosition])
            parsePosition++;
    string operation = expression.substr(startPosition, parsePosition - startPosition);
    int result = -1;
    if(priority < 0)
        priority = -priority - 1;
    if(operation == "-")
    {
        if(priority < 1)
            result = 0;
    } elif(operation == "+")
    {
        if(priority < 1)
            result = 1;
    } elif(operation == "/")
    {
        if(priority < 2)
            result = 2;
    } elif(operation == "*")
    {
        if(priority < 2)
            result = 3;
    } elif(operation == "**" || operation == "^")
    {
        if(priority < 3)
            result = 4;
    } else
        throw new parseException("Undefined binary operation:'" + operation + "'", startPosition);
    if(result >= 0)
        skipSpaces(expression);
    else
        parsePosition = startPosition;
    return result;
}

static Expression parseString(string expression, int priority)
{
    skipSpaces(expression);
    Expression result = parseOneValue(expression);
    while(parsePosition < expression.length() && !(bracketsLevel > 0 && expression[parsePosition] == ')'))
    {
        int binaryOperationType = parseBinaryOperation(expression, priority);
        switch(binaryOperationType)
        {
            case -1:
                return result;
            case 0:
                result = new Sub(result, parseString(expression, 1));
                break;
            case 1:
                result = new Add(result, parseString(expression, 1));
                break;
            case 2:
                result = new Div(result, parseString(expression, 2));
                break;
            case 3:
                result = new Mult(result, parseString(expression, 2));
                break;
            case 4:
                result = new Pwr(result, parseString(expression, -3));
                break;
        }
    }
    return result;
}

Expression parse(string s)
{
    parsePosition = 0, bracketsLevel = 0;
    return parseString(s, 0);
}
