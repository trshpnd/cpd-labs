// Lab 4: Hash tables
// trshpnd 2024

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>

using namespace std;

#define DATA_DIR    "arquivos-suporte//players.csv"
#define SEARCH_DIR  "arquivos-suporte//consultas.csv"  

struct Player {
    int id;
    string name;
    vector<string> position;

};

struct Result{
    int id;
    string name;
    int queries;
};

unsigned long long polynomialHash(int number, int mod, int base = 31) {
    unsigned long long hashValue = 0;
    unsigned long long basePower = 1;

    while (number > 0) {
        int digit = number % 10;
        hashValue = (hashValue + digit * basePower) % mod;
        basePower = (basePower * base) % mod;
        number /= 10;
    }

    return hashValue;

}

Player readPlayer(const string& line) {
    stringstream ss(line);
    string item;
    Player player;

    // Read ID
    getline(ss, item, ',');
    player.id = stoi(item);

    // Read Name
    getline(ss, item, ',');
    player.name = item;

    // Read Positions
    getline(ss, item, ',');
    stringstream posStream(item);
    string pos;
    while (getline(posStream, pos, ',')) {
        player.position.push_back(pos);
    }

    return player;

}

vector<Player> readPlayersFromCSV(const string& filename) {
    ifstream file(filename);
    vector<Player> players;
    string line;

    // Ignore the header
    getline(file, line);

    // Read each line and parse the player
    while (getline(file, line)) {
        Player player = readPlayer(line);
        players.push_back(player);
    }

    return players;

}

vector<int> readSearchFromCSV(const string& filename) {
    ifstream file(filename);
    vector<int> searches;
    string line;

    while (getline(file, line)){
        int search = stoi(line);
        searches.push_back(search); 
    }

    return searches;

}

vector<vector<Player>> buildHash(vector<vector<Player>> hashTable, vector<Player> players, bool verbose = false){
    int notEmpty =0;
    int maxListSize =0;
    int entries =0;

    cout << "Verbose mode:";

    if(verbose != false){
        cout << " On"                                   << endl;
        cout << "Hash table data allocation started."   << endl;
    }
    else cout << " Off" << endl;

    auto start = chrono::high_resolution_clock::now();

    for (const auto& player : players){
        int hashIndex = polynomialHash(player.id, hashTable.size());
        hashTable[hashIndex].push_back(player);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if(verbose != false) cout << "Hash table data allocation finished." << endl;

    for (size_t i = 0; i < hashTable.size(); ++i){

        if (!hashTable[i].empty()) {
            if(hashTable[i].size() > maxListSize) maxListSize = hashTable[i].size();
            notEmpty++;
            if(verbose != false) cout << "Index " << i << ": ";

            for (const auto& player : hashTable[i]) {
                if(verbose != false) cout << player.id << " ";
                entries++;
            }

            if(verbose != false) cout << endl;
        }

    }
    
    stringstream outputDirectory;
    outputDirectory << "experimento" << hashTable.size() << ".txt";
    ofstream file(outputDirectory.str());

    if(!file){
        cerr << "Erro ao abrir arquivo de saída." << endl;
        return hashTable;
    }


    cout << "---------------------------------------------"                                << endl;
    cout << "PARTE 1: ESTATISTICAS DA TABELA HASH"                                         << endl;
    cout << "TEMPO DE CONSTRUCAO DA TABELA "       << duration.count() << " ms"            << endl;
    cout << "TAXA DE OCUPACAO "                    << (double) notEmpty/hashTable.size()   << endl;
    cout << "TAMANHO MAXIMO DE LISTA "             << maxListSize                          << endl;
    cout << "TAMANHO MEDIO DE LISTA "              << entries/notEmpty                     << endl;
    cout << "---------------------------------------------"                                << endl;

    file << "PARTE 1: ESTATISTICAS DA TABELA HASH"                                          << endl;
    file << "TEMPO DE CONSTRUCAO DA TABELA "       << duration.count() << " ms"             << endl;
    file << "TAXA DE OCUPACAO "                    << (double) notEmpty/hashTable.size()    << endl;
    file << "TAMANHO MAXIMO DE LISTA "             << maxListSize                           << endl;
    file << "TAMANHO MEDIO DE LISTA "              << entries/notEmpty                      << endl;
    file << endl;

    return hashTable;
}

void searchHash(vector<vector<Player>> hashTable, vector<int> searches, bool verbose = false) {
    vector<Result> results;
    Result res;
    int allQueries, maxQueries;
    maxQueries =0;
    allQueries =0;

    if(verbose != false) cout << "Searching for entries. " << endl;

    auto start = chrono::high_resolution_clock::now();

    for(size_t i = 0; i < searches.size(); ++i){
        int queryCount =0;
        bool found =false;

        int hashIndex = polynomialHash(searches[i], hashTable.size());
        //cout << hashTable[hashIndex].empty() << " " << hashIndex << endl; //dbg

        if(!hashTable[hashIndex].empty()){

            for(const auto& player : hashTable[hashIndex]){
            
                queryCount++;

                //cout << player.id << endl; //dbg
                //cout << searches[i] << endl;
                //cout << (player.id == searches[i]) << endl;

                if(player.id == searches[i]){
                    
                    res.name = player.name;
                    found = true;
                    if(queryCount > maxQueries) maxQueries = queryCount; // Atualizar maxQueries apenas por nome ENCONTRADO.

                    break;
                }

            }

            //cout << res.name << endl; //dbg

        }

        if(found != true){
            res.name = "NAO ENCONTRADO";
        }

        res.id = searches[i];
        res.queries = queryCount;
        allQueries = allQueries + queryCount;

        results.push_back(res);

    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if(verbose != false) cout << "Search complete. " << endl;

    stringstream outputDirectory;
    outputDirectory << "experimento" << hashTable.size() << ".txt";
    ofstream file;
    file.open(outputDirectory.str(), std::ios::app);

    if(!file.is_open()){
        cerr << "Erro ao abrir arquivo de saida." << endl;
        return;
    }

    cout << "---------------------------------------------"                                 << endl;
    cout << "PARTE 2: ESTATISTICAS DAS CONSULTAS"                                           << endl;
    cout << "TEMPO PARA REALIZACAO DE TODAS AS CONSULTAS " << duration.count() << " ms"     << endl;

    file << "PARTE 2: ESTATISTICAS DAS CONSULTAS"                                           << endl;
    file << "TEMPO PARA REALIZACAO DE TODAS AS CONSULTAS " << duration.count() << " ms"     << endl;

    for(const auto& result : results){
        if(verbose !=false) cout << result.id << " " << result.name << " " << result.queries                    << endl;
        file << result.id << " " << result.name << " " << result.queries                    << endl;
    }

    cout << "MAXIMO NUMERO DE TESTES POR NOME ENCONTRADO: " << maxQueries                   << endl;
    cout << "MEDIA NUMERO DE TESTES POR NOME ENCONTRADO: " << allQueries/searches.size()    << endl;
    cout << "---------------------------------------------"                                 << endl;

    file << "MAXIMO NUMERO DE TESTES POR NOME ENCONTRADO: " << maxQueries                   << endl;
    file << "MEDIA NUMERO DE TESTES POR NOME ENCONTRADO: " << allQueries/searches.size()    << endl;

}

int main(){

    int M[] = {997, 1999, 3989, 7993};

    for(const auto& mod : M){

        cout << "Experimento " << mod << endl;
        vector<vector<Player>> hashTable(mod);
        vector<Player> players = readPlayersFromCSV(DATA_DIR);
        vector<int> searches = readSearchFromCSV(SEARCH_DIR);
        hashTable = buildHash(hashTable, players);
        searchHash(hashTable, searches);
    }

    return 0;
}
