#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    // Stałe parametry
    int n1 = 50;       // pierwszy graf
    int n2 = 5;       // drugi graf
    int tests = 30;    // liczba testów
    string solverPath = "C:\\src\\github\\sandrawar\\kSubgraphFinder\\main.exe";

    // Różne wartości k
    vector<int> k_values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 30, 40};

    ofstream results("results_k.csv");
    results << "k,avg_time_ms\n";

    for (int k : k_values) {
        string cmd = "benchmark.exe " + to_string(n1) + " " + to_string(n2) + " " +
                     to_string(k) + " " + to_string(tests) + " " + solverPath +
                     " > tmp_output.txt";

        cout << "Running: " << cmd << endl;
        system(cmd.c_str());

        ifstream tmp("tmp_output.txt");
        string line;
        double avg = 0.0;
        while (getline(tmp, line)) {
            if (line.find("ŚREDNI CZAS:") != string::npos) {
                size_t pos = line.find(":");
                string numberPart = line.substr(pos+1);
                numberPart.erase(remove_if(numberPart.begin(), numberPart.end(),
                                           [](unsigned char c){ return !isdigit(c) && c!='.'; }),
                                 numberPart.end());
                avg = stod(numberPart);
            }
        }
        tmp.close();

        results << k << "," << avg << "\n";
        cout << "k=" << k << " avg=" << avg << " ms\n";
    }

    results.close();
    cout << "Wyniki zapisane w results_k.csv\n";
    return 0;
}
