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
    int m = 10;        // m
    int k = 1;         // k
    int tests = 30;    // liczba testów
    string solverPath = "C:\\src\\github\\sandrawar\\kSubgraphFinder\\main.exe";

    // Różne wartości n
    vector<int> n_values = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 36, 38, 39, 40, 50, 60, 70, 80, 90, 100};

    ofstream results("results.csv");
    results << "n,avg_time_ms\n";

    for (int n : n_values) {
        string cmd = "benchmark.exe " + to_string(n) + " " + to_string(m) + " " +
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

        results << n << "," << avg << "\n";
        cout << "n=" << n << " avg=" << avg << " ms\n";
    }

    results.close();
    cout << "Wyniki zapisane w results.csv\n";
    return 0;
}
