#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFSIZE 1024
#define PORT_NO 35000


void intSwap (int *pa, int *pb){
    int temp = *pa;
    *pa = *pb;
    *pb = temp;
}



int main()
{
  int serverSocket;      
  int playerSocket1;           
  int playerSocket2;          
  int flags;                
      struct sockaddr_in server;//címinformáció ipcím portszám        
      struct sockaddr_in client;     
      int server_size;                  
      int client_size;  
      int bytes;                   
      int rcvsize;         
      int trnmsize;            
      int err;                    
      char on;                         
      char buffer[BUFSIZE+1];     
      bool player1win;
      bool player2win;
      char playername1[20];
      char playername2[20];
      int elet;
      char kitalalando[18];

      on                     = 1;
      flags                  = 0;
      bytes                  = BUFSIZE;
      server_size            = sizeof server;
      client_size            = sizeof client;
      server.sin_family      = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;
      server.sin_port        = htons(PORT_NO);
      serverSocket = socket(AF_INET, SOCK_STREAM, 0);
      if(serverSocket<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk letrehozni a szerver socketet!\n");
        exit(1);
      }


      setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
      setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);


      err = bind(serverSocket, (struct sockaddr *) &server, server_size);
      if(err<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk bindelni a szerver socketet!\n");
        exit(2);
      }


      err = listen(serverSocket, 2);
      if(err<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk hallgató módba kapcsolni a szervert!\n");
        exit(3);
      }

      printf("A szerver elindult!\n");

      playerSocket1 = accept(serverSocket, (struct sockaddr *) &client, (socklen_t*)&client_size);
      if(playerSocket1<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk elfogadni az 1. jatekos socketjet!\n");
        exit(4);
      }
      else
      {
        sprintf(buffer,"PLAYERNAME\n");
        bytes=strlen(buffer);
        trnmsize = send(playerSocket1, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy küldje el a felhasználó nevét!\n");
          exit(5);
        }
        rcvsize = recv(playerSocket1, playername1, 8, flags);
        if(rcvsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk fogadni az 1. jatekos nevét!\n");
          exit(10);
        }
        playername1[rcvsize-1] = '\0';
        sprintf(buffer, "WAIT\n");
        bytes = strlen(buffer);
          // üzenet küldése az 1. játékosnak
        trnmsize = send(playerSocket1, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy felcsatlakozott -e a másik!\n");
          exit(5);
        }
      }

      playerSocket2 = accept(serverSocket, (struct sockaddr *) &client, (socklen_t*)&client_size);
      if(playerSocket2<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk elfogadni a 2. jatekos socketjet!\n");
        exit(6);
      }
      else
      {
        do{
          sprintf(buffer,"PLAYERNAME\n");
          bytes=strlen(buffer);
          trnmsize = send(playerSocket2, buffer, bytes, flags);
          if(trnmsize<0)
          {
            fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 2. jatekost, hogy küldje el a felhasználó nevét!\n");
            exit(5);
          }
          rcvsize = recv(playerSocket2, playername2, 8, flags);
          if(rcvsize<0)
          {
            fprintf(stderr, "Hiba! Nem tudtuk fogadni az 2. jatekos nevét!\n");
            exit(10);
          }
          playername2[rcvsize-1] = '\0';
        }while(strcmp(playername2,playername1)==0);

      }
      printf("%s\n%s\n",playername1,playername2 );
      printf("A ket jatekos sikeresen felcsatlakozott!\n");

      while(1)
      { 
        elet=10;
        char szo[18];
        int szohossz=0;
        sprintf(buffer, "GAME_START\n");
        bytes = strlen(buffer);
        trnmsize = send(playerSocket1, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost a jatek kezdeterol!\n");
          exit(7);
        }

        trnmsize = send(playerSocket2, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost a jatek kezdeterol!\n");
          exit(8);
        }
        sprintf(buffer,"SZO\n");
        printf("%s\n",buffer );
        trnmsize = send(playerSocket1, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost a jatek kezdeterol!\n");
          exit(7);
        }
        rcvsize = recv(playerSocket1, szo, 18, flags);
        if(rcvsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk fogadni az 2. jatekos nevét!\n");
          exit(10);
        }
        szo[rcvsize-1] = '\0';
        szohossz=strlen(szo);
        for (int i = 0; i <szohossz; ++i)
        {
          kitalalando[i]='_';
        }
        kitalalando[szohossz]='\0';
        printf("%s\n",kitalalando );
        
        char tipp[18];
        while(1)
        {
                sprintf(buffer, "WORD_DOTTED\n");
                bytes = strlen(buffer);
                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy megkapta -e a kitalalandó szó helyettesítőt!\n");
                    exit(18);
                }
              sprintf(buffer, "%s\n",kitalalando);
                bytes = strlen(buffer);

                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                  fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a kitalalandó szó helyettesítőt a 2. jatekosnak!\n");
                  exit(19);
                }
              sprintf(buffer, "ELET\n");
              bytes = strlen(buffer);
              trnmsize = send(playerSocket2, buffer, bytes, flags);
              if(trnmsize<0)
              {
                fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost az életéről!\n");
                exit(8);
              }
              
              sprintf(buffer, "%d\n",elet);
              bytes = strlen(buffer);
              trnmsize = send(playerSocket2, buffer, bytes, flags);
              if(trnmsize<0)
              {
                fprintf(stderr, "Hiba! Nem tudtuk elküldeni a 2. jatekos az életét!\n");
                exit(8);
              }
              sprintf(buffer, "TIPP\n");
              bytes = strlen(buffer);
              trnmsize = send(playerSocket2, buffer, bytes, flags);
              if(trnmsize<0)
              {
                fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost hogy adja meg a tippjét\n");
                exit(8);
              }
              rcvsize = recv(playerSocket2, tipp, 18, flags);
              if(rcvsize<0)
              {
                fprintf(stderr, "Hiba! Nem tudtuk fogadni az 2. jatekos tippjét!\n");
                exit(10);
              }
              tipp[rcvsize-1] = '\0';
              printf("%s\n", tipp);
              if(strlen(tipp)==1)
              {
              int count=0;
              for (int i = 0; i < strlen(szo); ++i)
                {

                  if(szo[i]==tipp[0])
                  { 
                    kitalalando[i]=szo[i];
                    count++;
                  }
                  
                }
                if(count==0)
                  {elet--;}
                if(strcmp(kitalalando,szo)==0)
                {
                  player2win=true;
                  break;
                }

              }
              else if(strcmp(kitalalando,szo)==0)
                {
                  player2win=true;
                  break;
                }
                else if(strcmp(tipp,szo)==0)
                {
                  player2win=true;
                  break;
                }
                else if(strcmp(tipp,szo)!=0)
                {
                  elet--;
                }
                if(strcmp(tipp,"/feladom")==0)
                {elet=0;
                  break;
                }
                
              if(elet==0)
                {break;}


        }
        char felad1[5];
        char felad2[5];

        sprintf(buffer, "PLAY_AGAIN\n");
        bytes = strlen(buffer);

        trnmsize = send(playerSocket1, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk megkerdezni az 1. jatekost, hogy akar-e visszavagot!\n");
          exit(21);
        }

        rcvsize = recv(playerSocket1, felad1, 5, flags);

        if(rcvsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk fogadni, hogy az 1. jatekos akar-e visszavagot!\n");
          exit(22);
        }
        felad1[rcvsize-1] = '\0';


        trnmsize = send(playerSocket2, buffer, bytes, flags);
        if(trnmsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk megkerdezni a 2. jatekost, hogy akar-e visszavagot!\n");
          exit(23);
        }

        rcvsize = recv(playerSocket2, felad2, 5, flags);

        if(rcvsize<0)
        {
          fprintf(stderr, "Hiba! Nem tudtuk fogadni, hogy a 2. jatekos akar-e visszavagot!\n");
          exit(24);
        }
        felad2[rcvsize-1] = '\0';

        if((strcmp(felad1, "igen")==0 && strcmp(felad2, "igen")==0))
        {
          sprintf(buffer, "SESSION\n");
      bytes = strlen(buffer);

      trnmsize = send(playerSocket1, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk megmondani az 1. jatekosnak, hogy vege a jateknak!\n");
        exit(25);
      }

      trnmsize = send(playerSocket2, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk megmondani a 2. jatekosnak, hogy vege a jateknak!\n");
        exit(26);
      }

      sprintf(buffer, " :: KOREREDMENY ::\n");
      if(elet==0)
      {
        strcat(buffer,"Vesztettél\n");
        strcat(buffer,playername2);
        strcat(buffer,"\n");
      }
      else if(player2win==true)
      {
        strcat(buffer, " :: A KOVETKEZO JATEKOS NYERT! ::\n");
        strcat(buffer,playername2);
        strcat(buffer,"\n");
      }
      bytes = strlen(buffer);

      trnmsize = send(playerSocket1, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a vegeredmenyt az 1. jatekosnak!\n");
        exit(27);
      }

      trnmsize = send(playerSocket2, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a vegeredmenyt a 2. jatekosnak!\n");
        exit(28);
      }
          intSwap(&playerSocket1,&playerSocket2);
        }
        else
        {
          break;
        }

      }
      sprintf(buffer, "GAME_OVER\n");
      bytes = strlen(buffer);

      trnmsize = send(playerSocket1, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk megmondani az 1. jatekosnak, hogy vege a jateknak!\n");
        exit(25);
      }

      trnmsize = send(playerSocket2, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk megmondani a 2. jatekosnak, hogy vege a jateknak!\n");
        exit(26);
      }

      sprintf(buffer, " :: VEGEREDMENY ::\n");
      if(elet==0)
      {
        strcat(buffer,"Vesztettél\n");
        strcat(buffer,playername2);
        strcat(buffer,"\n");
      }
      else if(player2win==true)
      {
        strcat(buffer, " :: A KOVETKEZO JATEKOS NYERT! ::\n");
        strcat(buffer,playername2);
        strcat(buffer,"\n");
      }
      bytes = strlen(buffer);

      trnmsize = send(playerSocket1, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a vegeredmenyt az 1. jatekosnak!\n");
        exit(27);
      }

      trnmsize = send(playerSocket2, buffer, bytes, flags);
      if(trnmsize<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a vegeredmenyt a 2. jatekosnak!\n");
        exit(28);
      }

      //bezárjuk a socketeket
      close(playerSocket2);
      close(playerSocket1);
      close(serverSocket);
    }