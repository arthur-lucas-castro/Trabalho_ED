#include <iostream>
#include <cstring>
#include <fstream>

#include <sstream>
using namespace std;
#define N 1000
#define NomeArquivoTemporarioPadrao "arquivoTemp"
#define NomeArquivoEntrada "captura_pacotes.bin"
#define NomeArquivoSaida "ordenado.bin"

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
public:
    QuickSort(pacote *pacotes) {
        this->pacotes = pacotes;
    }
    void Quicksort(const int start, const int end);


private:
    pacote *pacotes;
    int Particionar(const int start, const int end);
    void Trocar( int i,  int j);
};

class MergeSortExterno {
    friend class QuickSort;
    friend class Arquivo;
public:
    void OrdenarExternamente(const char* nome)
    {
        int numArqs = CriaArquivosOrdenados(nome);
        int bufferPorArquivo = N / (numArqs + 1);
        Merge(numArqs, bufferPorArquivo, "ordenado.bin");
        //ApagaArquivosTemporarios(numArqs);
    }
private:
    int ProcuraMenor(Arquivo *arquivo, int tamanhoBufferPorArquivo, pacote* menor, int numArquivos);
    void AchaPosicaoMenor(int numArquivos, Arquivo* arquivos, int& posicaoMenor);
    void Merge(int numArquivos, int tamanhoBufferPorArquivo, string nome);
    void LiberarMemoria(int numArquivos, Arquivo* arquivos, pacote* bufferDeSaida);
    void PreencherObjetosArquivos(int numArquivos, Arquivo* arquivos, int tamanhoBufferPorArquivo);
    int CriaArquivosOrdenados(string nome);
    int ResecreverEmArquivosDivididos(fstream& arquivo);
    void PreencheBuffer(Arquivo* arquivo, int tamanhoBufferPorArquivo);
    void ApagaArquivosTemporarios(int numArqs)
    {
        for (int quantidadeArquivos = 1; quantidadeArquivos <= numArqs; quantidadeArquivos++)
        {
            stringstream name;
            name << NomeArquivoTemporarioPadrao << quantidadeArquivos << ".bin";
            remove(name.str().c_str());
        }
    }
};
void le_arquivo_bin(fstream& arquivo) {
    pacote umPacote;
    int linhas = 0;
    while (arquivo.read((char*)&umPacote, sizeof(umPacote))) {
        cout << umPacote.tempo << "; ";
        cout << umPacote.protocolo << "; ";
        cout << endl;
        linhas++;
    }
}
void le_arquivo_bin(fstream& arquivo, int &linhas ) {
    pacote umPacote;
    while (arquivo.read((char*)&umPacote, sizeof(umPacote))) {
        linhas++;
    }
}
int MergeSortExterno::CriaArquivosOrdenados(string nome)
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
            name << NomeArquivoTemporarioPadrao << quantidadeArquivos << ".bin";
            ArquivoTemp.open(name.str(), ios::out| ios::binary);
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
        name << NomeArquivoTemporarioPadrao << quantidadeArquivos << ".bin";
        ArquivoTemp.open(name.str(), ios::out);
        ArquivoTemp.write((char*)&pacotes, sizeof(pacotes));
        ArquivoTemp.close();
        totalItens = 0;
    }
    return quantidadeArquivos;
}
void MergeSortExterno::Merge(int numArquivos, int tamanhoBufferPorArquivo, string nome) {
    pacote* bufferDeSaida = new pacote[tamanhoBufferPorArquivo];
    Arquivo* arquivos;
    arquivos = new Arquivo[numArquivos];
    PreencherObjetosArquivos(numArquivos, arquivos, tamanhoBufferPorArquivo);
    int qtdBuffer = 0, teste = 0;
    pacote menor;
    while (ProcuraMenor(arquivos, tamanhoBufferPorArquivo, &menor, numArquivos) == 1)
    {
        bufferDeSaida[qtdBuffer] = menor;
        qtdBuffer++;
        teste++;
        if (qtdBuffer == tamanhoBufferPorArquivo) {
            fstream arquivoTemp(NomeArquivoSaida, ios::binary | ios::out | ios::app);
            for (int i = 0; i < qtdBuffer; i++)
                arquivoTemp.write((char*)&bufferDeSaida[i], sizeof(bufferDeSaida[i]));
            arquivoTemp.close();
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
    LiberarMemoria(numArquivos, arquivos, bufferDeSaida);;
    
}
void MergeSortExterno::LiberarMemoria(int numArquivos, Arquivo* arquivos, pacote* bufferDeSaida)
{
    for (int i = 0; i < numArquivos; i++)
    {
        delete[] arquivos[i].buffer;
    }
    delete[] arquivos;
    delete[] bufferDeSaida;
}
void MergeSortExterno::PreencherObjetosArquivos(int numArquivos, Arquivo* arquivos, int tamanhoBufferPorArquivo)
{
    for (int i = 0; i < numArquivos; i++)
    {
        stringstream name;
        name << NomeArquivoTemporarioPadrao << i + 1 << ".bin";
        arquivos[i].arquivo.open(name.str(), ios::binary | ios::out | ios::in);
        arquivos[i].max = 0;
        arquivos[i].pos = 0;
        arquivos[i].buffer = new pacote[tamanhoBufferPorArquivo];
        PreencheBuffer(&arquivos[i], tamanhoBufferPorArquivo);
    }
}
void MergeSortExterno::PreencheBuffer(Arquivo* arquivo, int tamanhoBufferPorArquivo) {
    if (!arquivo->arquivo.is_open())
        return;
    arquivo->pos = 0;
    arquivo->max = 0;
    pacote umPacote;
    int tamanhoBufferCarregado = 0;
    while (!arquivo->arquivo.eof() && tamanhoBufferCarregado < tamanhoBufferPorArquivo)
    {
        arquivo->arquivo.read((char*)&umPacote, sizeof(umPacote));
        if (!arquivo->arquivo.eof()) {
            arquivo->buffer[arquivo->max] = umPacote;
            arquivo->max++;
            tamanhoBufferCarregado++;
        }

    }
    if (arquivo->arquivo.eof()) {
        arquivo->arquivo.close();
    }
       

}

int MergeSortExterno::ProcuraMenor(Arquivo *arquivos, int tamanhoBufferPorArquivo, pacote* menor, int numArquivos) {
    int posicaoMenor = -1;
    AchaPosicaoMenor(numArquivos, arquivos, posicaoMenor);
    if(posicaoMenor != -1)
    {
        *menor = arquivos[posicaoMenor].buffer[arquivos[posicaoMenor].pos];
        arquivos[posicaoMenor].pos++;
        if (arquivos[posicaoMenor].pos == arquivos[posicaoMenor].max)
            PreencheBuffer(&arquivos[posicaoMenor], tamanhoBufferPorArquivo);
        return 1;
    }
    else
        return 0;
}

void MergeSortExterno::AchaPosicaoMenor(int numArquivos, Arquivo* arquivos, int& posicaoMenor)
{
    for (int i = 0; i < numArquivos; i++)
    {
        if (arquivos[i].pos < arquivos[i].max) {
            if (posicaoMenor == -1)
                posicaoMenor = i;
            else {
                if (strcmp(arquivos[i].buffer[arquivos[i].pos].protocolo, arquivos[posicaoMenor].buffer[arquivos[posicaoMenor].pos].protocolo) <= 0) {
                    if (strcmp(arquivos[i].buffer[arquivos[i].pos].protocolo, arquivos[posicaoMenor].buffer[arquivos[posicaoMenor].pos].protocolo) == 0) {
                        if (arquivos[i].buffer[arquivos[i].pos].tempo >= arquivos[posicaoMenor].buffer[arquivos[posicaoMenor].pos].tempo) {
                            posicaoMenor = i;
                        }
                    }
                    else {
                        posicaoMenor = i;
                    }
                }
            }
        }
    }
}





int main() {
    MergeSortExterno mergeSortExterno;
    //fstream arquivo_bin_read(NomeArquivoSaida, ios::in|ios::binary);
    //le_arquivo_bin(arquivo_bin_read);
    mergeSortExterno.OrdenarExternamente(NomeArquivoEntrada);

    return 0;
}

void QuickSort::Quicksort(const int start, const int end)
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
    pacote pacoteAux = pacotes[i];
    pacotes[i] = pacotes[j];
    pacotes[j] = pacoteAux;
}
