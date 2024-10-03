//**************************************//
//     LABORATORIO 1 - INSERTION SORT   //
//     trshpnd 2024                     //
//**************************************//
#include <iostream>
#include <string.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <intrin.h>

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
    int sequence_array[MAX_SEQ_SIZE] = {0};
    int sequence_size;
}Sequence;

static int shell_array[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576};
static int knuth_array[] = {1,4,13,40,121,364,1093,3280,9841,29524,88573,265720,797161,2391484,0,0,0,0,0,0,0};
static int ciura_array[] = {1,4,10,23,57,132,301,701,1577,3548,7983,17961,40412,90927,204585,460316,1035711,0,0,0,0};

void print_array(int *numbers, int size, string file_dir){
    ofstream output(file_dir, ios::app);

    for(int i = 0; i < size; i++){
        //printf("%d ", numbers[i]);
        cout << numbers[i] << " ";
        output << numbers[i] << " ";
    }
}

void swap(int *array, int origin_index, int dest_index){
    int aux = array[dest_index];
    array[dest_index] = array[origin_index];
    array[origin_index] = aux;
}

bool is_smaller(int a, int b){
    if(a - b < 0){
        return true;
    }else return false;
}

int* readLineFromFile(int lineNumber, const string& filename, int& tamanho) {
    ifstream file(filename);
    if (!file) {
        //cerr << "Não foi possível abrir o arquivo." << endl;
        return nullptr;
    }

    string line;
    for (int i = 0; i < lineNumber; ++i) {
        if (!getline(file, line)) {
            //cerr << "Erro ao ler a linha do arquivo." << endl;
            return nullptr;
        }
    }

    istringstream iss(line);
    iss >> tamanho;

    int* array = new int[tamanho];
    for (int i = 0; i < tamanho; ++i) {
        if (!(iss >> array[i])) {
            //cerr << "Erro ao ler o valor da linha." << endl;
            delete[] array;
            return nullptr;
        }
    }

    return array;
}

string GetCpuInfo() {
    int cpuInfo[4] = {};
    char cpuBrandString[0x40] = {};

    // Get the information associated with each extended ID.
    __cpuid(cpuInfo, 0x80000000);
    unsigned nExIds = cpuInfo[0];

    for (unsigned i = 0x80000000; i <= nExIds; ++i) {
        __cpuid(cpuInfo, i);

        // Interpret CPU brand string and fill it in cpuBrandString
        if  (i == 0x80000002)
            memcpy(cpuBrandString, cpuInfo, sizeof(cpuInfo));
        else if  (i == 0x80000003)
            memcpy(cpuBrandString + 16, cpuInfo, sizeof(cpuInfo));
        else if  (i == 0x80000004)
            memcpy(cpuBrandString + 32, cpuInfo, sizeof(cpuInfo));
    }

    return cpuBrandString;
}

void insertion_sort(int *numbers, int size){
    int aux;
    for(int i = 1; i < size; i++){  // inicia o loop considerando que o elemento 0 está na pos correta.
        aux = numbers[i];           // seleciona o elemento para comparar com os anteriores.
        int j = i - 1;              // seleciona o elemento anterior ao da pos [i]
        while((j > 0) && (numbers[j] > aux)){   // enquanto o elemento anterior for maior que o selecionado, e a pos for válida (>0)
            numbers[j + 1] = numbers[j];        // o elemento assume a posição do selecionado (troca)
            j--;                                // decrementa o iterador [j] para a próxima comparação com 'aux'.
        }
        numbers[j + 1] = aux;       // o elemento selecionado está na sua posição final.
        //print_array(numbers, size);
    }
}

void shell_sort(int *numbers, int size, enum option seq_type, string file_dir){
    int h = 0;
    int toBeSorted[size] = {0};
    ofstream output(file_dir, ios::app);

    memcpy(toBeSorted, numbers, sizeof(toBeSorted));

    Sequence This;

    // Define os atributos do array da sequência a ser utilizado. SHELL | KNUTH | CIURA
    switch(seq_type){
        case SHELL:
            This.sequence_name = "SHELL";
            This.sequence_size = SHELL_SEQ_SIZE;
            memcpy(This.sequence_array, shell_array, sizeof(This.sequence_array));
            break;

        case KNUTH:
            This.sequence_name = "KNUTH";
            This.sequence_size = KNUTH_SEQ_SIZE;
            memcpy(This.sequence_array, knuth_array, sizeof(This.sequence_array));
            break;

        case CIURA:
            This.sequence_name = "CIURA";
            This.sequence_size = CIURA_SEQ_SIZE;
            memcpy(This.sequence_array, ciura_array, sizeof(This.sequence_array));
            break;

        default:
            This.sequence_name = "SHELL";
            This.sequence_size = SHELL_SEQ_SIZE;
            memcpy(This.sequence_array, shell_array, sizeof(This.sequence_array));
            break;

    }

    if(file_dir == DIR_SAIDA1){
        print_array(toBeSorted, size, file_dir);
        cout << "SEQ=" << This.sequence_name << endl;
        output << "SEQ=" << This.sequence_name << endl;
    }

    // Início do processo de ordenação proprimente dito.

    auto start = chrono::high_resolution_clock::now();

    // Definição do tamanho máximo de partição. Varia de acordo com o tamanho do array a ser ordenado.
    while (This.sequence_array[h+1] < size){
        h++; 
    }
    
    while (h >= 0){ // Iteração das posições do array da sequência selecionada.
        for (int i = This.sequence_array[h]; i < size; i++){
            for (int j = i; j>= This.sequence_array[h] && is_smaller(toBeSorted[j], toBeSorted[j-This.sequence_array[h]]); j -= This.sequence_array[h])
            swap(toBeSorted, j, j - This.sequence_array[h]);
        }

        if(file_dir == DIR_SAIDA1){ // Print no arquivo saida1.txt
            print_array(toBeSorted, size, file_dir);
            cout << "INCR=" << This.sequence_array[h] << endl;
            output << "INCR=" << This.sequence_array[h] << endl;
        }
        h--; // decrementa o iterador do array da sequência.
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if(file_dir == DIR_SAIDA2){
        output << This.sequence_name << "," << size << "," << duration.count() << "," << GetCpuInfo() << endl;
    }

}

int main(){
    int tam, linha = 1;

    if (remove(DIR_SAIDA1) != 0 || remove(DIR_SAIDA2) != 0) {
        perror("Error deleting file");
    } else {
        puts("File successfully deleted");
    }
    
    while(true){
        int *numeros = readLineFromFile(linha, DIR_ENTRADA1, tam);
        if(numeros == nullptr) break;
        shell_sort(numeros, tam, SHELL, DIR_SAIDA1);
        shell_sort(numeros, tam, KNUTH, DIR_SAIDA1);
        shell_sort(numeros, tam, CIURA, DIR_SAIDA1);
        linha++;
    }

    linha = 1;

    while(true){
        int *numeros = readLineFromFile(linha, DIR_ENTRADA2, tam);
        if(numeros == nullptr) break;
        shell_sort(numeros, tam, SHELL, DIR_SAIDA2);
        shell_sort(numeros, tam, KNUTH, DIR_SAIDA2);
        shell_sort(numeros, tam, CIURA, DIR_SAIDA2);
        linha++;
    }

    return 0;
}