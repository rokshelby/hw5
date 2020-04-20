#ifndef MYGLOBAL_H
#define MYGLOBAL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>	
#include <limits.h>
#include <pwd.h>
	
	
	
	int GetInputPlaceInSharedMem(int);

	int GetSharedIDFromFile();
	int GetSemIDFromFile();
	
	char * GetSharedKey();
	int GetSharedInt();
	char * GetSharedInputFile();

	
	int GetInputPlaceInSharedMem(int);

	void StartProgram();

	int GetBinZero();
	void SetSharedID(int);
	void SetSemID(int);

	void UpdateTime();	
	void CatchSignal(int);
	void SetTime();
	sem_t * mutex;
	int sharedID;
	int semID;
	int * arr;
	int CheckTime();
	int RandomResourceNumber();
	void RandomTime();


	//#define NOTIMETEST
	const char inputFile[] = "./InputNumberFile.txt";
	const char sharedKey[] = "./master.c";
	const char outputFile[] = "./adder_log";
	const char semaphoreName[] = "semSignal2";
	const char sharedIDFile[] = "./sharedID.txt";
	const char semIDFile[] = "./semID.txt";
	const int sharedInt = 63;
#endif	
