#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFSIZE 1024
#define PORT_NO 60000


void kirajzol(char palya[5][5])
{
      printf("     |     |     |     |     |\n");
      printf("  %c  |  %c  |  %c  |  %c  |  %c  |\n", palya[0][0], palya[0][1], palya[0][2],palya[0][3],palya[0][4]);
      printf("_____|_____|_____|_____|_____|\n");
      printf("     |     |     |     |     |\n");
      printf("  %c  |  %c  |  %c  |  %c  |  %c  |\n", palya[1][0], palya[1][1], palya[1][2],palya[1][3],palya[1][4]);
      printf("_____|_____|_____|_____|_____|\n");
      printf("     |     |     |     |     |\n");
      printf("  %c  |  %c  |  %c  |  %c  |  %c  |\n", palya[2][0], palya[2][1], palya[2][2],palya[2][3],palya[2][4]);
      printf("_____|_____|_____|_____|_____|\n");
      printf("     |     |     |     |     |\n");
      printf("  %c  |  %c  |  %c  |  %c  |  %c  |\n", palya[3][0], palya[3][1], palya[3][2],palya[3][3],palya[3][4]);
      printf("_____|_____|_____|_____|_____|\n");
      printf("     |     |     |     |     |\n");
      printf("  %c  |  %c  |  %c  |  %c  |  %c  |\n", palya[4][0], palya[4][1], palya[4][2],palya[4][3],palya[4][4]);
      printf("     |     |     |     |     |\n\n");
}

int main()
{
      int serverSocket;//socketek létrehozása    
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
      int lepesszam;
      bool player1win;
      bool player2win;
      char palya[5][5];
      char playername1[19];
      char playername2[19];

      on                     = 1;
      flags                  = 0;
      bytes                  = BUFSIZE;
      server_size            = sizeof server;
      client_size            = sizeof client;
      server.sin_family      = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;//(0.0.0.0) 
      server.sin_port        = htons(PORT_NO);
      serverSocket = socket(AF_INET, SOCK_STREAM, 0);//sock_stream jelzi hogy TCP,0 default
      if(serverSocket<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk letrehozni a szerver socketet!\n");
        exit(1);
      }

	//opciók beállítása,kapcsolatbontás után a port újrahasznosítása	
      setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
      setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

	//Socket hozzárendelése a névtérhez (ipcím,portszám)
      err = bind(serverSocket, (struct sockaddr *) &server, server_size);
      if(err<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk bindelni a szerver socketet!\n");
          exit(2);
      }

	//kapcsolat elfogadási szándék
      err = listen(serverSocket, 2);
      if(err<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk hallgató módba kapcsolni a szervert!\n");
          exit(3);
      }

	//kapcsolat elfogadása és queue méret
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
          //értesítés küldése
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
        //addig kérjük be a játékos nevét amíg nem különbözik az elsőtől
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

      printf("A ket jatekos sikeresen felcsatlakozott!\n");

          

        while(1)
        {
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


              for (int i = 0; i < 5; ++i)
              {
                for (int j = 0; j <5; ++j)
                {
                  palya[i][j]='-';
                }
              }

              int sor,oszlop;
                    
              char jatekos1[8];
              char jatekos2[8];
                    
              lepesszam=0;
              player1win=false;
              player2win=false;

              while(1)
              { 
                //üzenünk a klienseknek hogy a játékosok küldjék el a lépésüket az első játékos kezd
                  
                sprintf(buffer, "ENTER_MOVE\n");
                vissza:
                bytes = strlen(buffer);
                trnmsize = send(playerSocket1, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy kuldje el a lépését!\n");
                    exit(9);
                }
                     
                rcvsize = recv(playerSocket1, jatekos1, 8, flags);
                if(rcvsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk fogadni az 1. jatekos lépését!\n");
                    exit(10);
                }
                jatekos1[rcvsize-1] = '\0';

                
                

                //Annak vizsgálata,hogy üres -e a mező ha nem újra bekérünk egy lépést, a feladom egy kivételes eset.
                if(strcmp(jatekos1,"feladom")==0)
                {
                    player2win=true;
                    break;
                }
                else
                {
                    sor=(int)jatekos1[0]-'1';
                    oszlop=(int)jatekos1[2]-'1';
                    if(palya[sor][oszlop]=='-')
                    {        
                          palya[sor][oszlop]='X';
                          lepesszam++;   
                    }
                    else
                    {
                        goto vissza;
                    }
                }

                sprintf(buffer, "PALYA\n");
                bytes = strlen(buffer);
                trnmsize = send(playerSocket1, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy megkapta -e a pálya adatait!\n");
                    exit(11);
                }
                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy megkapta -e a pálya adatait!\n");
                    exit(12);
                }
                sprintf(buffer, " -- PALYA ADATAI --\n %s\n",palya[0]);
                bytes = strlen(buffer);
                //1.játékos
                trnmsize = send(playerSocket1, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait az 1. jatekosnak!\n");
                    exit(13);
                }
                // 2. játékos
                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait a 2. jatekosnak!\n");
                    exit(14);
                }
                kirajzol(palya);

                //Annak vizsgálata, hogy az első játékos nyert-e vagy a lépésszám elérte -e a maximumot, azaz tele -e a tábla.
                for(int i = 0; i < 5; ++i)
                { 
                  if(palya[i][0]=='X'&&palya[i][1]=='X'&&palya[i][2]=='X'&&palya[i][3]=='X'&&palya[i][4]=='X')
                  {
                      player1win=true;
                  }
                  else if(palya[0][i]=='X'&&palya[1][i]=='X'&&palya[2][i]=='X'&&palya[3][i]=='X'&&palya[4][i]=='X')
                  {
                      player1win=true;
                  }
                          
                }
                if(palya[0][0]=='X'&&palya[1][1]=='X'&&palya[2][2]=='X'&&palya[3][3]=='X'&&palya[4][4]=='X')
                {
                    player1win=true;
                }
                if (lepesszam==25||player1win==true||player2win==true)
                {
                    break;
                }
                //ha nem volt üres a mező akkor ide tér vissza a program, játékos lépésének fogadása    
                
                sprintf(buffer, "ENTER_MOVE\n");
                vissza1:
                bytes = strlen(buffer);
                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy kuldje el a lépését!\n");
                    exit(15);
                }
                     
                rcvsize = recv(playerSocket2, jatekos2, 8, flags);
                if(rcvsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk fogadni a 2. jatekos lépését!\n");
                    exit(16);
                }
                jatekos2[rcvsize-1] = '\0';

                

                //ha a játékos2 feladja automatikusan a játékos1 nyer ha nem akkor megnézzük, hogy foglalt -e a mező ha nem akkor beírjuk a táblába ha igen akkor adjon meg új lépést
                if(strcmp(jatekos2,"feladom")==0)
                {
                    player1win=true;
                    break;
                }
                else
                {
                    sor=(int)jatekos2[0]-'1';
                    oszlop=(int)jatekos2[2]-'1';
                    if(palya[sor][oszlop]=='-')
                    {
                      palya[sor][oszlop]='O';
                      lepesszam++;
                    }
                    else
                    {
                      goto vissza1;
                    }
                }
                

                //pálya ,lépésszám kiiratás
                sprintf(buffer, "PALYA\n");
                bytes = strlen(buffer);
                trnmsize = send(playerSocket1, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy megkapta -e a pálya adatait!\n");
                    exit(17);
                }
                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy megkapta -e a pálya adatait!\n");
                    exit(18);
                }
                sprintf(buffer, " -- PALYA ADATAI --\n %s\n",palya[0]);
                bytes = strlen(buffer);

                trnmsize = send(playerSocket1, buffer, bytes, flags);
                if(trnmsize<0)
                {
                  fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait az 1. jatekosnak!\n");
                  exit(19);
                }
                // 2. játékos
                trnmsize = send(playerSocket2, buffer, bytes, flags);
                if(trnmsize<0)
                {
                    fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait a 2. jatekosnak!\n");
                    exit(20);
                }

                //pálya kirajzolása
                kirajzol(palya);
                //annak vizsgálata, hogy a második játékos nyer-e vagy elfogyott a lépés
                for(int i = 0; i < 5; ++i)
                {                  
                    if(palya[i][0]=='O'&&palya[i][1]=='O'&&palya[i][2]=='O'&&palya[i][3]=='O'&&palya[i][4]=='O')
                    {
                        player2win=true;
                    }
                    else if(palya[0][i]=='O'&&palya[1][i]=='O'&&palya[2][i]=='O'&&palya[3][i]=='O'&&palya[4][i]=='O')
                    {
                        player2win=true;
                    }
                }
                if(palya[0][0]=='O'&&palya[1][1]=='O'&&palya[2][2]=='O'&&palya[3][3]=='O'&&palya[4][4]=='O')
                {
                    player2win=true;
                }
                if (lepesszam==25||player1win==true||player2win==true)
                {
                    break;
                }
                     
              }

              //új játék kezdése
              //a \0 a string készítése miatt szükséges
              char choice1[5];
              char choice2[5];

              sprintf(buffer, "PLAY_AGAIN\n");
              bytes = strlen(buffer);

              trnmsize = send(playerSocket1, buffer, bytes, flags);
              if(trnmsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk megkerdezni az 1. jatekost, hogy akar-e visszavagot!\n");
                  exit(21);
              }

              rcvsize = recv(playerSocket1, choice1, 5, flags);

              if(rcvsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk fogadni, hogy az 1. jatekos akar-e visszavagot!\n");
                  exit(22);
              }
              choice1[rcvsize-1] = '\0';


              trnmsize = send(playerSocket2, buffer, bytes, flags);
              if(trnmsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk megkerdezni a 2. jatekost, hogy akar-e visszavagot!\n");
                  exit(23);
              }

              rcvsize = recv(playerSocket2, choice2, 5, flags);

              if(rcvsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk fogadni, hogy a 2. jatekos akar-e visszavagot!\n");
                  exit(24);
              }
              choice2[rcvsize-1] = '\0';

              if(!(strcmp(choice1, "igen")==0 && strcmp(choice2, "igen")==0))
              {
                  break;
              }

        }

      //Ha nem szavazták meg az új játékot akkor kiiratjuk,hogy melyik fél nyert
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
      if(lepesszam==25)
      {
          strcat(buffer, " :: DONTETLEN! ::\n");
      }
      else if(player1win==true)
      {
          strcat(buffer, " ::A KOVETKEZO JATEKOS NYERT! ::\n");
          strcat(buffer,playername1);
      }
      else if(player2win==true)
      {
          strcat(buffer, " :: A KOVETKEZO JATEKOS NYERT! ::\n");
          strcat(buffer,playername2);
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
      exit(0);
}
