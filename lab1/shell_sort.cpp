
//***********************************//
//     LABORATORIO 1 - SHELL SORT    //
//     trshpnd 2024                  //
//***********************************//

#include <iostream>
#include <string.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <intrin.h>
#include <vector>

using namespace std;

#define SHELL_SEQ_SIZE  21
#define KNUTH_SEQ_SIZE  14
#define CIURA_SEQ_SIZE  17
#define MAX_SEQ_SIZE    SHELL_SEQ_SIZE

#define DIR_ENTRADA1    "entradas\\entrada1.txt"
#define DIR_ENTRADA2    "entradas\\entrada2.txt"
#define DIR_SAIDA1      "saida1.txt"
#define DIR_SAIDA2      "saida2.txt"

enum option{
    SHELL,
    KNUTH,
    CIURA
};

typedef struct Sequence{
    string sequence_name;
    vector<int> sequence_array;
    int sequence_size;
}Sequence;

vector<int> shell_array = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576};
vector<int> knuth_array = {1,4,13,40,121,364,1093,3280,9841,29524,88573,265720,797161,2391484,0,0,0,0,0,0,0};
vector<int> ciura_array = {1,4,10,23,57,132,301,701,1577,3548,7983,17961,40412,90927,204585,460316,1035711,0,0,0,0};

void print_array(vector<int> &numbers, string file_dir){
    ofstream output(file_dir, ios::app);
    for(int number : numbers){
        //cout << number << " ";
        output << number << " ";
    }
}

void swap_numbers(vector<int> &numbers, int origin_index, int dest_index){
    int aux = numbers[dest_index];
    numbers[dest_index] = numbers[origin_index];
    numbers[origin_index] = aux;
}


bool is_smaller(int a, int b){
    if(a - b < 0){
        return true;
    }else return false;
}

vector<int> readLineFromFile(int lineNumber, const string& filename, int& line_length) {
    vector<int> numbers;
    ifstream file(filename);

    if (!file) {
        cerr << "Não foi possível abrir o arquivo." << endl;
        return numbers;
    }

    string line;
    for (int i = 0; i < lineNumber; ++i) {
        if (!getline(file, line)) {
            cerr << "Erro ao ler a linha do arquivo. (EOF)" << endl;
            return numbers;
        }
    }

    istringstream iss(line);
    iss >> line_length;

    int number;
    while (iss >> number) {
        numbers.push_back(number);
    }
    return numbers;
}

string GetCpuInfo() {
    int cpuInfo[4] = {};
    char cpuBrandString[0x40] = {};

    __cpuid(cpuInfo, 0x80000000);
    unsigned nExIds = cpuInfo[0];

    for (unsigned i = 0x80000000; i <= nExIds; ++i) {
        __cpuid(cpuInfo, i);

        if  (i == 0x80000002)
            memcpy(cpuBrandString, cpuInfo, sizeof(cpuInfo));
        else if  (i == 0x80000003)
            memcpy(cpuBrandString + 16, cpuInfo, sizeof(cpuInfo));
        else if  (i == 0x80000004)
            memcpy(cpuBrandString + 32, cpuInfo, sizeof(cpuInfo));
    }

    return cpuBrandString;
}

void shell_sort(vector<int> numbers, int size, enum option seq_type, string file_dir){
    int h = 0;
    vector<int> toBeSorted = numbers;
    ofstream output(file_dir, ios::app);

    Sequence This;

    // Define os atributos do array da sequência a ser utilizado. SHELL | KNUTH | CIURA
    switch(seq_type){
        case SHELL:
            This.sequence_name = "SHELL";
            This.sequence_size = SHELL_SEQ_SIZE;
            This.sequence_array = shell_array;
            break;

        case KNUTH:
            This.sequence_name = "KNUTH";
            This.sequence_size = KNUTH_SEQ_SIZE;
            This.sequence_array = knuth_array;
            break;

        case CIURA:
            This.sequence_name = "CIURA";
            This.sequence_size = CIURA_SEQ_SIZE;
            This.sequence_array = ciura_array;
            break;

        default: // Caso não reconheça o enumerador informado, realiza ordenação por sequência shell.
            This.sequence_name = "SHELL";
            This.sequence_size = SHELL_SEQ_SIZE;
            This.sequence_array = shell_array;
            break;

    }

    if(file_dir == DIR_SAIDA1){
        print_array(toBeSorted, file_dir);
        //cout    << "SEQ=" << This.sequence_name << endl;
        output  << "SEQ=" << This.sequence_name << endl;
    }

    // Definição do tamanho máximo de partição. Varia de acordo com o tamanho do array a ser ordenado.
    while (This.sequence_array[h+1] < size){
        h++;
    }

    // Início do processo de ordenação proprimente dito.
    auto start = chrono::high_resolution_clock::now();

    while (h >= 0){ // Iteração das posições do array da sequência selecionada.
        for (int i = This.sequence_array[h]; i < size; i++){
            for (int j = i; j>= This.sequence_array[h] && is_smaller(toBeSorted[j], toBeSorted[j-This.sequence_array[h]]); j -= This.sequence_array[h])
            swap_numbers(toBeSorted, j, j - This.sequence_array[h]);
        }

        if(file_dir == DIR_SAIDA1){ // Print no arquivo saida1.txt
            print_array(toBeSorted, file_dir);
            //cout    << "INCR=" << This.sequence_array[h] << endl;
            output  << "INCR=" << This.sequence_array[h] << endl;
        }
        h--; // decrementa o iterador do array da sequência.
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double, milli>(end - start);

    if(file_dir == DIR_SAIDA2){
        output << This.sequence_name << "," << size << "," << duration.count() << "," << GetCpuInfo() << endl;
    }
}

int main(){
    int tam, linha = 1;

    if (remove(DIR_SAIDA1) != 0 || remove(DIR_SAIDA2) != 0) {
        perror("Erro ao excluir arquivo.\n");
    } else {
        puts("Arquivo excluido com sucesso.\n");
    }

    cout << "Inicio do teste 1." << endl;
    while(true){
        vector<int> numeros = readLineFromFile(linha, DIR_ENTRADA1, tam);
        if(numeros.empty()) break;
        cout << "Leitura da linha " << linha << " concluida: " << tam << " numeros lidos." << endl;
        shell_sort(numeros, tam, SHELL, DIR_SAIDA1);
        cout << "SHELL, " << tam << " finalizado." << endl;
        shell_sort(numeros, tam, KNUTH, DIR_SAIDA1);
        cout << "KNUTH, " << tam << " finalizado." << endl;
        shell_sort(numeros, tam, CIURA, DIR_SAIDA1);
        cout << "CIURA, " << tam << " finalizado." << endl;
        linha++;
    }
    cout << "Fim do teste 1. " << "Resultados salvos em \"" << DIR_SAIDA1 << "\"" << endl << endl;

    linha = 1;

    cout << "Inicio do teste 2." << endl;
    while(true){
        vector<int> numeros = readLineFromFile(linha, DIR_ENTRADA2, tam);
        if(numeros.empty()) break;
        cout << "Leitura da linha " << linha << " concluida: " << tam << " numeros lidos." << endl;
        shell_sort(numeros, tam, SHELL, DIR_SAIDA2);
        cout << "SHELL, " << tam << " finalizado." << endl;
        shell_sort(numeros, tam, KNUTH, DIR_SAIDA2);
        cout << "KNUTH, " << tam << " finalizado." << endl;
        shell_sort(numeros, tam, CIURA, DIR_SAIDA2);
        cout << "CIURA, " << tam << " finalizado." << endl;
        linha++;
    }
    cout << "Fim do teste 2. " << "Resultados salvos em \"" << DIR_SAIDA2 << "\"" << endl;

    return 0;
}