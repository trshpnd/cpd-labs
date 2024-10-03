//////////////////////////////////////////
// CLASSIFICAÇÃO E PESQUISA DE DADOS    //
// LABORATÓRIO 3: RADIX SORT            //
// trshpnd 2024                         //
//////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Par{
    string str;
    int num;
};

const int ASCII_RANGE = 256;
const int BASE = 10;

void msd_radix_sort_strings(vector<string>& arr, int left, int right, int d) {  // Adaptação da versão do sedgewick & wayne para c++. 
    if (left >= right) return;                                                  // Utiliza vector para manipulação de array dinâmico.

    vector<int> count(ASCII_RANGE + 2, 0);
    vector<string> aux(right - left + 1);

    for (int i = left; i <= right; ++i) {
        int c = d < arr[i].size() ? arr[i][d] + 1: 0;
        count[c + 1]++;
    }

    for (int r = 0; r < ASCII_RANGE +1; ++r) {
        count[r + 1] += count[r];
    }

    for (int i = left; i <= right; ++i) {
        int c = d < arr[i].size() ? arr[i][d] + 1: 0;
        aux[count[c]++] = arr[i];
    }

    for (int i = left; i <= right; ++i) {
        arr[i] = aux[i - left];
    }

    for (int r = 0; r < ASCII_RANGE; ++r) {
        msd_radix_sort_strings(arr, left + count[r], left + count[r + 1] - 1, d + 1);
    }
}

void radix_sort_strings(vector<string>& arr) {          // Inicializador da função de ordenação (para strings)
    msd_radix_sort_strings(arr, 0, arr.size() - 1, 0);
}

void msd_radix_sort_pares(vector<Par>& arr, int left, int right, int exp) {
    if (left >= right || exp == 0) return;

    vector<vector<Par>> buckets(BASE);
    for (int i = left; i <= right; ++i) {
        int digit = (arr[i].num / exp) % BASE;
        buckets[digit].push_back(arr[i]);
    }

    int index = left;
    for (int i = BASE - 1; i >= 0; --i) {
        for (const auto& p : buckets[i]) {
            arr[index++] = p;
        }
    }

    index = left;
    for (int i = BASE - 1; i >= 0; --i) {
        if (buckets[i].size() > 1) {
            msd_radix_sort_pares(arr, index, index + buckets[i].size() - 1, exp / BASE);
        }
        index += buckets[i].size();
    }
}

void radix_sort_pares(vector<Par>& arr) {       // Inicializador da função de ordenação (para pares)
    int maxNum = 0;
    for (const auto& p : arr) {
        if (p.num > maxNum) {
            maxNum = p.num;
        }
    }

    int exp = 1;
    while (maxNum / exp >= BASE) {
        exp *= BASE;
    }

    msd_radix_sort_pares(arr, 0, arr.size() - 1, exp);
}

void lerPalavras(const string& nomeArquivo, vector<string>& palavras){
    ifstream file(nomeArquivo);
    string palavra;

    if(!file){
        cerr << "Erro ao abrir arquivo de entrada." << endl;
        return;// palavras;
    }

    while(file >> palavra){
        palavras.push_back(palavra);
    }

    file.close();
}

void lerPares(const string& nomeArquivo, vector<Par>& pares){
    ifstream file(nomeArquivo);
    string str;
    int num;

    if(!file){
        cerr << "Erro ao abrir arquivo de entrada." << endl;
        return;
    }

    while(file >> str >> num){
        pares.push_back({str, num});
    }

    file.close();
}

void mostrarVetor(vector<string>& vetor){
    for(const string& elem : vetor){
        cout << elem << endl;
    }
}

void salvarVetor(const vector<string>& palavras, const string& nomeArquivo){
    ofstream file(nomeArquivo);

    if(!file){
        cerr << "Erro ao abrir arquivo de saída." << endl;
        return;
    }

    for(const string& palavra : palavras){
        file << palavra << endl;
    }

    file.close();
}

void salvarPares(const vector<Par>& pares, const string& nomeArquivo){ // Salvar apenas os primeiros 2000 pares.
    ofstream file(nomeArquivo);
    int maxPares = 2000;

    if(!file){
        cerr << "Erro ao abrir arquivo de saída." << endl;
        return;
    }

    for(int i = 0; i < maxPares; i++){
        file << pares[i].str << " " << pares[i].num << endl;
    }
}

void contarPalavras(const string& arquivoEntrada, const string& arquivoSaida){
    ifstream Entrada(arquivoEntrada);
    ofstream Saida(arquivoSaida);
    
    string palavraAtual;
    string palavraProx;
    int cont = 0;

    if(!Entrada){
        cerr << "Erro ao abrir arquivo de entrada." << endl;
        return;
    }

    if(!Saida){
        cerr << "Erro ao abrir arquivo de saida." << endl;
        return;
    }

    Entrada >> palavraAtual;
    cont = 1;

    while(Entrada >> palavraProx){
        if(palavraProx != palavraAtual){
            Saida << palavraAtual + " " << cont << endl;
            palavraAtual = palavraProx;
            cont = 1;
        }
        else cont++;
    }

    Saida << palavraAtual + " " << cont << endl;

    Entrada.close();
    Saida.close();
}

int main() {
    vector<string> arr; // = {"she", "sells", "seashells", "by", "the", "sea", "shore", "the", "shells", "she", "sells", "are", "surely", "seashells"};
    vector<Par> arrPares;

    string arquivoEntrada = "war_and_peace"; // Alterar somente com o nome do arquivo, sem a extensão txt.

    string dirEntrada = "entradas-2\\" + arquivoEntrada + ".txt";
    string dirSaida = "saidas\\" + arquivoEntrada + "_sorted.txt";

    lerPalavras(dirEntrada, arr);
    radix_sort_strings(arr);
    salvarVetor(arr, dirSaida);

    dirEntrada = dirSaida;
    dirSaida = "saidas\\" + arquivoEntrada + "_counted.txt";

    contarPalavras(dirEntrada, dirSaida);

    dirEntrada = dirSaida;
    dirSaida = "saidas\\" + arquivoEntrada + "_ranked.txt";

    lerPares(dirEntrada, arrPares);
    radix_sort_pares(arrPares);
    salvarPares(arrPares, dirSaida);

    return 0;
}