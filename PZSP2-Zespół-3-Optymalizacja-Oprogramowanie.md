# PZSP2 Zespół 3, Optymalizacja Sieci - Oprogramowanie

Repozytorium kodu online znajduje się na GitLabie wydziałowym - [link](https://gitlab-stud.elka.pw.edu.pl/mwawrzy2/pzsp2-projekt)

## Narzędzia formatowania i analizy statycznej

**Dla kodu w C++:**

- clang-format
- clang-tidy

**Dla kodu w Pythonie:**

- ruff - linter i code formatter

## CI/CD

CI/CD będzie dzielił się na stopnie:
- Formatter
- Compiler
- Unit tests

#### Formatter
Pipleline sprawdza poprawność formatu kodu

#### Compiler
Pipeline kompiluje kod do uruchomienia testów jednostkowych

#### Unit tests
Pipeline uruchamia testy jednostkowe na skompilowanym kodzie

<b>[ewentualnie]</b> Zostanie dodany stopień uruchamiający testy akceptacyjne.

Tylko i wyłącznie po pomyślnym przejściu przez pipeline zmiany mogą być mergowane z branchem `main`.

## Metodyka tworzenia kodu

main
Główny branch, do którego mergowane są feature branche.

\*-feature
To są branche tworzące konkretny feature. Wiele osób może pobierać kod z tego branchu i mergować kod do tego branch.

\*feature-\*-dev
Branche używane przez konkretną osobę do implementacji/edycji części danego feature'a. Wykorzystywane przed pushowaniem na główny branch tego feature'a.

## Wybór języków programowania

**W C++:**

- implementacja algorytmów ewolucyjnych

Do implementacji algorytmów wybraliśmy C++ ze względu na jego szybkość.

**W Pythonie:**

- implementacja CLI
- wizualizacja wyników działania algorytmów oraz wyników przeprowadzonych eksperymentów

Wybraliśmy Python ze względu na łatwości tworzenia aplikacji CLI (biblioteka `argparse` oraz biblioteka `rich` do formatowania tekstu w terminalu) oraz na bibliotekę `matplotlib` do generowania wykresów.

## Propozycje testów akceptacyjnych

W miejscach gdzie zostały użyte `<>` należy wprowadzić rzeczywiste ściezki/nazwy.

### Testy złożoności obliczeniowej

Użytkownik chce sprawdzić jak rośnie czas obliczeń mierząc go dla 3 różnych topologii sieci - małej (5, 10, 15 węzłów), średniej (20, 25, 30 węzłów) i dużej (35, 40, 45, 50 węzłów).

#### Scenariusze pozytywne

Przypadek, gdy użytkownik posiada plik z wygenerowanymi wcześniej danymi do wykresów w poprawnym formacie.

1. Jeśli pliku z danymi nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/c-comp-data` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> c-comp --load-data <nazwa-pliku-z-danymi>` (`<nazwa-pliku-z-danymi>` oznacza nazwę pliku w folderze `c-comp-data`, nie trzeba dodawać do niej `c-comp-data/`)
5. Po uruchomieniu użytkownik powinien zobaczyć interaktywne okno matplotlib, w którym znajduje się wykres, spełniający poniższe kryteria:
    - liczba węzłów na osi OX
    - czas trwania algorytmów na osi OY
    - dla każdego z algorytmów ewolucyjnych tworzona jest jedna krzywa
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - krzywa dla każdego z algorytmów ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor krzywej odpowiada, któremu algorytmowi
6. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie
---

Przypadek, gdy użytkownik nie posiada pliku z danymi.

1. Użytkownik uruchamia terminal
2. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
3. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> c-comp`
4. Program uruchamia generowanie danych, użytkownik jest informowany o postępie prac w formie progress bar wskazujacego ile z przewidywanych iteracji zostało już wykonanych (gdzie jedna iteracja to uruchomienie jednego algorytmu dla jednej sieci)
5. Po skończeniu generowania danych, pomiary zapisywane są do pliku `datafile-c-comp{N}` do folder `c-comp-data` o czym użytkownik jest informowany w postaci wiadomości tekstowej w terminalu.
6. Następnie użytkownik powinien zobaczyć interaktywne okno matplotlib, w którym znajduje się wykres, spełniający poniższe kryteria:
    - liczba węzłów na osi OX
    - czas trwania algorytmów na osi OY
    - dla każdego z algorytmów ewolucyjnych tworzona jest jedna krzywa
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - krzywa dla każdego z algorytmów ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor krzywej odpowiada, któremu algorytmowi
7. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie

#### Scenariusz negatywny

Przypadek, gdy użytkownik posiada gotowe dane, ale są one w niepoprawnym formacie.

1. Jeśli pliku z danymi nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/c-comp-data` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> c-comp --load-data <nazwa-pliku-z-danymi>` (`<nazwa-pliku-z-danymi>` oznacza nazwę pliku w folderze `c-comp-data`, nie trzeba dodawać do niej `c-comp-data/`).
5. Program wykrywa błędny format danych, wypisuje na terminal wiadomość: `Invalid data format, look at example-datafile-c-comp.csv in examples folder`
6. Program kończy działanie


### Krzyw zbieżności badanych metryk

Użytkownik chce zbadać zbieżność metryki GD, GD+, IGD, IGD+ w zależności od p-ństwa mutacji lub liczności populacji.

W każdym z poniższych scenariuszy będzie miał do wyboru jeden z trzech algorytmów: `INSGA, NSGA2, SPEA2`. Nazw algorytmy należy wpisać w pole `<nazwa-algorytmu>` w argumentach uruchamiania programu

#### Scenariusze pozytywne

##### Wariant A - zależność zbieżności metryk od p-ństwa mutacji

Przypadek, gdy użytkownik posiada plik z wygenerowanymi wcześniej danymi do wykresów w poprawnym formacie.

1. Jeśli pliku z danymi nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/m-conv-data` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> m-conv --load-data <nazwa-pliku-z-danymi>` (`<nazwa-pliku-z-danymi>` oznacza nazwę pliku w folderze `m-conv-data`, nie trzeba dodawać do niej `m-conv-data/`).
5. Po uruchomieniu użytkownik powinien zobaczyć interaktywne okno matplotlib, w którym znajdują się 4 wykresy każdy dla danej metryki w swoim własnym układzie współrzędnych, pojedynczy wykres powinien spełniać poniższe kryteria:
    - liczba iteracji algorytmu na osi OX
    - wartość danej metryki na osi OY
    - dla każdej wartośc p-ństwa mutacji tworzona jest jedna krzywa
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - krzywa dla każdego z p-ństwa mutacji ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor krzywej odpowiada, której wartości p-ństwa
6. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie

##### Wariant B - zależność zbieżności metryk od liczności populacji

Przypadek, gdy użytkownik posiada plik z wygenerowanymi wcześniej danymi do wykresów w poprawnym formacie.

1. Jeśli pliku z danymi nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/p-conv-data` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> p-conv --load-data <nazwa-pliku-z-danymi>` (`<nazwa-pliku-z-danymi>` oznacza nazwę pliku w folderze `p-conv-data`, nie trzeba dodawać do niej `p-conv-data/`).
5. Po uruchomieniu użytkownik powinien zobaczyć interaktywne okno matplotlib, w którym znajdują się 4 wykresy każdy dla danej metryki w swoim własnym układzie współrzędnych, pojedynczy wykres powinien spełniać poniższe kryteria:
    - liczba iteracji algorytmu na osi OX
    - wartość danej metryki na osi OY
    - dla każdej liczności populacji tworzona jest jedna krzywa
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - krzywa dla każdej liczności populacji ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor krzywej odpowiada, której liczności populacji
6. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie

---

1. Uruchomienie programu bez gotowych danych -> uruchamiamy algorytmy ewolucyjne
2. Użytkownik uruchamia program jako `<nazwa-programu> m-conv <nazwa pliku z grafem>` -> testy wykonują się w tle, użytkownik informowany jest o postępie prac w formie progress bar -> wyniki testów są zapisane jako `datafile-m-conv{N}` -> z danych utworzone zostaje 8 wykresów, które uruchamiają się w interkatywnym oknie matplotlib, w zależności od prawdpodobieństwa mutacji oraz liczności populacji dla każdej z 4 metryk


##### Wariant A' - zależność zbieżności metryk od p-ństwa mutacji

Przypadek, gdy użytkownik nie posiada plik z wygenerowanymi wcześniej danymi, ale ma plik z grafem w poprawnym formacie.

1. Jeśli pliku z grafem nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/graphs` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> m-conv --algorithm <nazwa-algorytmu> --load-graph <nazwa-pliku-z-grafem>` (`<nazwa-pliku-z-grafem>` oznacza nazwę pliku w folderze `graphs`, nie trzeba dodawać do niej `graphs/`).
5. Program uruchamia generowanie danych, użytkownik jest informowany o postępie prac w formie progress bar wskazujacego ile z przewidywanych iteracji zostało już wykonanych (gdzie jedna iteracja to uruchomienie algorytmu z daną wartości p-ństwa mutacji)
6. Po skończeniu generowania danych, pomiary zapisywane są do pliku `datafile-m-conv{N}` do folder `m-conv-data` o czym użytkownik jest informowany w postaci wiadomości tekstowej w terminalu.
5. Następnie użytkownik powinien zobaczyć interaktywne okno matplotlib, w którym znajdują się 4 wykresy każdy dla danej metryki w swoim własnym układzie współrzędnych, pojedynczy wykres powinien spełniać poniższe kryteria:
    - liczba iteracji algorytmu na osi OX
    - wartość danej metryki na osi OY
    - dla każdej wartośc p-ństwa mutacji tworzona jest jedna krzywa
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - krzywa dla każdego z p-ństwa mutacji ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor krzywej odpowiada, której wartości p-ństwa
6. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie

##### Wariant B' - zależność zbieżności metryk od liczności populacji

Przypadek, gdy użytkownik nie posiada plik z wygenerowanymi wcześniej danymi, ale ma plik z grafem w poprawnym formacie.

1. Jeśli pliku z grafemnie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/graphs` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> p-conv --algorithm <nazwa-algorytmu> --load-graph <nazwa-pliku-z-grafem>` (`<nazwa-pliku-z-grafem>` oznacza nazwę pliku w folderze `graphs`, nie trzeba dodawać do niej `graphs/`).
5. Program uruchamia generowanie danych, użytkownik jest informowany o postępie prac w formie progress bar wskazujacego ile z przewidywanych iteracji zostało już wykonanych (gdzie jedna iteracja to uruchomienie algorytmu z daną licznością populacji)
6. Po skończeniu generowania danych, pomiary zapisywane są do pliku `datafile-p-conv{N}` do folder `p-conv-data` o czym użytkownik jest informowany w postaci wiadomości tekstowej w terminalu.
5. Następnie użytkownik powinien zobaczyć interaktywne okno matplotlib, w którym znajdują się 4 wykresy każdy dla danej metryki w swoim własnym układzie współrzędnych, pojedynczy wykres powinien spełniać poniższe kryteria:
    - liczba iteracji algorytmu na osi OX
    - wartość danej metryki na osi OY
    - dla każdej liczności populacji tworzona jest jedna krzywa
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - krzywa dla każdej liczności populacji ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor krzywej odpowiada, której liczności populacji
6. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie


#### Scenariusz negatywny

Przypadek, gdy użytkownik posiada gotowe dane, ale są one w niepoprawnym formacie.

1. Jeśli pliku z danymi nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/m-conv-data` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> m-conv --load-data <nazwa-pliku-z-danymi>` (`<nazwa-pliku-z-danymi>` oznacza nazwę pliku w folderze `m-conv-data`, nie trzeba dodawać do niej `m-conv-data/`).
5. Program wykrywa błędny format danych, wypisuje na terminal wiadomość: `Invalid data format, look at example-datafile-m-conv.csv in examples folder`
6. Program kończy działanie

Analogicznie dla zbieżności w zależnosci od liczności populacji.

---

Przypadek, gdy użytkownik nie posiada plik z wygenerowanymi wcześniej danymi, ale ma plik z grafem w poprawnym formacie.

1. Jeśli pliku z danymi nie ma jeszcze w folderze `<sciezka-folderu-z-projektem>/graphs` użytkownik kopiuje go tam.
2. Użytkownik uruchamia terminal
3. Użytkownik przenosi się do folderu z kodem źródłowym projektu `cd <sciezka-folderu-z-projektem>`
4. Użytkownik uruchamia program wpisując do terminala `<nazwa-programu> m-conv --algorithm <nazwa-algorytmu> --load-graph <nazwa-pliku-z-grafem>` (`<nazwa-pliku-z-grafem>` oznacza nazwę pliku w folderze `graphs`, nie trzeba dodawać do niej `graphs/`)
5. Program wykrywa błędny format danych, wypisuje na terminal wiadomość: `Invalid graph format, look at example-graph-file.csv in examples folder`
6. Program kończy działanie

Analogicznie dla zbieżności w zależnosci od liczności populacji.