# nowy temat pzsp2

na czym polega proejkt:

implementacja czterech algorytmow optymalizacji wielokryterialnej

one beda w materialach

jeden z czterech nie jest jeszcze znany w literaturze światowej chcemy cos w nim mzienic by sie lepiej zachowywal od tamtych

tez sieci

siec programowalna SDN

routing w sieci

intenty beda czyli zapotrzboewana pomiedzy wezlami

bierzemy pod uwage pojemnosc, throughput w sieci, jaki ruch płynie, suma przeplywow

straty w sieci i opoznienie

na kazdej sciezce jest jakas tam strata i jakies tam opoznienie

straty sa procentowo, opoznienia w ms

badamy jitter, opoznienie tylko z takim błędem

kazdy intent sklada sie z esciezek i w algorytmach ktre bdziemy implementowac ta sciezka to chromosom

algorytm ewolucyjny

optymalizujemy wiele kryteriow, ni e tylko jedno

dla trzech parametrow mamy przestrzen trojwymiarowa tylko wynikiem jest sfera punktow, front stanowi taki zbior punktow ktory dla jednego i drugiego kryterium sa optymalne, bedziey szukac frontu

w naszej przestrzeni decyzyjnej mamy zbior sciezek i ona ma tyle elementow ile krawedzi w tej sciezce, w przestrzeni decyzynej t wartosci tych parametrow

celem jest to zebysmy zaimplementowali czteyr algorytmy i wsrod nich ten ktory jest przez nas rkeomendoway 

szukamy front parettor, ktory jest tworzony przez kilka optymalnych sciezek, jedne z nich beda lepsze dla jednego parametru jedne dla innego

nasz algorytm bedzie bazował na NZGA2, tam bedzie inny proces selekcji

w NZGA2 proces selekcji polega na metodzie ktora nazywa sie crowding distance, w naszym algorytmie bedziemy faworyzyować, nasz algorytm tą selekcje będzie faworyzował ten parametr który wskażemy, ze bedzie faworyzowany, czyli np w video chcemy faworyzowac video, inne parametr tez beda optmalizowane ale bardziej po macoszemu

dostaniemy draft artykułu, który Pan aktualnei pisze, z niego mozna sie dowiedziec jak przebeigaja procesy?

pozostale algorytmy juz zostały opublikowane, i tez dostaniemy literature

na co bedziemy zwracać uwagę, kazdy taki algorytm wielokryterialny, zeby go ocenic on sie charakteryzuje kilkoma metrykami, są różne metryki, nas interesuje metryka generational distance, 

### generationa;l distance

kazda z tych metrk jest pokazana ak ona sie przedstawaia dla jakiegos prostego przykładu dwuwymiarowego, pan by chcial zebysmy te metryki zastosowali dla naszeog problemu

1. effektem nasszej pracy bedzie zeby pokazac te metryki dla wszystkich algorytmow

2. drugim efektem bedzie pokazanie wykresy słupkowe, takie zbey dla kazdego algorytm bylo pokazane jak wygląda.  maja byc wykresy slupkowe ktore beda pokazywac jak te parametr si eprzedstawuaha dka oiszczegikbcg akgirttniw

3. mamy tez przebadac zaeznosc licznosci populacji, od zbieznosci algorytmu, tutaj zbieznosci aalgorytmu beda te metryki IGD itd.

jakas siec z sndb liba, troche wieksza niz polska np, amerykanska, druga topologia sieci to beda sieci full mesh, i to tez bedzie opisane w tym artykule, wynik zbiorczy srednia po wszystich intentach, mamy przebadac jak to sie ma w zaleznosci od licznosci populacji, dla takiej sieci full mesh, nawet 50 wezloej no to tam beda inne wyniki dla populacji ktora ma 10 sciezek dla intentu, a inne dla 100 sciezek dla jednego intentu

jeszcze prawdopodobienstwo mutacji, jesli chodzi o parametry rekombinacji genetycnej to mamy tylko zaimplementowac mutacje, krzyzowaniem sie nie zajmowac, mutacja tutaj wystarczy i mamy zbadac prawdopodobienstwa mutacji, jak wplywaja na zbieznosc algorytmu

reasumujac, metrki, slupki ktore orbazuja jaka jest srednia wartosc parametrow naszych, licznosc populacji i prawdopodobienstwo mutacji
