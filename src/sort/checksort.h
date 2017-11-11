#pragma once

#define COUNTSWAPS

#include "../other/rand.h"
#include "../other/arraymethods.hpp"

#include <time.h>
#include <unistd.h>
#include <stdio.h>

double getClocksPerCount();

void
checksortfa(unsigned *s, unsigned mas_len, void(sortf)(unsigned *, unsigned *), double CPCI = 0.00071407856279258859);

void checksort(void(sortf)(unsigned *, unsigned *));
