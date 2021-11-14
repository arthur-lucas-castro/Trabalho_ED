#include <iostream>
#include <cstring>
#include <fstream>

#include <sstream>
using namespace std;
#define N 2000
#define NomeArquivoPadrao "arquivoTemp"
#define NomeArquivoEntrada "captura_pacotes.bin"
#define NomeArquivoSaida "ordenado.bin"
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
class Arquivo {
public:
    fstream arquivo;
    int pos, max;
    pacote* buffer;
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
    friend class Arquivo;
public:
    void OrdenarExternamente(const char* nome)
    {
        string novo;
        int numArqs = criaArquivosOrdenados(nome);
        int buffers = N / (numArqs + 1);
        Merge(numArqs, buffers, "ordenado.bin");

    };
private:
    int ProcuraMenor(Arquivo *arquivo, int K, pacote* menor, int numArquivos);
    void Merge(int numArquivos, int K, string nome);
    int criaArquivosOrdenados(string nome);
    int ResecreverEmArquivosDivididos(fstream& arquivo);
    void PreencheBuffer(Arquivo* arquivo, int K);
};
void le_arquivo_bin(fstream& arquivo) {
    pacote umPacote;
    int linhas = 0;
    while (!arquivo.eof()) {
        arquivo.read((char*)&umPacote, sizeof(umPacote));
        cout << umPacote.tempo << "; ";
        cout << umPacote.protocolo << "; ";
        linhas++;
    }
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
    int teste = 0;
    while (arquivo.read((char*)&pacotes[totalItens], sizeof(pacote))) {  
        totalItens++;
        if (totalItens == N) {   
            QuickSort quicksort(pacotes);
            quicksort.Quicksort(0, totalItens - 1);
            quantidadeArquivos++;
            stringstream name;
            name << NomeArquivoPadrao << quantidadeArquivos << ".bin";
            ArquivoTemp.open(name.str(), ios::out| ios::binary);
            ArquivoTemp.write((char*)&pacotes, sizeof(pacotes));
            ArquivoTemp.close();
            totalItens = 0;
        }
        teste++;
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
void MergeSortExterno::Merge(int numArquivos, int K, string nome) {
    pacote* bufferDeSaida = new pacote[K];
    Arquivo* arquivos;
    arquivos = new Arquivo[numArquivos];
    for (int i = 0; i < numArquivos; i++)
    {
        stringstream name;
        name << NomeArquivoPadrao << i+1 << ".bin"; 
        arquivos[i].arquivo.open(name.str(), ios::binary | ios::out | ios::in);
        arquivos[i].max = 0;
        arquivos[i].pos = 0;
        arquivos[i].buffer = new pacote[K];
        //le_arquivo_bin(arquivos[i].arquivo);
        PreencheBuffer(&arquivos[i], K);
    }
    int qtdBuffer = 0;
    pacote menor;
    int teste = 0;
    while (ProcuraMenor(arquivos, K, &menor, numArquivos) == 1)
    {
        bufferDeSaida[qtdBuffer] = menor;
        qtdBuffer++;
        teste++;
        if (qtdBuffer == K) {
            fstream ArquivoTemp(NomeArquivoSaida, ios::binary | ios::out | ios::app);
            for (int i = 0; i < qtdBuffer; i++)
            {
                ArquivoTemp.write((char*)&bufferDeSaida[i], sizeof(bufferDeSaida[i]));
            }
           
            ArquivoTemp.close();
            qtdBuffer = 0;
        }
    }
    if (qtdBuffer != 0) {
        fstream ArquivoTemp(NomeArquivoSaida, ios::binary | ios::out | ios::app);
        for (int i = 0; i < qtdBuffer; i++)
        {
            ArquivoTemp.write((char*)&bufferDeSaida[i], sizeof(bufferDeSaida[i]));
        }
        ArquivoTemp.close();
        qtdBuffer = 0;
    }
    
}
void MergeSortExterno::PreencheBuffer(Arquivo* arquivo, int K) {
    if (!arquivo->arquivo.is_open())
        return;
    arquivo->pos = 0;
    arquivo->max = 0;
    for (int i = 0; i < K; i++)
    {
        if(!arquivo->arquivo.eof())
        {
            arquivo->arquivo.read((char*)&arquivo->buffer[arquivo->max], sizeof(arquivo->buffer[arquivo->max]));
            arquivo->max++;
        }
        else
        {
            arquivo->arquivo.close();
            break;//trocar
        }
    }

}

int MergeSortExterno::ProcuraMenor(Arquivo *arquivos, int K, pacote* menor, int numArquivos) {
    int idx = -1;
    for (int i = 0; i < numArquivos; i++)
    {
        if (arquivos[i].pos < arquivos[i].max) {
            if (idx == -1)
                idx = i;
            else {
                if (strcmp(arquivos[i].buffer[arquivos[i].pos].protocolo, arquivos[idx].buffer[arquivos[idx].pos].protocolo) <= 0) {
                    if (arquivos[i].buffer[arquivos[i].pos].protocolo == arquivos[i].buffer[arquivos[idx].pos].protocolo) {
                        if (arquivos[i].buffer[arquivos[i].pos].tempo >= arquivos[i].buffer[arquivos[idx].pos].tempo) {
                            idx = i;
                        }
                    }
                    else {
                        idx = i;
                    }
                }
            }
        }
    }
    if( idx != -1)
    {
        *menor = arquivos[idx].buffer[arquivos[idx].pos];
        arquivos[idx].pos++;
        if (arquivos[idx].pos == arquivos[idx].max)
            PreencheBuffer(&arquivos[idx], K);
        return 1;
    }
    else
        return 0;
}





int main() {
    MergeSortExterno mergeSortExterno;
    int count = 0;
    //fstream arquivo_bin_read(NomeArquivoSaida, ios::in|ios::binary);
    //le_arquivo_bin(arquivo_bin_read);
    mergeSortExterno.OrdenarExternamente(NomeArquivoEntrada);
    /*for (int i = 1; i < 100; i++)
    {
        stringstream name;
        name << NomeArquivoPadrao << i << ".bin";
        fstream arquivo_bin_read(name.str(), ios::in | ios::binary);
        le_arquivo_bin(arquivo_bin_read);
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

        if (strcmp(pacotes[j].protocolo, pacotes[end].protocolo) <= 0) {
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
