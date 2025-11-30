# PZSP2 Zespół 3, Optymalizacja Sieci - Oprogramowanie

Repozytorium kodu online znajduje się na GitLabie wydziałowym - [link](https://gitlab-stud.elka.pw.edu.pl/mwawrzy2/pzsp2-projekt)

## Narzędzia formatowania i analizy statycznej

**Dla kodu w C++:**

- clang-format
- clang-tidy

**Dla kodu w Pythonie:**

- ruff - linter i code formatter

## CI/CD

Będą uruchamianie narzędzia formatowania i analizy statycznej oraz testy jednostkowe.

_TODO: opisać może trochę dokładniej_

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

Wybraliśmy Python ze względu na łatwości tworzenia aplikacji CLI (biblioteka `argparse` oraz biblioteka `rich` do formatowania tekstu w terminalu) oraz na

## Propozycje testów akceptacyjnych

### Testy złożoności obliczeniowej

Użytkownik chce sprawdzić jak rośnie czas obliczeń mierząc go dla 3 różnych topologii sieci - małej, średniej i dużej.

#### Scenariusze pozytywne

1. Uruchomenie programu z wcześniej wygenerowanymi danymi
2. Użytkownik uruchamia program jako `<nazwa-programu> c-comp --load-data <nazwa-pliku-z-danymi>` -> z danych utworzone zostają wykresy 3, które uruchamiają się w interaktywnym oknie matplotlib, dla każdego z badanych algorytmów

---

1. Uruchomienie programu bez gotowych danych -> uruchamiamy algorytmy ewolucyjne
2. Użytkownik uruchamia program jako `<nazwa-programu> c-comp <nazwa pliku z grafem>` -> testy wykonują się w tle, użytkownik informowany jest o postępie prac w formie progress bar -> wyniki testów są zapisane jako `datafile-c-comp{N}` -> z danych utworzone zostają wykresy, które uruchamiają się inter w interkatywnym oknie matplotlib, dla każdego z badanych algorytmów

#### Scenariusz negatywny

1. Uruchomienie programu z gotowymi danymi, ale dane są w niepoprawnym formacie
2. Program informuje o błędnym formacie danych `Invalid data format` kieruje nas do przykładowego pliku `<example-data>` i kończy działanie z kodem -1.

---

1. Uruchomienie programu bez gotowych danym ale dane grafu są w niepoprawnym formacie
2. Program informuje o błędnym formacie danych `Invalid graph format` kieruje nas do przykładowego pliku `<example-graph>` i kończy działanie z kodem -1.

### Krzyw zbieżności badanych metryk

#### Scenariusze pozytywne

1. Uruchomenie programu z wcześniej wygenerowanymi danymi
2. Użytkownik uruchamia program jako `<nazwa-programu> m-conv --load-data <nazwa-pliku-z-danymi>` -> z danych utworzone zostaje 8 wykresów, które uruchamiają się w interkatywnym oknie matplotlib, w zależności od prawdpodobieństwa mutacji oraz liczności populacji dla każdej z 4 metryk

---

1. Uruchomienie programu bez gotowych danych -> uruchamiamy algorytmy ewolucyjne
2. Użytkownik uruchamia program jako `<nazwa-programu> m-conv <nazwa pliku z grafem>` -> testy wykonują się w tle, użytkownik informowany jest o postępie prac w formie progress bar -> wyniki testów są zapisane jako `datafile-m-conv{N}` -> z danych utworzone zostaje 8 wykresów, które uruchamiają się w interkatywnym oknie matplotlib, w zależności od prawdpodobieństwa mutacji oraz liczności populacji dla każdej z 4 metryk

#### Scenariusz negatywny

Analogicznie jak przy Testach złożoności obliczeniowej
