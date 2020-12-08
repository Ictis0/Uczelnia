#include <iostream>
#include <list>
#include <chrono>
#include <fstream>

using namespace std;

int rozmiar;

int licznik_wygenerowanych = 0;
int licznik_sprawdzonych = 0;

int* inicjalizacja(int n)
{
    rozmiar = n;
    int* hetmani = new int[n];
    for (int i = 0; i < rozmiar; i++)
        hetmani[i] = -1;
    return hetmani;
}

int* kopiujHetmanow(int* hetmani)
{
    int* nowy = new int[rozmiar];
    for (int i = 0; i < rozmiar; i++)
        nowy[i] = hetmani[i];
    return nowy;
}

void pokaz(int* hetmani, bool matrix = false)
{
    for (int i = 0; i < rozmiar; i++)
        cout << "x: " << i << " y: " << hetmani[i] << endl;
    cout << endl;

    if (matrix)
    {
        for (int i = 0; i < rozmiar; i++)
        {
            for (int j = 0; j < rozmiar; j++)
            {
                if (hetmani[i] != j)
                    cout << "0 ";
                else
                    cout << "1 ";
            }
            cout << endl;
        }
    }
}

bool dodajHetmana(int x, int y, int* hetmani)
{
    if (hetmani[x] == -1)//kolumna
    {
        for (int i = 0; i < rozmiar; i++)
        {
            if (hetmani[i] == y)//wiersz
                return false;

            if (hetmani[i] != -1)//skosy
                if (abs(i - x) == abs(hetmani[i] - y))
                    return false;
        }

        hetmani[x] = y;
        return true;
    }
    return false;
}

list<int*> BruteForce(int* hetmani)
{
    list<int*> toRet;
    for (int i = 0; i < rozmiar; i++)
        for (int j = 0; j < rozmiar; j++)
        {
            int* nowiHetmani = kopiujHetmanow(hetmani);
            if (dodajHetmana(i, j, nowiHetmani))
                toRet.push_back(nowiHetmani);
            else
                delete[] nowiHetmani;
        }

    return toRet;
}

list<int*> Separacja(int* hetmani)
{
    list<int*> toRet;
    int i = 0;
    for (i = 0; i < rozmiar; i++)
        if (hetmani[i] == -1)
            break;

    for (int j = 0; j < rozmiar; j++)
    {
        int* nowiHetmani = kopiujHetmanow(hetmani);
        if (dodajHetmana(i, j, nowiHetmani))
            toRet.push_back(nowiHetmani);
        else 
            delete[] nowiHetmani;
    }

    return toRet;
}

int licznosc(int* hetmani)
{
    int suma = 0;
    for (int i = 0; i < rozmiar; i++)
        if (hetmani[i] != -1)
            suma++;
    return suma;
}

ofstream zapis("dane.csv");

void BFS(int rozmiar, bool szybko = true, bool poka = false)// rozmiar problemu | Inteligentne wstawianie czy bruteforce | wyswietlanie macierzy 
{
    licznik_sprawdzonych = 0;
    licznik_wygenerowanych = 0;
    auto start = std::chrono::high_resolution_clock::now();

    list<int*> kolejka;
    kolejka.push_back(inicjalizacja(rozmiar));

    while (!kolejka.empty())
    {
        licznik_sprawdzonych++;

        int* badanyStan = kolejka.front();
        kolejka.pop_front();
        if (licznosc(badanyStan) == rozmiar)
        {
            pokaz(badanyStan, poka);
            break;
        }

        list<int*>noweStany;

        if (szybko)
            noweStany = Separacja(badanyStan);
        else //a ja wolmo
            noweStany = BruteForce(badanyStan);

        delete[] badanyStan;

        for (int* var : noweStany)
        {
            kolejka.push_back(var);
            licznik_wygenerowanych++;
        }        
    }   

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    zapis << rozmiar << "," << licznik_sprawdzonych << "," << licznik_wygenerowanych << "," << duration.count() / 1000000.0 << endl;

    std::cout << "Czas szukania rozwiazania: " << duration.count() / 1000000.0 << " sekund.\n";
    cout << "Ilosc stanow wygenerowanych: " << licznik_wygenerowanych << endl;
    cout << "Ilosc stanow sprawdzonych: " << licznik_sprawdzonych << endl;   

    if (!kolejka.empty())    
        for (int* var : kolejka)        
            delete[] var;
        
    
}

void DFS(int rozmiar, bool szybko = true, bool poka = false)
{
    licznik_sprawdzonych = 0;
    licznik_wygenerowanych = 0;
    auto start = std::chrono::high_resolution_clock::now();

    list<int*> kolejka;
    kolejka.push_back(inicjalizacja(rozmiar));

    while (!kolejka.empty())
    {
        licznik_sprawdzonych++;

        int* badanyStan = kolejka.back();
        kolejka.pop_back();
        if (licznosc(badanyStan) == rozmiar)
        {
            pokaz(badanyStan, poka);
            break;
        }

        list<int*>noweStany;

        if (szybko)
            noweStany = Separacja(badanyStan);
        else //wolmo
            noweStany = BruteForce(badanyStan);

        delete[] badanyStan;

        for (int* var : noweStany)
        {
            kolejka.push_back(var);
            licznik_wygenerowanych++;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);    

    zapis << rozmiar << "," << licznik_sprawdzonych << "," << licznik_wygenerowanych << "," << duration.count() / 1000000.0 << endl;

    std::cout << "Czas szukania rozwiazania: " << duration.count() / 1000000.0 << " sekund.\n";
    cout << "Ilosc stanow wygenerowanych: " << licznik_wygenerowanych << endl;
    cout << "Ilosc stanow sprawdzonych: " << licznik_sprawdzonych << endl;

    if (!kolejka.empty())
        for (int* var : kolejka)
            delete[] var;           
}

int main()
{
    for(int i=4;i<=14;i++)
        DFS(i,false);  
    zapis.close();

}