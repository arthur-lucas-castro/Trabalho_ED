#include "MergeSortExterno.h"

void MergeSortExterno::OrdenarExternamente(const char* nome)
{
    int numArqs = CriaArquivosOrdenados(nome);
    int bufferPorArquivo = N / (numArqs + 1);
    Merge(numArqs, bufferPorArquivo, "ordenado.bin");
    ApagaArquivosTemporarios(numArqs);
}
int MergeSortExterno::CriaArquivosOrdenados(string nome)
{
    try
    {
        fstream arquivo(nome, fstream::out | fstream::in | fstream::binary);
        return ResecreverEmArquivosDivididos(arquivo);
    }
    catch (const std::exception&)
    {
        cout << "Falha ao usar o arquivo " << nome << endl;
    }
    return 0;
  
}
int MergeSortExterno::ResecreverEmArquivosDivididos(fstream& arquivo)
{   
    Pacote pacotes[N];
    int quantidadeArquivos = 0, totalItens = 0;;
    fstream ArquivoTemp;
    while (arquivo.read((char*)&pacotes[totalItens], sizeof(Pacote))) {
        totalItens++;
        if (totalItens == N) {
            QuickSort quicksort(pacotes);
            quicksort.Quicksort(0, totalItens - 1);
            quantidadeArquivos++;
            stringstream name;
            name << NomeArquivoTemporarioPadrao << quantidadeArquivos << ".bin";
            ArquivoTemp.open(name.str(), ios::out | ios::binary);
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
    Pacote* bufferDeSaida = new Pacote[tamanhoBufferPorArquivo];
    Arquivo* arquivos;
    arquivos = new Arquivo[numArquivos];
    PreencherObjetosArquivos(numArquivos, arquivos, tamanhoBufferPorArquivo);
    int qtdBuffer = 0;
    Pacote menor;
    remove(NomeArquivoSaida);
    while (ProcuraMenor(arquivos, tamanhoBufferPorArquivo, &menor, numArquivos) == 1)
    {
        bufferDeSaida[qtdBuffer] = menor;
        qtdBuffer++;
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
void MergeSortExterno::LiberarMemoria(int numArquivos, Arquivo* arquivos, Pacote* bufferDeSaida)
{
    try
    {
        for (int i = 0; i < numArquivos; i++)
        {
            delete[] arquivos[i].buffer;
        }
        delete[] arquivos;
        delete[] bufferDeSaida;

    }
    catch (const std::exception&)
    {
        cout << "erro ao liberar a memoria" << endl;
    }
    
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
        arquivos[i].buffer = new Pacote[tamanhoBufferPorArquivo];
        PreencheBuffer(&arquivos[i], tamanhoBufferPorArquivo);
    }
}
void MergeSortExterno::PreencheBuffer(Arquivo* arquivo, int tamanhoBufferPorArquivo) {
    if (!arquivo->arquivo.is_open())
        return;
    arquivo->pos = 0;
    arquivo->max = 0;
    Pacote umPacote;
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

int MergeSortExterno::ProcuraMenor(Arquivo* arquivos, int tamanhoBufferPorArquivo, Pacote* menor, int numArquivos) {
    int posicaoMenor = -1;
    AchaPosicaoMenor(numArquivos, arquivos, posicaoMenor);
    if (posicaoMenor != -1)
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
void MergeSortExterno::ApagaArquivosTemporarios(int numArqs)
{
    for (int quantidadeArquivos = 1; quantidadeArquivos <= numArqs; quantidadeArquivos++)
    {
        stringstream name;
        name << NomeArquivoTemporarioPadrao << quantidadeArquivos << ".bin";
        remove(name.str().c_str());
    }
}
