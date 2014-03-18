Witam!
jak widzicie Panowie w pliku 'planowanie_ruchu_robotow_labirynty'
sa pliki:
1. algorithm.cpp
2. scene

w pliku 'scene' znajduja sie przykladowe labirynty w formacie txt
ktorych struktura przedstawia sie nastepujaco:
[wiersze][spacja][columny] - ktore okreslaja rozmiar planszy/sceny
scena jest reprezentowana za pomoca zer i jedynek, gdzie
* zera - pola po ktorych mozna sie poruszac
* jedynki - pola zabronione/sciany po ktorych nie mozna sie poruszac

algorithm.cpp zaimplementowane sa algorytmy DFS i BFS
przy kompilacji mozna skorzytac z dyrektyw
CONSOLE - wypisuje logi algorytmu w konsoli
DEBUGCONSOLE - nie wazne :P
FILE - zapisuje wynik dzialania do pliku data_out.txt

wynikiem dzialania sa wspolrzedne na planszy wierzcholkow grafu
wspolrzedne w formie [wiersz][kolumna] analogicznie do zapisu
macierzowego.
- kazdy krok algorytmu zwraca i zapisuje dwie linijki do pliku
  -w pierwszej z zapisanych linijek sa wspolrzedne punku,
  w ktorym obecnie jestesmy (przetwarzany wierzcholek)
  -w drugiej zapisywanej linijce sa wspolrzedne wierzcholkow,
  ktore znajduja sie na liscie/stosie
  
PRZYKLADOWO dla alg DFS:
1 1 - jestemsy w punkcie (1,1)
2 1 1 2 - na liscie mamy wspolrzedne sasiadow (2,1) i (1,2)
2 1 - kolejnym przetwarzanym wierzcholkiem jest ten o wspolrzednych (2,1)
1 2 3 1 2 2 - a to jego lista wspolrzednych nieodiwedzony sasiadow
1 2 - pierwszy z listy...
3 1 2 2 2 2 1 3 -itd

mysle ze taki plik do wizualizacji jest o tyle spoko ze nie bedzie trzeba
wchodzic w kod.
co do kodu to w mainie znajduja sie parametry ktorymi moze krecic uzytkownik:
- sciezka do pliku wejsciowego
- okreslenie ilosc kierunkow poruszania (4 badz 8)
- okreslenie algorytmu przeszukiwania (dfs bad bfs)
- okreslenie punktu startu 
- okreslenie punktu mety

thats it, hope u like it:)


