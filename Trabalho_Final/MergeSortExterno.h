#ifndef MERGESORTEXTERNO_H
#define MERGESORTEXTERNO_H
#include "Pacote.h"
#include "QuickSort.h"
#include "Arquivo.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
class MergeSortExterno {
    friend class QuickSort;
    friend class Arquivo;
public:
    void OrdenarExternamente(const char* nome);
private:
    int ProcuraMenor(Arquivo* arquivo, int tamanhoBufferPorArquivo, Pacote* menor, int numArquivos);
    void AchaPosicaoMenor(int numArquivos, Arquivo* arquivos, int& posicaoMenor);
    void Merge(int numArquivos, int tamanhoBufferPorArquivo, string nome);
    void LiberarMemoria(int numArquivos, Arquivo* arquivos, Pacote* bufferDeSaida);
    void PreencherObjetosArquivos(int numArquivos, Arquivo* arquivos, int tamanhoBufferPorArquivo);
    int CriaArquivosOrdenados(string nome);
    int ResecreverEmArquivosDivididos(fstream& arquivo);
    void PreencheBuffer(Arquivo* arquivo, int tamanhoBufferPorArquivo);
    void ApagaArquivosTemporarios(int numArqs);
};
#endif