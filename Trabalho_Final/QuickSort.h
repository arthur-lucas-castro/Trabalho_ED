#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "Pacote.h"
#include <string.h>
class QuickSort {
public:
    QuickSort(Pacote* pacotes) {
        this->pacotes = pacotes;
    }
    void Quicksort(int start, int end);
private:
    Pacote* pacotes;
    int Particionar(const int start, const int end);
    void Trocar(int i, int j);
};
#endif