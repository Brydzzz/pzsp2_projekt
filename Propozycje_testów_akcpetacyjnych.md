# Propozycje testów akcpetacyjnych

omówione z właścicielem tematu

## Testy złożoności obliczeniowej

Sprawdzenie jak rośnie czas obliczeń (liniowo czy wykładniczo) mierząc czas dla 3 różnych topologii sieci - małej, średniej i dużej

W naszym projekcie mieliśmy się ograniczyć do sieci o 50 węzłach, więc proponowane przez nas rozmiary sieci to sztucznie wygenerowane sieci o 10, 30 i 50 węzłach.

Dla lepiej wyglądającego wykresu moglibyśmy rozszerzyć eksperyment o zaczynanie od sieci o rozmiarze 5 i zwiększaniu rozmiaru o 1 aż do 50.

Testy wykonamy za pomocą Pythona i biblioteki matplotlib.

Spodziewamy się, że czas obliczeń będzie rósł wykładniczo

## Krzywe zbieżności badanych metryk

Mierzone przez nas metryki to GD, IGD, GD+, IGD+.

Mamy zmierzyć ich zbieżność w funkcji liczności populacji i prawdopodobieństwa mutacji.

Tutaj podobnie będziemy wykonywać testy w Pythonie.

Spodziewamy się, że metryki GD+ i IGD+ jako ulepszone wersje oryginalnych metryk będą dawały lepsze wyniki.
