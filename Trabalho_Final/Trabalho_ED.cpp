#include <iostream>
#include <cstring>
#include <fstream>
#include "Pacote.h"
#include "QuickSort.h"
#include "MergeSortExterno.h"
#include "Config.h"
#include <sstream>
using namespace std;


/*struct Pacote {
    unsigned indice;
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char informacao[100];
};*/




void le_arquivo_bin(fstream& arquivo) {
    Pacote umPacote;
    int linhas = 0;
    while (arquivo.read((char*)&umPacote, sizeof(umPacote))) {
        cout << umPacote.tempo << "; ";
        cout << umPacote.protocolo << "; ";
        cout << endl;
        linhas++;
    }
}

int main() {

    MergeSortExterno mergeSortExterno;
    fstream arquivo_bin_read(NomeArquivoSaida, ios::in|ios::binary);
    le_arquivo_bin(arquivo_bin_read);
   
    mergeSortExterno.OrdenarExternamente(NomeArquivoEntrada);

    return 0;
}


