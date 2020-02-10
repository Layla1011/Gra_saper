#include <iostream>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <string>
#include <fstream>

int poz_x=0, poz_y=0;
int koniec = 0;

using namespace std;

struct pole
{
    int wartosc;
    bool odkryte;
};

void wyswietlReguly()
{
    system("cls");
    fstream plik;
    string linia;
    plik.open("reguly.txt",ios::in);
    while(!plik.eof())
    {
        getline(plik,linia);
        cout << linia << endl;
    }
    cout << endl;
    system("PAUSE");
    plik.close();
}
void wybierzPoziom(int &rozmiar, int &il_bomb)
{
    system("cls");
    int odp=0;
    cout << "Wybierz poziom zaawansowania: " << endl;
    cout << "1- poczatkujacy, 2- sredniozaawansowany, 3- zaawansowany" << endl;
    cin >> odp;
        switch(odp)
        {
        case 1:
            rozmiar=8;
            il_bomb=10;
        break;
        case 2:
            rozmiar=16;
            il_bomb=40;
        break;
        case 3:
            rozmiar=30;
            il_bomb=180;
        break;
        default:
            cout << "Nie ma takiego poziomu!" << endl;
            system("pause");
            wybierzPoziom(rozmiar,il_bomb);
        break;
        }
}

void ustawMine(int rozmiar,pole **plansza,int il_bomb) //* lub & przed rozmiarem i il_bomb
{
    int x,y,r,i;
    r=rozmiar;
    i=il_bomb;
    srand(time(NULL));
    while(i>0)
    {
        x=rand()%r;
        y=rand()%r;
        if(plansza[x][y].wartosc!=9)
        {
            plansza[x][y].wartosc=9;
            i--;
        }
    }
}

void ustawWartosci(int rozmiar,pole **plansza)
{
    for(int i=0; i<rozmiar; i++)
    {
        for(int j=0; j<rozmiar; j++)
        {
            if(plansza[i][j].wartosc==9)
            {
                for (int k = -1; k<2; k++)
                    for (int l = -1; l<2; l++)
                    {
                        if ((i+l)<0 || (j+k)<0 ) continue; //wyjdz bo krawedz
                        if ((i+l)>(rozmiar-1) || (j+k)>(rozmiar-1) ) continue; //wyjdz bo krawedz

                        if (plansza[i+l][j+k].wartosc==9) continue; //wyjdz bo mina
                        plansza[i+l][j+k].wartosc += 1; //zwieksz o 1
                    }
            }
        }
    }
}

void pokazPlansze(int rozmiar,pole **plansza)
{
    //system("cls");
    for(int i=0; i<rozmiar; i++)
    {
        for(int j=0; j<rozmiar; j++)
        {
            if(j==poz_x && i==poz_y) //aktualne miejsce
            {
                   //SetConsoleTextAttribute( hOut, FOREGROUND_RED | FOREGROUND_INTENSITY );
                   cout << plansza[j][i].wartosc << " ";
            }
            else
            {
                if(plansza[j][i].odkryte==false) cout << "#" << " ";
                else if(plansza[j][i].odkryte==true) cout << plansza[j][i].wartosc << " ";
            }
        }
        cout << endl;
    }

}

void odkryjPlansze(int x, int y,int rozmiar, pole **plansza)
{
    if (x<0 || x>rozmiar-1) return; // poza tablic¹ wyjœcie
    if (y<0 || y>rozmiar-1) return; // poza tablic¹ wyjœcie
    if (plansza[x][y].odkryte==true) return;  // ju¿ odkryte wyjœcie

    if (plansza[x][y].wartosc!=9 && plansza[x][y].odkryte==false)
        plansza[x][y].odkryte=true;   // odkryj!

    if (plansza[x][y].wartosc!=0) return; // wartoœæ > 0 wyjscie

    //wywo³anie funkcji dla ka¿dego s¹siada
    odkryjPlansze(x-1,y-1,rozmiar,plansza);
    odkryjPlansze(x-1,y,rozmiar,plansza);
    odkryjPlansze(x-1,y+1,rozmiar,plansza);
    odkryjPlansze(x+1,y-1,rozmiar,plansza);
    odkryjPlansze(x+1,y,rozmiar,plansza);
    odkryjPlansze(x+1,y+1,rozmiar,plansza);
    odkryjPlansze(x,y-1,rozmiar,plansza);
    odkryjPlansze(x,y,rozmiar,plansza);
    odkryjPlansze(x,y+1,rozmiar,plansza);
}

void sterowanie(int rozmiar,pole **plansza)
{
    //zabezpiecz
    cout << "Wpisz numer kolumny (od 0 do "<<rozmiar-1<<"): ";
    cin >> poz_x;
    cout << endl;
    cout << "Wpisz numer wiersza (od 0 do "<<rozmiar-1<<"): ";
    cin >> poz_y;
    cout << endl;
    if(plansza[poz_x][poz_y].wartosc==9) //trafienie na mine
    {
        koniec=2;
    }
    odkryjPlansze(poz_x,poz_y,rozmiar,plansza);
    pokazPlansze(rozmiar,plansza);
    //
}

bool sprawdzCzyWygrana(int rozmiar, pole **plansza, int il_bomb)
{
    int pola_odkryte_wartosciowe=0;
    for (int i=0; i<rozmiar; i++)
    {
        for (int j = 0; j<rozmiar; j++)
        {
            if(plansza[j][i].odkryte==true&&plansza[j][i].wartosc!=9)
            pola_odkryte_wartosciowe++;
        }
    }
    if (((rozmiar*rozmiar)-il_bomb)==pola_odkryte_wartosciowe) return true;
    return false;
}

int main()
{
    int odp=3,rozmiar=1,il_bomb=0;
    cout << "Witaj!" << endl;
    Sleep(2000);
    system("cls");
    while(odp!=0 && odp!=2){
        cout << "Oto gra w sapera. Co chcesz zrobic?" << endl;
        cout << "Wyswietl reguly gry - 1" << endl;
        cout << "Rozpocznij gre - 2" << endl;
        cout << "Wyjdz z programu - 0" << endl;
        cin >> odp;
        switch(odp)
        {
        case 1:
            wyswietlReguly();
        break;
        case 2:
            wybierzPoziom(rozmiar,il_bomb);
        break;
        case 0:
            exit(0);
        break;
        default: cout << "Nie ma takiej opcji" << endl;
        }
        system("cls");
    }
    pole **plansza = new pole *[rozmiar]; //alokacja pamieci
    for ( int i = 0; i < rozmiar; ++i )
     {
         plansza[i] = new pole [rozmiar]; //alokacja pamieci
         for (int j = 0; j < rozmiar; ++j){ //wpisanie wartosci do tablicy
            plansza[i][j].wartosc=0;
            plansza[i][j].odkryte=false;
        }
     }

    ustawMine(rozmiar,plansza,il_bomb);
    ustawWartosci(rozmiar,plansza);
    while(koniec==0)
    {
        sterowanie(rozmiar,plansza);
        if (sprawdzCzyWygrana(rozmiar,plansza,il_bomb)==true) koniec=1;
    }
    if (koniec==1) cout << "\nWygrales! :)" << endl;
    if (koniec==2) cout << "\nKoniec gry, trafiles na mine :(" << endl;
    system("pause");
    //pokazPlansze(rozmiar,plansza);


    //zniszcz plansze
    for ( int i(0); i < rozmiar; ++i )
        delete [] plansza[i]; //uwolnienie pamieci
    delete [] plansza; //uwolnienie pamieci
    plansza = NULL;
    return 0;
}
