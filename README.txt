Echipa: MIDorFEED.exe
Membrii: Boldisor Dragos-Alexandru (322CB), Draghici Vlad Matei (322CB).
Etapa 2

Instructiuni de compilare:
make (g++ -std=c++17 -Wall -Wextra -g)

Proiectul este structurat in 3 parti:

- partea de "facade" care va interactiona direct cu programul xboard si va
apela functii cu ajutorul carora va returna miscari valide.

- partea de "board" care retine reprezentarea interna a tablei de sah si o va
utiliza cu scopul de a verifica pozitiile de sah, de a gasi miscari valide si
de a face modificari pe tabla.

- partea de "game" care contine "logica" bot-ului. El preia miscarile valide si
pe baza unor algoritmi alege o miscare si o returneaza mai departe facade-ului.
(in a doua etapa botul va verifica daca poate sa faca rocada si o va executa,
iar daca nu, va alege o miscare random din cele valide).

I. Facade
    In main() este creata o instanta a clasei Facade, apoi se apeleaza metoda
start(). Aceasta face toate setarile initiale necesare xboard-ului si apeleaza
alte metode care vor primi input-ul (comenzile). Centrale sunt cele 2 metode
newGame() si pauseGame(). Prima se apeleaza la fiecare comanda "new" si, in
functie de comanda primita, va executa o serie de instructiuni (schimba
culoarea, face o mutare apeland metode din Game, face update-uri de board etc).
Metoda pauseGame() este apelata cand programul primeste "force" si are un
comportament asemanator (exceptia este ca NU va da la output miscari).

II. Game
    Clasa Game are momentan urmatoarele functii:
        - getNextMove: functia intoarce string-ul cu miscarea pe care bot-ul
            vrea sa o faca. Aceasta se foloseste de functia alphabeta_negamax().
        - alphabeta_negamax: functie care aplica algoritmul negamax cu alphabeta
            pruning. Daca depth-ul este 0, este chemata functia de evaluate din
            board. Altfel, functia este apelata recursiv; mai intai se retine
            board-ul initial, apoi se face o miscare, se apeleaza functia din
            care se extrage scorul obtinut, se reface tabla la forma initiala
            si se calculeaza maximul scorului. 
        - updateBoardForEnemy: functia primeste ca input un string reprezentand
            o miscare si culoarea care face miscarea si cheama corespunzator
            metoda MoveOnBoard din Board pentru a schimba tabla de sah.
        - FromMoveToStr: functia care face trecerea dintr-o miscare intr-un
            format string necesar xboard-ului.
	Clasa Game a fost micsorata (o parte din functionalitati au fost mutate in
	Board) deoarece aici vor fi doar functii care comunica direct cu Facade sau
	cu Board, precum si logica algoritmului Minimax ce va fi implementat.

III. Board
    1. Enum-ul PieceType: contine codificarea pieselor (piesele negre au
        valoare negativa iar cele albe, pozitiva).
    2. Struct-ul Piece: reprezinta o piesa. Contine 2 field-uri: un int care
        reprezinta numarul de miscari executate pana acum si un PieceType (ce
        tip de piesa este).
    3.1. Clasa Board are urmatoarele campuri:
        - board (Piece **board): o matrice de 8*8 Piece-uri care reprezinta
            tabla de sah.
        - whitePieces (map<pair<char, char>, Piece>): un map cu cheile
            pair<char, char> care reprezinta o pozitie in tabla de sah si
            valori de tip Piece ce reprezinta piesa aflata pe acea pozitie.
            Prin urmare, acest map asociaza fiecarei pozitii(care are o piesa)
            din tabla de sah piesa aflata pe acea pozitie. Acest map este
            pentru piesele albe.
        - blackPieces (map<pair<char, char>, Piece>): Un map asemanator cu cel
            de whitePieces, dar pentru piesele negre.
		- whiteLastMove (pair<pair<char, char>, pair<char, char>>): Ultima
			miscare a albului. Aceasta este folosita pentru a verifica
			conditiile de functionare pentru En Passant.
		- blackLastMove (pair<pair<char, char>, pair<char, char>>): Asemanator
			lui whiteLastMove.
            
    3.2. Clasa Board are urmatoarele functii:
        - InitializeBoard: initializeaza piesele in pozitia de start.
        - PrintBoard: afisaza starea actuala a tablei de sah (board,
            whitePieces, blackPieces) (pentru debugging).
		- PrintLegalMoves: afiseaza toate miscarile legale gasite pentru
            configuratia tablei din acel moment.
        - VerifyPosition: verifica daca o anumita pozitie este valida
            (se afla pe tabla).
        - MakeMoveOnBoard: o parte din codul care se afla initial in board.
            Aceasta functie actualizeaza tabla, map-urile si face alte
            modificari necesare (ex: promoveaza pioni) (se fac modificari
            pentru miscari normale, rocada si en passant).
        - verifyIfCheck: functie centrala a testarii pozitiei de sah. Aceasta
            functie primeste 3 parametrii si are 2 posibilitati de functionare:
            verificare initiala (sau pentru o miscare a regelui sau pentru
            miscari de tip En Passant): acest tip de functionare verifica
            toate posibilele atacuri din partea inamicului cu ajutorul
            functiilor verifyForX. Daca se gaseste un atac, acesta este
            introdus in map-ul result si returnat la final.
            A doua modilitate de functionare este cea folosita pentru a
            verifica daca o miscare pseudo-valida este valida d.p.d.v. al
            sahului: pentru pozitia de plecare trebuie verificat daca conduce
            la descoperirea regelui (un posibil sah), iar pentru pozitia finala
            trebuie verificat daca apara regele de pisele care il ataca
            (primite ca parametru) (ne folosim de functiile verifyBlockRook si
            verifyBlockBishop).
        - verifyForX (Pawn, Knight, King, RBQ - adica Rook, Bishop, Queen):
            Aceaste functii sunt chemate de verifyIfCheck si cauta pornind de
            la pozitia regelui daca cumva acesta este atacat.
        - verifyBlockRook: verifica daca piesa nou mutata apara regele de un
            atac venit pe linie sau coloana (atac de tip Tura). Folosit pentru
            tura si regina inamica.
        - verifyBlockBishop: asemanator verifyBlockRook doar ca pentru atacuri
            pe diagonala (regina sau nebun).
            
        - GetAllLegalMoves: functia primeste culoarea care este la mutare si
            genereaza toata mutarile legale ale acelei culori. Functia parcurge
            toate piesele ramase in viata pentru culoarea data si apeleaza pentru
            fiecare piesa functia care genereaza toate mutarile legale ale acelui
            tip de piesa.
        - GetLegal_Moves (Pawn, Knight, Bishop, Rook, Queen, King): o clasa de
            functii care genereaza toate miscarile valide ale unui tip de piesa.
            Functiile primesc o piesa ca parametru si returneaza un vector cu
            mutarile valide. Fiecare functie respecta urmatorul tipar:
                - se genereaza o mutare conforma cu tipul de piesa
                - se testeaza daca este legala (nu se intra in sah, se rezolva
                    sahul daca este cazul)
                - daca mutare este legala se adauga in vectorul de mutari

        - evaluate care evalueaza euristic forma actuala a tablei (valoarea
            pieselor, formatiunile de pioni, ocuparea spatilor mai importante
            (mijlocul tablei cu o prioritate mai mare decat marginile), sahul)
        

Responsabilitati:

Etapa 1:
    Facade si o parte din Game (schimbarea tablei pentru miscari ale
    adversarului) -> Boldisor Dragos Alexandru.
    Board si Game -> Draghici Vlad Matei.

Etapa 2:
    Partea de verificare a sahului (pentru cazul initial/miscarea regelui si
    pentru cazul celorlalte miscari) -> Boldisor Dragos Alexandru.
    Partea de schimbare a pionului in oricare din: tura, nebun, regina,
    cal -> Boldisor Dragos Alexandru.
    Miscarile pseudo-valide pentru toate piesele -> Draghici Vlad Matei.
    Rocada, en passant -> Draghici Vlad Matei.

Etapa 3:
    Am decis sa lucram impreuna, pe rand. Ideile au fost discutate in echipa si
    aplicate asemenea.

Surse de inspiratie:
1. https://www.gnu.org/software/xboard/engine-intf.html