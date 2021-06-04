#include "facade.h"

int main()
{
    Facade *f = new Facade();
    f->start();
    delete f;
    return 0;
}