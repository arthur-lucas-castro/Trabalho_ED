
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "Pacote.h"
#include <fstream>
using namespace std;
class Arquivo {
public:
    fstream arquivo;
    int pos, max;
    Pacote* buffer;
};
#endif