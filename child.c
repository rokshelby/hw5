#include "myGlobal.h"


int main(int argc, char ** argv)
{
	
	int index = 0;
	int size = 0;
	struct sembuf sbuf;
	sscanf(argv[0], "%d", &index);
	sscanf(argv[1], "%d", &size);
	
	sharedID = GetSharedIDFromFile();
	semID = GetSemIDFromFile();

	sbuf.sem_num = 0;
	sbuf.sem_op = -1;
	sbuf.sem_flg = 0;
	
	semop(semID, &sbuf, 1);
	
	arr = (int*)shmat(sharedID, NULL, 0);
	printf("Child: pid %d clock time %d\n", getpid(), arr[0]);
	shmdt(arr);

	sbuf.sem_num = 0;
	sbuf.sem_op = 1;
	sbuf.sem_flg = 0;
	semop(semID, &sbuf, 1);
	sleep(3);
	return 0;
}
int GetSharedIDFromFile()
{
        FILE * fptr;
        int num = 0;
        fptr = fopen(sharedIDFile, "r");
        fscanf(fptr,"%d", &num);
        fclose(fptr);
        return num;
}
int GetSemIDFromFile()
{
        FILE * fptr;
        int num = 0;
        fptr = fopen(semIDFile, "r");
        fscanf(fptr, "%d", &num);
        fclose(fptr);
        return num;
}

