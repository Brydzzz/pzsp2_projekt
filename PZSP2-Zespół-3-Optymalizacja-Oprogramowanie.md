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
    - dla każdego z algorytmów ewolucyjnych tworzony jest jeden wykres
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - plot dla każdego z algorytmów ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor wykresu odpowiada, któremu algorytmowi
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
    - dla każdego z algorytmów ewolucyjnych tworzony jest jeden wykres
      - punkty pomiarowe zaznaczone są kropkami
      - kolejne punkty połączone są prostymi
    - plot dla każdego z algorytmów ma inny kolor
    - na wykresie znajduje się legenda wskazująca, na to który kolor wykresu odpowiada, któremu algorytmowi
7. Po zamknięciu interaktywnego okna matplotlib, program kończy swoje działanie

#### Scenariusz negatywny

1. Uruchomienie programu z gotowymi danymi, ale dane są w niepoprawnym formacie
2. Program informuje o błędnym formacie danych `Invalid data format` kieruje nas do przykładowego pliku `<example-data>` i kończy działanie z kodem -1.

---

1. Uruchomienie programu bez gotowych danych ale dane grafu są w niepoprawnym formacie
2. Program informuje o błędnym formacie danych `Invalid graph format` kieruje nas do przykładowego pliku `<example-graph>` i kończy działanie z kodem -1.

### Krzyw zbieżności badanych metryk

#### Scenariusze pozytywne

1. Uruchomenie programu z wcześniej wygenerowanymi danymi
2. Użytkownik uruchamia program jako `<nazwa-programu> m-conv --load-data <nazwa-pliku-z-danymi>` -> z danych utworzone zostaje 8 wykresów, które uruchamiają się w interkatywnym oknie matplotlib, w zależności od prawdpodobieństwa mutacji oraz liczności populacji dla każdej z 4 metryk

---

1. Uruchomienie programu bez gotowych danych -> uruchamiamy algorytmy ewolucyjne
2. Użytkownik uruchamia program jako `<nazwa-programu> m-conv <nazwa pliku z grafem>` -> testy wykonują się w tle, użytkownik informowany jest o postępie prac w formie progress bar -> wyniki testów są zapisane jako `datafile-m-conv{N}` -> z danych utworzone zostaje 8 wykresów, które uruchamiają się w interkatywnym oknie matplotlib, w zależności od prawdpodobieństwa mutacji oraz liczności populacji dla każdej z 4 metryk

#### Scenariusz negatywny

Analogicznie jak przy testach złożoności obliczeniowej
