//SO IS1 210B LAB02
//sebastian ratanczuk
//sebastian-ratanczuk@zut.edu.pl
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) 
{    
    //printf("%s\n",argv[1]);
    if(argc==2)
    {
        char *zdanie = argv[1];
        int rozmiar = strlen(zdanie);
        if(rozmiar == 1)
        {
            int asci = *zdanie;  
            //printf("%d",asci);          
            return asci;
        }
        else if(rozmiar >= 20)
        {
            fprintf(stderr, "Zla ilosc argumentow\n"); 
            return 1;
        }
        else
        {
            const int rozmiarLewy = rozmiar/2;
            const int rozmiarPrawy = rozmiar-rozmiarLewy;
            char *lewy = (char*) malloc(rozmiarLewy * sizeof(*lewy));
            char *prawy = (char*) malloc(rozmiarPrawy * sizeof(*prawy));
            
            for(int i = 0; i<rozmiarLewy; i++)
                lewy[i]=zdanie[i];

            int index = 0;
            for(int i = rozmiarLewy; i<rozmiar; i++)
                prawy[index++]=zdanie[i];           
            //printf("%s\n",lewy);
            //printf("%s\n",prawy);

            int thispid = getpid();

            int dzieciak1 = fork();
            if(dzieciak1==0)    execlp(argv[0],"./lab8", lewy, NULL);

            int dzieciak2 = fork();
            if(dzieciak2==0) execlp("./lab8","./lab8", prawy, NULL);

            int status1, child1 = waitpid(dzieciak1,&status1,0);            
            printf("Pid rodzica = %d, Pid dziecka = %d, Argument = %s, Asci(ret)=%c\n",thispid , child1,lewy,WEXITSTATUS(status1));           
                 
            int status2, child2 = waitpid(dzieciak2,&status2,0);
            printf("Pid rodzica = %d, Pid dziecka = %d, Argument = %s, Asci(ret)=%c\n",thispid , child2,prawy,WEXITSTATUS(status2));

            free(lewy);
            free(prawy);            

            if(WEXITSTATUS(status1) > WEXITSTATUS(status2))
            {
                return WEXITSTATUS(status1);
            }
            else
            {
                return WEXITSTATUS(status2);
            }
        }
    }
    else
    {
        fprintf(stderr, "Zla ilosc argumentow\n");
        return 1;
    }

    return 0;
}
