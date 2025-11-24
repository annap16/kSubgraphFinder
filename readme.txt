W celu skompilowania programu, należy w folderze nadrzędnym (zawierającym ten plik readme) uruchomić komendę:

g++ -std=c++17 -Wall -Iheaders src/main.cpp src/utils.cpp src/graph.cpp src/graphGenerator.cpp src/inputParser.cpp src/graphMappingIterator.cpp src/findDenseSubgraph.cpp src/approximation.cpp src/resultWriter.cpp -o main

Po uruchomieniu powstanie plik wykonywalny o nazwie main.

Uruchamianie:
./main mode input_file_path output_file_path

Jako tryb mozna podac 'a' w celu uruchomienia algorytmu aproksymacyjnego lub 'd' - dla algorytmu dokładnego.

Format pliku wynikowego:
1. W pierwszej linii znajduje się informacja o koszcie - liczbie dodanych krawędzi. Jeśli rozwiązania nie znaleziono - koszt wynosi -1.
2. W kolejnej linii znajduje się rozmiar znalezionego rozszerzenia grafu wejściowego - pewna liczba n.
3. W kolejnych n liniach znajduje się macierz sąsiedztwa znalezionego rozszerzenia grafu wejściowego. 
4. W kolejnej linii znajduje się liczba k.
5. W kolejnych k liniach znajduje się lista mapowań - i-ta liczba w wierszu mówi o tym, na jaki wierzchołek większego grafu wejściowego zmapowany został i-ty wierzchołek grafu-wzorca.

W celu przetestowania algorytmu dokładnego, przygotowaliśmy gotowe instancje testowe, oraz oczekiwane koszty rozszerzenia grafu dla tych przypadków, a także test wczytujący wszystkie dostępne inputy i sprawdzający, czy zwrócone mapowania faktycznie przekładają się na wyliczony koszt. Przykładowe inputy znajdują się w folderze test/exact_algorithm/input_data. Aby skompilować program testujący, należy wykonać komendę:

g++ -std=c++17 -Wall -Iheaders -Itest test/test_utils/testRunner.cpp test/testMain.cpp src/utils.cpp src/graph.cpp src/graphGenerator.cpp src/inputParser.cpp src/graphMappingIterator.cpp src/findDenseSubgraph.cpp src/approximation.cpp src/resultWriter.cpp -o testMain

Żeby uruchomić:

./testMain
