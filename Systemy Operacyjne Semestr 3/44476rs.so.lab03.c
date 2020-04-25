// SO IS1 210b LAB03
// Sebastian Ratańczuk
// sebastian-ratanczuk@zut.edu.pl
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(int argc, char** argv)
{
	//printf("%s\n",argv[1]);
	if (argc == 2)
	{
		char* zdanie = argv[1];
		int rozmiar = strlen(zdanie);
		if (rozmiar == 1)
		{
			int asci = *zdanie;
			//printf("%d",asci);          
			return asci;
		}
		else if (rozmiar >= 20)
		{
			fprintf(stderr, "Zla ilosc argumentow\n");
			return 1;
		}
		else
		{
			const int rozmiarLewy = rozmiar / 2;
			const int rozmiarPrawy = rozmiar - rozmiarLewy;
			char* lewy = (char*)malloc(rozmiarLewy+1 * sizeof(*lewy));
			char* prawy = (char*)malloc(rozmiarPrawy+1 * sizeof(*prawy));

			for (int i = 0; i < rozmiarLewy; i++)
				lewy[i] = zdanie[i];
			lewy[rozmiarLewy] = 0;
			int index = 0;
			for (int i = rozmiarLewy; i < rozmiar; i++)
				prawy[index++] = zdanie[i];
			prawy[rozmiarPrawy] = 0;
			//printf("%s\n",lewy);
			//printf("%s\n",prawy);	
			
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));

			PROCESS_INFORMATION pl;
			PROCESS_INFORMATION pp;

			char argsl[100]= "SO.exe ";
			char argsr[100]= "SO.exe ";
			strcat_s(argsl, 100, lewy);
			strcat_s(argsr, 100,prawy);
			//printf("%s\n", argsl);

			int psl = CreateProcessA(NULL, argsl, NULL, NULL, 0, 0, NULL, NULL, &si, &pl);
			if(!psl)	printf("create process failed (%d)\n", GetLastError());
			
			int psr = CreateProcessA(NULL, argsr, NULL, NULL, 0, 0, NULL, NULL, &si, &pp);
			if (!psr)	printf("create process failed (%d)\n", GetLastError());

			WaitForSingleObject(pl.hProcess, INFINITE);
			WaitForSingleObject(pp.hProcess, INFINITE);

			int ppid  = GetCurrentProcessId();

			int exit_lewy=0, exit_prawy=0;

			GetExitCodeProcess(pl.hProcess, &exit_lewy);

			GetExitCodeProcess(pp.hProcess, &exit_prawy);


			printf("%d %d %c\n",ppid,pl.dwProcessId,exit_lewy);
			printf("%d %d %c\n",ppid,pp.dwProcessId,exit_prawy);
			free(lewy);
			free(prawy);	

			if (exit_lewy > exit_prawy)
				return exit_lewy;
			else
				return exit_prawy;
		}
	}
	else
	{
		fprintf(stderr, "Zla ilosc argumentow\n");
		return 1;
	}

	return 0;
}