//SO IS1 210B LAB06
//sebastian ratanczuk
//sebastian-ratanczuk@zut.edu.pl
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include <time.h>

float Global_counter;
HANDLE mutex;

struct Dane {

    float* tablica;
    int pocz;
    int kon;
};

DWORD WINAPI thread(LPVOID data)
{
    struct Dane* dane = (struct Dane*)data;
    //printf("%d %d\n",dane->pocz,dane->kon);
    float suma = 0;
    int ilosc = dane->kon - dane->pocz +1;
    for (int i = dane->pocz; i <= dane->kon; i++)
        suma = suma + dane->tablica[i];

    WaitForSingleObject(mutex, INFINITE);
    Global_counter = Global_counter + suma;
    ReleaseMutex(mutex);
    printf("Watek nr: %ld suma=%f, ilosc= %d\n", GetCurrentThreadId(),suma,ilosc);
    //printf("test\n");
    return 0;
}

int main(int argc, char** argv)
{
    Global_counter = 0;    
    if (argc == 3)
    {

        mutex = CreateMutex(NULL, FALSE, NULL);
        int ilosc_liczb = atoi(argv[1]);
        int ilosc_watkow = atoi(argv[2]);


        //char* lewy = (char*)malloc(rozmiarLewy + 1 * sizeof(*lewy));

        float* tablica = (float*)malloc(ilosc_liczb*sizeof(*tablica));
        srand(0);

        for (int i = 0; i < ilosc_liczb; i++)
        {
            tablica[i] = 1000. * rand() / RAND_MAX;
           // printf("tablica float\n");

        }


        HANDLE *threads=(HANDLE*)malloc(ilosc_watkow*sizeof(*threads));

        DWORD *thrdids = (DWORD*)malloc(ilosc_watkow*sizeof(*thrdids));

        struct Dane *dane=(struct Dane*)malloc(ilosc_watkow*sizeof(*dane));


        clock_t t1 = clock();
        for (int i = 0; i < ilosc_watkow - 1; i++)
        {
            int ilosc = ilosc_liczb / ilosc_watkow;
            dane[i].tablica = tablica;
            dane[i].pocz = i * ilosc;
            dane[i].kon = (i + 1) * ilosc - 1;
            //printf("%d %d %d\n",i,dane[i].pocz,dane[i].kon);
            threads[i] = CreateThread(NULL, 0, thread, &dane[i],0, &thrdids[i]);
            if (threads[i] == NULL)
            {
                printf("ERROR");
            }
        }
       

        dane[ilosc_watkow - 1].tablica = tablica;
        dane[ilosc_watkow - 1].pocz = dane[ilosc_watkow - 2].kon + 1;
        dane[ilosc_watkow - 1].kon = ilosc_liczb - 1;
        //printf("%d %d %d\n",ilosc_watkow-1,dane[ilosc_watkow-1].pocz,dane[ilosc_watkow-1].kon);
        threads[ilosc_watkow - 1] = CreateThread(NULL, 0, thread, &dane[ilosc_watkow - 1],0, &thrdids[ilosc_watkow - 1]);

       // int threaderr = pthread_create(&threads[ilosc_watkow - 1], NULL, thread, &dane[ilosc_watkow - 1]);
       // if (threaderr != 0)
           
        for (int i = 0; i < ilosc_watkow; i++)
        {
            long retval;
            WaitForSingleObject(threads[i], INFINITE);
            CloseHandle(threads[i]);
        }

        clock_t t2 = clock();
        double time = (t2 - t1) / (double)CLOCKS_PER_SEC;

        printf("w/t Suma %f Czas %f\n", Global_counter, time);
        Global_counter = 0;
        t1 = clock();
        for (int i = 0; i < ilosc_liczb; i++)
        {
            Global_counter = Global_counter + tablica[i];
        }
        t2 = clock();
        time = (t2 - t1) / (double)CLOCKS_PER_SEC;
        printf("wo/t Suma %f Czas %f\n", Global_counter, time);
        CloseHandle(mutex);
        return 0;
    }
    else
    {
        {
            fprintf(stderr, "Zla ilosc argumentow\n");
            return 1;
        }
        return -3;
    }
}