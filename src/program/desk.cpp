//#define windows

#include <stdio.h>
#include <vector>

#ifdef windows
#include <windows.h>
#endif  // windows

using namespace std;

#ifdef windows
HANDLE hCon;
#endif // windows

unsigned long long fact(int n)  //factorial of a number
{
    unsigned long long ret = 1;
    for(int i = 2; i <= n; i++)
        ret *= i;
    return ret;
}

struct desk
{
    char **deskFigures, **deskOccupy;   //desk with figures, desk with occupation counter for each cell
    int x, y, size, a, f;   //pointer position(x, y), size of the desk, amount of prints, amount of figures
    vector<pair<int, int> > *s; //stack of placed figures
    bool stackIsUsed;   //if stack is needed
    desk(int size, bool stackIsUsed = true) : x(0), y(0), size(size), a(0), f(0), stackIsUsed(stackIsUsed)
    {
        deskFigures = new char *[size];
        for(int i = 0; i < size; i++)
            deskFigures[i] = new char[size];
        deskOccupy = new char *[size];
        for(int i = 0; i < size; i++)
            deskOccupy[i] = new char[size];
        if(stackIsUsed)
            s = new vector<pair<int, int> >;
    }

    ~desk()
    {
        for(int i = 0; i < size; i++)
            delete deskFigures[i];
        delete deskFigures;
        for(int i = 0; i < size; i++)
            delete deskOccupy[i];
        delete deskOccupy;
        if(stackIsUsed)
            delete s;
    }

    void setKnight(int x, int y, int z) //increase all cells ocuupied by knight at (x, y) on z
    {
        if(x + 2 < size)
        {
            if(y - 1 >= 0)
                deskOccupy[x + 2][y - 1] += z;
            if(y + 1 < size)
                deskOccupy[x + 2][y + 1] += z;
        }
        if(y + 2 < size)
        {
            if(x - 1 >= 0)
                deskOccupy[x - 1][y + 2] += z;
            if(x + 1 < size)
                deskOccupy[x + 1][y + 2] += z;
        }
        if(x - 2 >= 0)
        {
            if(y - 1 >= 0)
                deskOccupy[x - 2][y - 1] += z;
            if(y + 1 < size)
                deskOccupy[x - 2][y + 1] += z;
        }
        if(y - 2 >= 0)
        {
            if(x - 1 >= 0)
                deskOccupy[x - 1][y - 2] += z;
            if(x + 1 < size)
                deskOccupy[x + 1][y - 2] += z;
        }
    }

    void setBishop(int x, int y, int z) //increase all cells occupied by bishop at (x, y) on z
    {
        for(int ix = 0, iy1 = y - x, iy2 = y + x; ix < size; ix++, iy1++, iy2--)
            if(ix != x)
            {
                if((iy1 >= 0) and (iy1 < size))
                    deskOccupy[ix][iy1] += z;
                if((iy2 >= 0) and (iy2 < size))
                    deskOccupy[ix][iy2] += z;
            }
    }

    void setRook(int x, int y, int z)   //increase all cells occupied by rook at (x, y) on z
    {
        for(int i = 0; i < size; i++)
        {
            if(i != x)
                deskOccupy[i][y] += z;
            if(i != y)
                deskOccupy[x][i] += z;
        }
    }

    void setKing(int x, int y, int z)   //increase all cells occupied by king at (x, y) on z
    {
        for(int ix = x - 1; ix <= x + 1; ix++)
            for(int iy = y - 1; iy <= y + 1; iy++)
                if((ix >= 0) and (ix < size) and (iy >= 0) and (iy < size) and ((ix != 0) or (iy != 0)))
                    deskOccupy[ix][iy] += z;
    }

    void setFigure(int x, int y, int z, char figure)    //icrease all cells occupied by figure
    //and (x, y) on z
    {
        deskOccupy[x][y] += z;
        switch(figure)
        {
            case 'N':
                setKnight(x, y, z);
                break;
            case 'B':
                setBishop(x, y, z);
                break;
            case 'R':
                setRook(x, y, z);
                break;
            case 'Q':
                setBishop(x, y, z);
                setRook(x, y, z);
                break;
            case 'K':
                setKing(x, y, z);
                break;
        }
    }

    bool putFigure(int x, int y, char figure)   //place figure on desk and update occupaied cells
    {
        if(deskFigures[x][y] > 0)
            return false;
        deskFigures[x][y] = figure;
        if(stackIsUsed)
            s->push_back(make_pair(x, y));
        setFigure(x, y, 1, figure);
        f++;
        return true;
    }

    int cellsLeft() //amount of cells after pointer
    {
        return (size - y) * size - x;
    }

    bool putNextFree(char figure)   //attempt to put figure on the next free(and not occupied) place
    {
        if(y >= size)
            return false;
        while(deskOccupy[x][y] > 0)
        {
            if(++x == size)
            {
                if(++y == size)
                    return false;
                x = 0;
            }
        }
        return putFigure(x, y, figure);
    }

    void removeFigure(int x, int y) //remove figure from (x, y)
    {
        if(deskFigures[x][y] != 0)
            setFigure(x, y, -1, deskFigures[x][y]);
        deskFigures[x][y] = 0;
        f--;
    }

    void removeBishop(int diag, bool color, bool left)  //remove bishop
    {
        if(color)   //true - white, false - black
        {
            if(diag > 0)    //diag - number of diagonal
            {
                int diff = left ? 0 : diag * 2;
                removeFigure(diff, diag * 2 - diff);
                removeFigure(size - diff - 1, size - diag * 2 + diff - 1);
            } else if(left)    //determine position of the bishop on the diagonal
                removeFigure(0, 0);
            else
                removeFigure(size - 1, size - 1);
        } else
        {
            if(diag > 0)
            {
                int diff = left ? 0 : diag * 2;
                removeFigure(size - diff - 1, diag * 2 - diff);
                removeFigure(diff, size - diag * 2 + diff - 1);
            } else if(left)
                removeFigure(0, size - 1);
            else
                removeFigure(size - 1, 0);
        }
    }

    void putBishop(int diag, bool color, bool left) //place bishop
    {
        if(color)
        {
            if(diag > 0)
            {
                int diff = left ? 0 : diag * 2;
                putFigure(diff, diag * 2 - diff, 'B');
                putFigure(size - diff - 1, size - diag * 2 + diff - 1, 'B');
            } else if(left)
                putFigure(0, 0, 'B');
            else
                putFigure(size - 1, size - 1, 'B');
        } else
        {
            if(diag > 0)
            {
                int diff = left ? 0 : diag * 2;
                putFigure(size - diff - 1, diag * 2 - diff, 'B');
                putFigure(diff, size - diag * 2 + diff - 1, 'B');
            } else if(left)
                putFigure(0, size - 1, 'B');
            else
                putFigure(size - 1, 0, 'B');
        }
    }

    void swapBishops(int diag, bool color, bool left)   //move pair bishops to another borders
    {
        removeBishop(diag, color, left);
        putBishop(diag, color, not(left));
    }

    void figureBefore() //remove one figure from stack and change pointer to the next position after figure was placed
    {
        pair<int, int> c = s->back();
        s->pop_back();
        x = c.first;
        y = c.second;
        removeFigure(x, y);
        if(++x == size)
        {
            x = 0;
            y++;
        }
    }

    void clear()    //clear the desk
    {
        for(int ix = 0; ix < size; ix++)
            for(int iy = 0; iy < size; iy++)
            {
                deskFigures[ix][iy] = 0;
                deskOccupy[ix][iy] = 0;
            }
        x = 0;
        y = 0;
        if(stackIsUsed)
            s->empty();
        f = 0;
    }

    void show() //show the desk
    {
        a++;
        printf("%i:\n", a);
#ifdef windows
        unsigned background = 4;
        SetConsoleTextAttribute(hCon, background);
#else
        printf("\x1b[31m");
        char background = 47;
#endif  //windows
        for(int y = 0; y < size; y++)
            for(int x = 0; x < size; x++)
            {
#ifdef windows
                SetConsoleTextAttribute(hCon, background);
#else
                printf("\x1b[%d\\m", background);
#endif  //windows
                if(deskFigures[x][y] == 0)
                    putchar(' ');
                else
                    putchar(deskFigures[x][y]);
#ifdef windows
                if(x < size - 1)
                    background = background == 4 ? 116 : 4;
                else
                    SetConsoleTextAttribute(hCon, 7);
#else
                if(x < size - 1)
                    background = background == 47 ? 40 : 47;
                else
                {
                    printf("\x1b[0m\n");
                    if(y < size - 1)
                        printf("\x1b[31m");
                }
#endif // windows
            }
#ifdef windows
        SetConsoleTextAttribute(hCon, 7);
#endif // windows
    }
};

int m = -1; //0 for number of arrangements, 1 for all variants

unsigned lookForFigure(int size, int amount, char figure, bool show = true) //brute-force
{
    desk *d = new desk(size);
    unsigned ret = 0;
    while((d->f > 0) or (d->cellsLeft() >= amount))
        if(not(d->putNextFree(figure)))
        {
            if(d->f == amount)
            {
                if(show)
                    d->show();
                ret++;
            }
            d->figureBefore();
        }
    delete d;
    return ret;
}

void lookForKnight(int size, int amount)    //arrangements of knights
{
    if(m == 0)
    {
        printf("Amount of arrangements: ");
        if(size == 2)
            printf("1\n");
        else if(size == 4)
            printf("6\n");
        else
            printf("2\n");
    } else
    {
        desk *d = new desk(size);
        if(size == 2)
        {
            d->putFigure(0, 0, 'N');
            d->putFigure(0, 1, 'N');
            d->putFigure(1, 0, 'N');
            d->putFigure(1, 1, 'N');
            d->show();
        } else if(size == 4)
            lookForFigure(size, amount, 'N');
        else
        {
            for(int x = 0; x < size; x++)
                for(int y = 0; y < size; y++)
                    if(x + y % 2 == 0)
                        d->putFigure(x, y, 'N');
            d->show();
            d->clear();
            for(int x = 0; x < size; x++)
                for(int y = 0; y < size; y++)
                    if(x + y % 2 == 1)
                        d->putFigure(x, y, 'N');
            d->show();
        }
    }
}

void lookForBishop(int size, int amount)    //arrangements of bishops
{
    if(m == 0)
        printf("Amount of arrangements: %llu", (unsigned long long) 1 << size);
    else
    {
        desk *d = new desk(size, false);
        bool *b = new bool[size];
        for(int dw = 0; dw < size / 2; dw++)
        {
            d->putBishop(dw, true, false);
            d->putBishop(dw, false, false);
        }
        int p = size - 1;
        for(;;)
        {
            d->show();
            while(b[p])
            {
                d->swapBishops(p % (size / 2), p < size / 2, true);
                b[p--] = false;
                if(p < 0)
                    return;
            }
            b[p] = true;
            d->swapBishops(p % (size / 2), p < size / 2, false);
            p = size - 1;
        }
    }
}

void lookForRook(int size, int amount)  //arrangements of rooks
{
    if(m == 0)
        printf("Amount of arrangements: %llu", fact(size));
    else
        lookForFigure(size, amount, 'R');
}

void lookForQueen(int size, int amount) //arrangements of queens
{
    if(m == 0)
        printf("Amount of arrangements: %u", lookForFigure(size, amount, 'Q', false));
    else
        lookForFigure(size, amount, 'Q');
}

void lookForKing(int size, int amount)  //arrangements of kings
{
    if(m == 0)
        printf("Amount of arrangements: %u", lookForFigure(size, amount, 'K', false));
    else
        lookForFigure(size, amount, 'K');
}

int desk_main()
{
    int s = -1, f = -1; //s - size, f - figure
#ifdef windows
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
#endif // windows
    sizeInput:
    printf("Enter size of the desk:\n");
    scanf("%i", &s);
    if((s <= 0) or (s % 2 != 0))
    {
        printf("Size must be above 0 and even\n");
        goto sizeInput;
    }
    modeInput:
    printf("Enter mode:\n\t0: Print amount of results\n\t1: Print all results\n");
    scanf("%i", &m);
    if((m < 0) or (m > 1))
    {
        printf("Mode can be 0 or 1\n");
        goto modeInput;
    }
    figureInput:
    printf("Enter figure to be placed\n\t0: Knight (N)\n\t1: Bishop (B)\n\t2: Rook (R)\n\t3: Queen (Q)\n\t4: King (K)\n");
    scanf("%i", &f);
    if((f < 0) or (f > 4))
    {
        printf("Figure number ranges from 0 to 4\n");
        goto figureInput;
    }
    switch(f)
    {
        case 0:
            if(s == 2)
                lookForKnight(s, 4);
            else
                lookForKnight(s, s * s / 2);
            break;
        case 1:
            lookForBishop(s, 2 * s - 2);
            break;
        case 2:
            lookForRook(s, s);
            break;
        case 3:
            if(s == 2)
                lookForQueen(s, 1);
            else
                lookForQueen(s, s);
            break;
        case 4:
            lookForKing(s, (s / 2) * (s / 2));
            break;
    }
    return 0;
}
