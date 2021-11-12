#include <iostream>
#include <cstring>
#include <fstream>

#include <sstream>
using namespace std;
#define N 1000
#define NomeArquivoPadrao "arquivoTemp"
#define NomeArquivoEntrada "captura_pacotes.bin"

// dados armazenados no arquivo

struct pacote {
    unsigned indice;
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char informacao[100];
};
class QuickSort {
    friend class MergeSortExterno;
public:
    QuickSort(pacote *pacotes) {
        this->pacotes = pacotes;
    }
    void Quicksort(const int start, const int end);


private:
    pacote *pacotes;
    int Partition(const int start, const int end);
    void Swap( int i,  int j);
};

class MergeSortExterno {
    friend class QuickSort;
public:
    void OrdenarExternamente(const char* nome)
    {
        string novo;
        int numArqs = criaArquivosOrdenados(nome);
        int buffers = N / (numArqs + 1);

    };
private:
    void Merge();
    int criaArquivosOrdenados(string nome);
    int ResecreverEmArquivosDivididos(fstream& arquivo);
};
void le_arquivo_bin(ifstream& arquivo) {
    pacote umPacote;
    int linhas = 0;
    while (arquivo.read((char*)&umPacote, sizeof(umPacote))) {
        cout << umPacote.indice << "; ";
        cout << umPacote.tempo << "; ";
        cout << umPacote.origem << "; ";
        cout << umPacote.destino << "; ";
        cout << umPacote.protocolo << "; ";
        cout << umPacote.tamanho << "; ";
        cout << umPacote.informacao << endl;
        linhas++;
    }
    cout << "\n\n\n" << linhas;
}
void le_arquivo_bin(ifstream& arquivo, int *linhas ) {
    pacote umPacote;
    while (arquivo.read((char*)&umPacote, sizeof(umPacote))) {
        cout << umPacote.indice << "; ";
        cout << umPacote.tempo << "; ";
        cout << umPacote.origem << "; ";
        cout << umPacote.destino << "; ";
        cout << umPacote.protocolo << "; ";
        cout << umPacote.tamanho << "; ";
        cout << umPacote.informacao << endl;
        linhas++;
    }
    cout << "\n\n\n" << linhas;
}
int MergeSortExterno::criaArquivosOrdenados(string nome)
{
    fstream arquivo(nome, fstream::out | fstream::in | fstream::binary);  
    return ResecreverEmArquivosDivididos(arquivo);
}
int MergeSortExterno::ResecreverEmArquivosDivididos(fstream& arquivo)
{
    pacote pacotes[N];
    int quantidadeArquivos = 0, totalItens = 0;;
    fstream ArquivoTemp;
    while (arquivo.read((char*)&pacotes[totalItens], sizeof(pacote))) {  
        totalItens++;
        if (totalItens == N) {   
            QuickSort quicksort(pacotes);
            quicksort.Quicksort(0, totalItens - 1);
            quantidadeArquivos++;
            stringstream name;
            name << NomeArquivoPadrao << quantidadeArquivos << ".bin";
            ArquivoTemp.open(name.str(), ios::out);
            ArquivoTemp.write((char*)&pacotes, sizeof(pacotes));
            ArquivoTemp.close();
            totalItens = 0;
        }
    }
    if (totalItens > 0) {
        QuickSort quicksort(pacotes);
        quicksort.Quicksort(0, totalItens - 1);
        quantidadeArquivos++;
        stringstream name;
        name << NomeArquivoPadrao << quantidadeArquivos << ".bin";
        ArquivoTemp.open(name.str(), ios::out);
        ArquivoTemp.write((char*)&pacotes, sizeof(pacotes));
        ArquivoTemp.close();
        totalItens = 0;
    }
    return quantidadeArquivos;
}




int main() {
    MergeSortExterno mergeSortExterno;
    int count = 0;
    ifstream arquivo_bin_read(NomeArquivoEntrada, ios::binary);
    //le_arquivo_bin(arquivo_bin_read);
    mergeSortExterno.OrdenarExternamente(NomeArquivoEntrada);
    /*for (int i = 1; i < 100; i++)
    {
        stringstream name;
        name << NomeArquivoPadrao << i << ".bin";
        ifstream arquivo_bin_read(name.str(), ios::binary);
        le_arquivo_bin(arquivo_bin_read, &count);
    }*/

    return 0;
}

void QuickSort::Quicksort(const int start, const int end)
{
    if (start >= end) return;

    int pivot = Partition(start, end);

    Quicksort(start, pivot - 1);
    Quicksort(pivot + 1, end);
}

int QuickSort::Partition(const int start, const int end)
{
    int i = start;
    for (int j = start; j < end; j++) {

        /* Elemento atual menor ou igual ao pivô? */
        if (pacotes[j].protocolo <= pacotes[end].protocolo) {
            if (pacotes[j].protocolo == pacotes[end].protocolo) {
                if (pacotes[j].tempo >= pacotes[end].tempo) {
                    Swap(i++, j);
                }
            }
            else {
                Swap(i++, j);
            }
            
        }
    }
    Swap(i, end);

    return i;
}

void QuickSort::Swap(int i, int j)
{
    pacote k = pacotes[i];
    pacotes[i] = pacotes[j];
    pacotes[j] = k;
}
