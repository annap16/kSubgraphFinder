#include <iostream>
#include <chrono>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

long long measure_exec(const std::string& command) {
    using namespace std::chrono;

    auto t1 = high_resolution_clock::now();
    int ret = system(command.c_str());
    auto t2 = high_resolution_clock::now();

    if (ret != 0)
        std::cerr << "[ERROR] Program returned non-zero exit code\n";

    return duration_cast<milliseconds>(t2 - t1).count();
}

void run_for_directory(const std::string& program,
                       const std::string& inputDir,
                       const std::string& outputDir,
                       int file_num)
{
    std::cout << "===== DIRECTORY: " << inputDir << " =====\n";

    fs::create_directories(outputDir);

    /*for (int i = 0; i < file_num; i++) {
        std::string inFile = inputDir + "/input" + std::to_string(i) + ".txt";
        std::string outFile = outputDir + "/output" + std::to_string(i) + ".txt";

        if (!fs::exists(inFile)) {
            std::cout << "[SKIP] " << inFile << " (not found)\n";
            continue;
        }

        std::string cmd = program + " " + inFile + " " + outFile;

        long long ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";
    }*/
   
        std::string inFile = inputDir + "/input9" + ".txt";
        std::string outFile = outputDir + "/output9" + ".txt";
        std::string cmd = program + " " + inFile + " " + outFile;
        
        long long ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";

        inFile = inputDir + "/input8" + ".txt";
         outFile = outputDir + "/output8" + ".txt";
        cmd = program + " " + inFile + " " + outFile;
        
        ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";

        inFile = inputDir + "/input5" + ".txt";
         outFile = outputDir + "/output5" + ".txt";
         cmd = program + " " + inFile + " " + outFile;
        
         ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";

        inFile = inputDir + "/input4" + ".txt";
         outFile = outputDir + "/output4" + ".txt";
         cmd = program + " " + inFile + " " + outFile;
        
        ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";

         inFile = inputDir + "/input3" + ".txt";
         outFile = outputDir + "/output3" + ".txt";
         cmd = program + " " + inFile + " " + outFile;
        
        ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";

        inFile = inputDir + "/input2" + ".txt";
         outFile = outputDir + "/output2" + ".txt";
         cmd = program + " " + inFile + " " + outFile;
        
        ms = measure_exec(cmd);
        std::cout << "[OK] " << inFile << " → " << ms << " ms\n";
}

int main() {
#ifdef _WIN32
    std::string program = "main.exe"; 
#else
    std::string program = "./main";
#endif

    //run_for_directory(program, "input/graphs",      "outputs/graphs", 5);
    run_for_directory(program, "input/multigraphs", "outputs/multigraphs", 10);

    return 0;
}
