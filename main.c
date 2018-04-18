#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

typedef struct {
	int begin;
	int end;
	int result;
} prime; 

void main(int argc, char* argv[]){ 
  
  if(argc != 5){
  	printf("zle wywolanie!\n");
	printf("/main [pocz_przedzialu] [koniec_przedzialu] [ilosc_procesow] [podzial_liczb_pierwszych]\n");
	exit(5);
  }
  
  prime pr, rr;
  int pid, status, i, total;
  int fifo_file, fifo_save2, fdes, fdes2;
  int st, en;
  st = atoi(argv[1]);
  en = atoi(argv[2]);
  int ct = (int)((en - st)/atoi(argv[3]));

  fifo_file = mkfifo("FIFO", 0666);
  fdes = open("FIFO", O_RDWR);
  
  fifo_file = mkfifo("FIFO2", 0666);
  fdes2 = open("FIFO2", O_RDWR);	

 //zapis do kolejki
	for(i=0; i<atoi(argv[3])*atoi(argv[4]); ++i){
		pr.begin = st+(i*ct);
		if(i+1 == atoi(argv[3])*atoi(argv[4])){
			pr.end = en + 1;
		}else{
			pr.end = st+(i*ct)+ct;
		}
		pr.result = 0;
		write(fdes, &pr, sizeof(pr));
	}
	for(i=0; i<atoi(argv[3]); ++i){
		pr.begin = 0;
		pr.end = 0;
		pr.result = 999;
		write(fdes, &pr, sizeof(pr));
	}
 

  for(i=0; i<atoi(argv[3]); ++i){
   if((pid = fork()) == 0) { /* Proces potomny pot ---*/  
	execl("./pot","pot" ,NULL); 
  }   
 }
total = 0;
 if((pid = fork()) == 0){
	 for(i=0; i<atoi(argv[3]); ++i){
	read(fdes2, &rr, sizeof(prime));
	total += rr.result;
 	}
	printf("liczby pierwsze w przedziale: %d\n", total);
	exit(0);
   }


 for(i=0; i<atoi(argv[3])+1; ++i){
      pid = wait(&status);
      printf("Proces %d zakończony status: %d\n",pid,status/256); 
 } 
 close(fdes);
 close(fdes2);
} 

