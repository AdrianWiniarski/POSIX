#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

int pierwsza(int);

typedef struct {
	int begin;
	int end;
	int result;
} prime;

void main(int argc, char* argv[]) { 
prime pr;
int count = 0;
int i, p;
int fifo_save, fifo_save2, fdes, fdes2;

  fifo_save = mkfifo("FIFO", 0666);
  fdes = open("FIFO", O_RDWR);

  fifo_save2 = mkfifo("FIFO2", 0666);
  fdes2 = open("FIFO2", O_RDWR);

  read(fdes, &pr, sizeof(pr));
 while(1){
  if(pr.result != 0){
	close(fdes);
        close(fdes2);
  	exit(1);
  }
  for(i = pr.begin; i<pr.end; ++i){
	p = pierwsza(i);
	if(p){
	  ++count;
	}
  } 
  pr.result = count;
  write(fdes2, &pr, sizeof(pr));
  } 
} 

int pierwsza(int n) 
// Funkcja zwraca 1 gdy n jest liczba pierwsza 0 gdy nie                                   
{ int i,j=0; 
  for(i=2;i*i<=n;i++) { 
    if(n%i == 0) return(0) ; 
 } 
 return(1); 
} 
