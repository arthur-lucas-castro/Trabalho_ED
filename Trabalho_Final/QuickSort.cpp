#include "QuickSort.h"

void QuickSort::Quicksort(int start, int end)
{
    if (start >= end) return;

    int pivo = Particionar(start, end);

    Quicksort(start, pivo - 1);
    Quicksort(pivo + 1, end);
}

int QuickSort::Particionar(const int start, const int end)
{
    int i = start;
    for (int j = start; j < end; j++) {

        if (strcmp(pacotes[j].protocolo, pacotes[end].protocolo) <= 0) {
            if (strcmp(pacotes[j].protocolo, pacotes[end].protocolo) == 0) {
                if (pacotes[j].tempo >= pacotes[end].tempo) {
                    Trocar(i++, j);
                }
            }
            else {
                Trocar(i++, j);
            }
        }
    }
    Trocar(i, end);

    return i;
}

void QuickSort::Trocar(int i, int j)
{
    Pacote pacoteAux = pacotes[i];
    pacotes[i] = pacotes[j];
    pacotes[j] = pacoteAux;
}