W celu skompilowania programu, należy w folderze nadrzędnym (zawierającym ten plik readme) uruchomić komendę:

g++ -std=c++17 -Wall -Iheaders src/main.cpp src/utils.cpp src/graph.cpp src/graphGenerator.cpp src/inputParser.cpp src/graphMappingIterator.cpp src/findDenseSubgraph.cpp src/approximation.cpp src/resultWriter.cpp -o main

Po uruchomieniu powstanie plik wykonywalny o nazwie main.

Uruchamianie:
./main mode input_file_path output_file_path

Jako tryb - mode - mozna podac 'a' w celu uruchomienia algorytmu aproksymacyjnego lub 'd' - dla algorytmu dokładnego. Ścieżki do pliku mogą być podane zarówno jako ścieżki względne i bezwzględne.

Format pliku wynikowego:
1. W pierwszej linii znajduje się informacja o koszcie - liczbie dodanych krawędzi. Jeśli rozwiązania nie znaleziono - koszt wynosi -1.
2. W kolejnej linii znajduje się rozmiar znalezionego rozszerzenia grafu wejściowego - pewna liczba n.
3. W kolejnych n liniach znajduje się macierz sąsiedztwa znalezionego rozszerzenia grafu wejściowego. 
4. W kolejnej linii znajduje się liczba k.
5. W kolejnych k liniach znajduje się lista mapowań - i-ta liczba w wierszu mówi o tym, na jaki wierzchołek większego grafu wejściowego zmapowany został i-ty wierzchołek grafu-wzorca.

W celu przetestowania algorytmu przygotowaliśmy gotowe instancje testowe wraz z oczekiwanymi kosztami rozszerzenia grafu. Testy wykonywane są dla dwóch wariantów algorytmu:
Algorytm dokładny
    1. Test wczytuje wszystkie dostępne pliki wejściowe z folderu test/exact_algorithm/input_data.
    2. Test sprawdza, czy zwrócone mapowania wierzchołków dają minimalny koszt rozszerzenia grafu.
    3. Dodatkowo weryfikowane jest, czy wszystkie znalezione podgrafy są izomorficzne ze wzorcem.

Algorytm aproksymacyjny
    1. Test wczytuje wszystkie pliki wejściowe z folderu test/approximation_algorithm/input_data.
    2. Test weryfikuje czy zwrócone podgrafy są izomorficzne ze wzorcem.

Aby skompilować program testujący, należy wykonać następującą komendę:
g++ -std=c++17 -Wall -Iheaders -Itest test/test_utils/testRunner.cpp test/testMain.cpp src/utils.cpp src/graph.cpp src/graphGenerator.cpp src/inputParser.cpp src/graphMappingIterator.cpp src/findDenseSubgraph.cpp src/approximation.cpp src/resultWriter.cpp -o testMain

Żeby uruchomić:

./testMain


W celu przetestowania średniego czasu czasu wykonania programu należy skompilować plik benchmark.cpp:

(w folderze timeTests projektu)
g++ benchmark.cpp -o benchmark.exe 

Żeby uruchomić:
./benchmark n1 n2 k test_count solver_path mode(d/a)

gdzie n1 oraz n2 to liczby wierzchołków grafów, k to liczba szukanych kopii, solver_path to ścieżka bezwzględna do skompilowanego pliku main.exe projektu, a mode to wybór algorytmu (dokładnego lub aproksymacyjnego)



