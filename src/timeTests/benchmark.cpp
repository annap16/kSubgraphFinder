#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <algorithm>

using namespace std;

vector<vector<int>> generate_matrix(int n, bool multigraph=true, int max_multiedge=10, bool undirected=false) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_multi(0, max_multiedge);
    uniform_int_distribution<> dist_simple(0, 1);

    vector<vector<int>> M(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            int val = multigraph ? dist_multi(gen) : dist_simple(gen);
            M[i][j] = val;
            if (undirected) M[j][i] = val;
        }
    }
    return M;
}

void write_graph_file(const string &filename, int n1, int n2, bool multigraph, int max_multiedge, bool undirected, int k) {
    ofstream f(filename);
    if (!f.is_open()) { cerr << "Nie mogę zapisać pliku: " << filename << endl; return; }

    auto g1 = generate_matrix(n1, multigraph, max_multiedge, undirected);
    f << n1 << "\n";
    for (auto &row : g1) {
        for (int i = 0; i < row.size(); i++)
            f << row[i] << (i+1 < row.size() ? ' ' : '\n');
    }

    auto g2 = generate_matrix(n2, multigraph, max_multiedge, undirected);
    f << n2 << "\n";
    for (auto &row : g2) {
        for (int i = 0; i < row.size(); i++)
            f << row[i] << (i+1 < row.size() ? ' ' : '\n');
    }
    f << k << "\n";
}

int main(int argc, char** argv) {
    if (argc < 6) {
        cerr << "Użycie:\n" << argv[0] << " n1 n2 k liczba_testów ścieżka_do_solvera\n";
        return 1;
    }

    int n1 = stoi(argv[1]), n2 = stoi(argv[2]), k = stoi(argv[3]), tests = stoi(argv[4]);
    string solverPath = argv[5];

    filesystem::path solverFs(solverPath);
    if (!filesystem::exists(solverFs)) {
        cerr << "Błąd: nie mogę znaleźć solvera pod ścieżką: " << solverFs << endl;
        return 1;
    }

#ifdef _WIN32
    if (solverFs.extension() != ".exe") solverFs += ".exe";
#endif

    string solverAbs = filesystem::absolute(solverFs).string();

    filesystem::create_directory("inputs");
    filesystem::create_directory("outputs");

    double total_ms = 0.0;

    for (int i = 0; i < tests; i++) {
        string inFile = "inputs/test_" + to_string(i) + ".txt";
        string outFile = "outputs/out_" + to_string(i) + ".txt";

        write_graph_file(inFile, n1, n2, true, 5, false, k);

        auto start = chrono::high_resolution_clock::now();

        string cmd = solverAbs + " d " + inFile + " " + outFile;
        cout << "Running command: " << cmd << endl; 

        int ret = system(cmd.c_str());

        auto end = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(end - start).count();

        if (ret != 0)
            cerr << "Błąd przy uruchamianiu solvera na " << inFile << endl;

        cout << "Test " << i << " = " << ms << " ms\n";
        total_ms += ms;
    }

    cout << "\nŚREDNI CZAS: " << (total_ms / tests) << " ms\n";
    return 0;
}
