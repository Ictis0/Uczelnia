#include <iostream>
#include <list>

using namespace std;

class Pole {
public:
    int* hetmani;
    int rozmiar;
    int licznik;

    Pole() { licznik = 0; rozmiar = 0; hetmani = NULL; }

    Pole(int n) { 

        rozmiar = n;
        hetmani = new int[n]; 

        for (int i = 0; i < rozmiar; i++)
            hetmani[i] = -1;
    }

    Pole(const Pole &a)
    {
        rozmiar = a.rozmiar;
        licznik = a.licznik;
        hetmani = new int[rozmiar];
        for (int i = 0; i < rozmiar; i++)
        {
            hetmani[i] = a.hetmani[i];
        }
    }

    void show()
    {        
        for (int i = 0; i < rozmiar; i++)        
            cout << "x: " << i << " y:" << hetmani[i] << endl;
        
        cout << endl;
    }

    bool dodajHetmana(int x, int y)
    {       
        if (hetmani[x] == -1)
        {
            for (int i = 0; i < rozmiar; i++)
            {
                if (y == hetmani[i])
                    return false;

                if (hetmani[i] != -1)
                    if (abs(i - x) == abs(hetmani[i] - y))
                        return false;
            }

            hetmani[x] = y;
            licznik++;
            return true;
        }
        return false;              
    }  

    list<int*> BruteForce()
    {
        list<int*> toRet;
        for (int i = 0; i < rozmiar; i++)        
            for (int j = 0; j < rozmiar; j++)
            {
                Pole nowePole(*this);
                if (nowePole.dodajHetmana(i, j))                
                    toRet.push_back(nowePole.hetmani);                
            }        

        return toRet;
    }

    list<int*> Separacja()
    {
        list<int*> toRet;
        int i = 0;
        for (i = 0; i < rozmiar; i++)        
            if (hetmani[i] == -1)
                break;
        
        for (int j = 0; j < rozmiar; j++)
        {
            Pole nowePole(*this);
            if (nowePole.dodajHetmana(i, j))
                toRet.push_back(nowePole.hetmani);
        }

        return toRet;
    }

    list<int*> Inteligentne(int* hetmani)
    {

    }
};

int main()
{
    Pole plansza(4);   
    plansza.dodajHetmana(0, 0);
    list<int*> k = plansza.Separacja();

    for (int* var : k) {
        for (int i = 0; i < 4; i++)
            cout << "x: " << i << " y:" << var[i] << endl;
        cout << endl;
    }
}
