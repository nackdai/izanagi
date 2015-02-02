#include <stdio.h>
#include "Render2D.h"

void callback()
{
    printf("****\n");
}

int main(int argn, char* argc[])
{
    SetCallback(callback);
    return Hoge();
}
