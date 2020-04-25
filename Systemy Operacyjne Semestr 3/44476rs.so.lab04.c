//SO IS1 210B LAB04
//sebastian ratanczuk
//sebastian-ratanczuk@zut.edu.pl
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

float Global_counter;
pthread_mutex_t jakos = PTHREAD_MUTEX_INITIALIZER;

struct Dane{    
    
    float* tablica;
    int pocz;
    int kon;
};

void *thread(void* data)
{    
    struct Dane *dane=(struct Dane*)data;      
    //printf("%d %d\n",dane->pocz,dane->kon);
    float suma = 0;

    for(int i=dane->pocz;i<=dane->kon;i++) 
        suma=suma+ dane->tablica[i];

    pthread_mutex_lock(&jakos);
        Global_counter=Global_counter+suma;
    pthread_mutex_unlock(&jakos);
    //fprintf(stdout,"Watek nr: %ld suma=%f\n",pthread_self(),suma);    
}

int main(int argc, char **argv)
{
    Global_counter=0;
    if(argc==3)
    {
        int ilosc_liczb = atoi(argv[1]);
        int ilosc_watkow = atoi(argv[2]);

        float tablica[ilosc_liczb];
        srand(0);

        for(int i=0;i<ilosc_liczb;i++)
            tablica[i]=1000.*rand()/RAND_MAX;       

        pthread_t threads[ilosc_watkow];     
        struct Dane dane[ilosc_watkow];

        clock_t t1 = clock();
        for(int i=0;i<ilosc_watkow-1;i++)        
        {
            int ilosc = ilosc_liczb/ilosc_watkow;            
            dane[i].tablica=tablica;
            dane[i].pocz=i*ilosc;
            dane[i].kon=(i+1)*ilosc-1;
            //printf("%d %d %d\n",i,dane[i].pocz,dane[i].kon);
            int threaderr = pthread_create(&threads[i],NULL,thread,&dane[i]);
            if(threaderr!=0)
                fprintf(stderr,"Blad tworzenia watku\n");
        }   

        dane[ilosc_watkow-1].tablica=tablica;
        dane[ilosc_watkow-1].pocz=dane[ilosc_watkow-2].kon+1;
        dane[ilosc_watkow-1].kon=ilosc_liczb-1;
        //printf("%d %d %d\n",ilosc_watkow-1,dane[ilosc_watkow-1].pocz,dane[ilosc_watkow-1].kon);

        int threaderr = pthread_create(&threads[ilosc_watkow-1],NULL,thread,&dane[ilosc_watkow-1]);
        if(threaderr!=0)
                fprintf(stderr,"Blad tworzenia watku\n");        
        
        for(int i=0;i<ilosc_watkow;i++)
        {
            long retval;
            pthread_join(threads[i],(void *)&retval);
        }     
        clock_t t2 = clock();  
        double time = (t2-t1)/(double)CLOCKS_PER_SEC;

        printf("w/t Suma %f Czas %f\n",Global_counter,time);  
        Global_counter=0;
        t1=clock();
        for(int i =0;i<ilosc_liczb;i++)
        {
            Global_counter=Global_counter+tablica[i];
        }
        t2=clock();
        time = (t2-t1)/(double)CLOCKS_PER_SEC;
        printf("wo/t Suma %f Czas %f\n",Global_counter,time);  
        return 0;
    }
    else
    {
        {
            fprintf(stderr,"Zla ilosc argumentow\n");
            return 1;
        }
        return -3;
    }    
}