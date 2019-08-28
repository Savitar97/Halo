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
      int p1socket;           
      int p2socket;          
      int flags;                
      struct sockaddr_in server;//címinformáció ipcím portszám        
      struct sockaddr_in client;     
      int server_size;                  
      int client_size;  
      int bytes;//bufferben lévő üzenet hossza               
      int rcvsize;//fogadott üzenet mérete        
      int trnmsize;//elküldött üzenet mérete
      int opt;                         
      char buffer[BUFSIZE+1];//buffer amiben az üzenet tárolódik  
      int elsojatekos_hajo,masodikjatekos_hajo;//az első és második játékos hajóinak a száma
      bool player1win;
      bool player2win;
      char palya1[5][5];//az első játékos táblája amit feltölt hajókkal és csak ő lát
      char palya2[5][5];// a második játékos táblája amit feltölt hajókkal és csak ő lát
      char jatekos1_mutat[5][5];//az első játékos táblája amit a második játékos lát
      char jatekos2_mutat[5][5];//a második játékos táblája amit az első játékos lát

      opt                     = 1;
      flags                  = 0;
      bytes                  = BUFSIZE;//küldött byteok mérete
      server_size            = sizeof server;
      client_size            = sizeof client;
      server.sin_family      = AF_INET;//ipv4
      server.sin_addr.s_addr = INADDR_ANY;//(0.0.0.0) 
      server.sin_port        = htons(PORT_NO); //Port konvertálása előjel nélküli rövid integer "hostshort" forma az arpa inet könyvtár tartalmazza
      serverSocket = socket(AF_INET, SOCK_STREAM, 0);//AF_INET jelzi hogy ipv4,sock_stream jelzi hogy TCP,0 default
      if(serverSocket<0)
      {
        fprintf(stderr, "Hiba! Nem tudtuk letrehozni a szerver socketet!\n");
        exit(1);
      }

      //opciók beállítása,kapcsolatbontás után a port újrahasznosítása  
      if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

      //Socket hozzárendelése a névtérhez (ipcím,portszám)
      if (bind(serverSocket, (struct sockaddr *)&server,  
                                 sizeof(server))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    //a szerverre hányan tudnak felcsatlakozni -e fölött a szám fölött hibát ír ki
    if (listen(serverSocket, 2) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

      //kapcsolat elfogadása és queue méret &client kliens címének tárolása,client_size pointer amely a méretét tárolja a címnek ez a visszatérési értékét tárolja a cliensnek
      p1socket = accept(serverSocket, (struct sockaddr *) &client, (socklen_t*)&client_size);
      if(p1socket<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk elfogadni az 1. jatekos socketjet!\n");
          exit(4);
      }
      else
      {
          sprintf(buffer, "WAIT\n");
          bytes = strlen(buffer);
          // üzenet küldése az 1. játékosnak
          trnmsize = send(p1socket, buffer, bytes, flags);
          if(trnmsize<0)
          {
              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy felcsatlakozott -e a másik!\n");
              exit(5);
          }
      }

      p2socket = accept(serverSocket, (struct sockaddr *) &client, (socklen_t*)&client_size);
      if(p2socket<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk elfogadni a 2. jatekos socketjet!\n");
          exit(6);
      }

      printf("A ket jatekos sikeresen felcsatlakozott!\n");


      while(1)
      {
              //értesítem a két játékost, hogy elkezdődött a játék
              sprintf(buffer, "GAME_START\n");
              bytes = strlen(buffer);

              trnmsize = send(p1socket, buffer, bytes, flags);
              if(trnmsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost a jatek kezdeterol!\n");
                  exit(7);
              }

              trnmsize = send(p2socket, buffer, bytes, flags);
              if(trnmsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost a jatek kezdeterol!\n");
                  exit(8);
              }
              //táblák feltöltése space-el
            for (int i = 0; i < 5; ++i)
              {
                for (int j = 0; j <5; ++j)
                {
                  palya1[i][j]=' ';
                }
              }

            for (int i = 0; i < 5; ++i)
              {
                for (int j = 0; j <5; ++j)
                {
                  palya2[i][j]=' ';
                }
              }
            for (int i = 0; i < 5; ++i)
              {
                for (int j = 0; j <5; ++j)
                {
                  jatekos1_mutat[i][j]=' ';
                }
              }
            for (int i = 0; i < 5; ++i)
              {
                for (int j = 0; j <5; ++j)
                {
                  jatekos2_mutat[i][j]=' ';
                }
              }
              //a két változó amibe majd a koordinátákat alakítom át
            int sor,oszlop;
            //a két chat tömb amiben az érkezett üzenetet tárolom majd
            char jatekos1[8];
            char jatekos2[8];
            //beállítom a hajók kezdő értékét
            elsojatekos_hajo=0,masodikjatekos_hajo=0;
            player1win=false;
            player2win=false;
          // üzenet küldése az 1. játékosnak hogy elkezdheti a hajók lerakását
          sprintf(buffer, "HAJO\n");
          bytes = strlen(buffer);
          trnmsize = send(p1socket, buffer, bytes, flags);
          if(trnmsize<0)
          {
              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy a hajók lerakása következik!\n");
              exit(5);
          }
          //első játékos hajóinak a bekérése
            do
            {
                  sprintf(buffer, "ENTER_MOVE\n");
                  bytes = strlen(buffer);
                  trnmsize = send(p1socket, buffer, bytes, flags);
                  if(trnmsize<0)
                  {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy kuldje el a lépését!\n");
                    exit(9);
                  }
                     
                  rcvsize = recv(p1socket, jatekos1, 8, flags);
                  if(rcvsize<0)
                  {
                    fprintf(stderr, "Hiba! Nem tudtuk fogadni az 1. jatekos lépését!\n");
                    exit(10);
                   }
                  jatekos1[rcvsize-1] = '\0';
                  sor=(int)jatekos1[0]-'1';
                  oszlop=(int)jatekos1[2]-'1';
                  if(palya1[sor][oszlop]==' ')
                    {        
                          palya1[sor][oszlop]='O';
                          elsojatekos_hajo++;
                          printf("%d\n",elsojatekos_hajo );
                          sprintf(buffer, "PALYA\n");
                          bytes = strlen(buffer);
                          trnmsize = send(p1socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                            fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy megkapta -e a pálya adatait!\n");
                            exit(17);
                          }
                                  
                          sprintf(buffer, " -- PALYA ADATAI --\n %s\n",palya1[0]);
                          bytes = strlen(buffer);

                          trnmsize = send(p1socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                          fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait az 1. jatekosnak!\n");
                          exit(19);
                          }   
                    }
                  else
                  {

                        sprintf(buffer, "WRONG\n");
                        bytes = strlen(buffer);
                        trnmsize = send(p1socket, buffer, bytes, flags);
                        if(trnmsize<0)
                        {
                          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 1.játékost hogy hibás lépést adott meg!\n");
                          exit(9);
                        }
                        printf("%d\n",elsojatekos_hajo );
                  }
                        kirajzol(palya1);
            }
            while(elsojatekos_hajo<5);
            // üzenet küldése az 1. játékosnak hogy végzett a hajók lerakásával
            sprintf(buffer, "KESZ\n");
          bytes = strlen(buffer);
          trnmsize = send(p1socket, buffer, bytes, flags);
          if(trnmsize<0)
          {
              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy lerakta a hajóit!\n");
              exit(5);
          }
            sprintf(buffer, "HAJO\n");
          bytes = strlen(buffer);
          // üzenet küldése az 1. játékosnak
          trnmsize = send(p2socket, buffer, bytes, flags);
          if(trnmsize<0)
          {
              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy a hajók lerakása következik!\n");
              exit(5);
          }
            do
            {
                  sprintf(buffer, "ENTER_MOVE\n");
                  bytes = strlen(buffer);
                  trnmsize = send(p2socket, buffer, bytes, flags);
                  if(trnmsize<0)
                  {
                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 2. jatekost, hogy kuldje el a lépését!\n");
                    exit(9);
                  }
                     
                  rcvsize = recv(p2socket, jatekos2, 8, flags);
                  if(rcvsize<0)
                  {
                    fprintf(stderr, "Hiba! Nem tudtuk fogadni az 2. jatekos lépését!\n");
                    exit(10);
                   }
                  jatekos2[rcvsize-1] = '\0';
                  sor=(int)jatekos2[0]-'1';
                  oszlop=(int)jatekos2[2]-'1';
                  if(palya2[sor][oszlop]==' ')
                    {        
                          palya2[sor][oszlop]='O';
                          masodikjatekos_hajo++;
                          printf("%d\n",masodikjatekos_hajo);
                          sprintf(buffer, "PALYA\n");
                          bytes = strlen(buffer);
                          trnmsize = send(p2socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                            fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 2. jatekost, hogy megkapta -e a pálya adatait!\n");
                            exit(17);
                          }
                                  
                          sprintf(buffer, " -- PALYA ADATAI --\n %s\n",palya2[0]);
                          bytes = strlen(buffer);

                          trnmsize = send(p2socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                          fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait az 2. jatekosnak!\n");
                          exit(19);
                          }  
                    }
                  else
                  {

                        sprintf(buffer, "WRONG\n");
                        bytes = strlen(buffer);
                        trnmsize = send(p2socket, buffer, bytes, flags);
                        if(trnmsize<0)
                        {
                          fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2.játékost hogy hibás lépést adott meg!\n");
                          exit(9);
                        }
                        printf("%d\n",masodikjatekos_hajo);  
                        
                  }
                  
                        kirajzol(palya2);
            }
            while(masodikjatekos_hajo<5);
            sprintf(buffer, "KESZ\n");
            bytes = strlen(buffer);
            // üzenet küldése az 2. játékosnak
            trnmsize = send(p2socket, buffer, bytes, flags);
            if(trnmsize<0)
            {
                fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy lerakta a hajóit!\n");
                exit(5);
            }
            // üzenet küldése az 1. játékosnak, hogy a tippelési fázis következik
            sprintf(buffer, "TIPP\n");
            bytes = strlen(buffer);

            trnmsize = send(p1socket, buffer, bytes, flags);
            if(trnmsize<0)
            {
                fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy a hajók lerakása következik!\n");
                exit(5);
            }
            trnmsize = send(p2socket, buffer, bytes, flags);
            if(trnmsize<0)
            {
                fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy a hajók lerakása következik!\n");
                exit(5);
            }

                    //tippelős fázis:
                      while(1)
                      {
                          sprintf(buffer, "ENTER_MOVE\n");
                          bytes = strlen(buffer);
                          trnmsize = send(p1socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy kuldje el a lépését!\n");
                              exit(9);
                          }
                               
                          rcvsize = recv(p1socket, jatekos1, 8, flags);
                          if(rcvsize<0)
                          {
                              fprintf(stderr, "Hiba! Nem tudtuk fogadni az 1. jatekos lépését!\n");
                              exit(10);
                          }
                          jatekos1[rcvsize-1] = '\0';
                          //győztes vizsgálata
                          if(strcmp(jatekos1,"feladom")==0)
                          {
                              player2win=true;
                              break;
                          }
                           else
                          {
                              sor=(int)jatekos1[0]-'1';
                              oszlop=(int)jatekos1[2]-'1';
                              if(palya2[sor][oszlop]==' ')
                              {     
                                    sprintf(buffer, "NEMTALALT\n");
                                  bytes = strlen(buffer);
                                  trnmsize = send(p1socket, buffer, bytes, flags);
                                  if(trnmsize<0)
                                  {
                                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 1.játékost hogy nem találta el a hajót!\n");
                                    exit(9);
                                  }
                                    palya2[sor][oszlop]='X';
                                    jatekos2_mutat[sor][oszlop]='X';
                                      
                              }
                              else if(palya2[sor][oszlop]=='O')
                              {
                                sprintf(buffer, "TALALT\n");
                                bytes = strlen(buffer);
                                trnmsize = send(p1socket, buffer, bytes, flags);
                                if(trnmsize<0)
                                {
                                  fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 1.játékost hogy eltalálta a hajót!\n");
                                  exit(9);
                                }
                                palya2[sor][oszlop]='@';
                                jatekos2_mutat[sor][oszlop]='@';
                                masodikjatekos_hajo--;
                                if(masodikjatekos_hajo==0)
                                {
                                  player1win=true;
                                  break;
                                }
                              }
                          }

                          //pálya elküldése az első játékosnak
                          sprintf(buffer, "PALYA\n");
                          bytes = strlen(buffer);
                          trnmsize = send(p1socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 1. jatekost, hogy megkapta -e a pálya adatait!\n");
                              exit(17);
                          }
                          
                          sprintf(buffer, " -- PALYA ADATAI --\n %s\n",jatekos2_mutat[0]);
                          bytes = strlen(buffer);

                          trnmsize = send(p1socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                            fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait az 1. jatekosnak!\n");
                            exit(19);
                          }
                          sprintf(buffer, "ENTER_MOVE\n");
                          bytes = strlen(buffer);
                          trnmsize = send(p2socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2. jatekost, hogy kuldje el a lépését!\n");
                              exit(15);
                          }
                               
                          rcvsize = recv(p2socket, jatekos2, 8, flags);
                          if(rcvsize<0)
                          {
                              fprintf(stderr, "Hiba! Nem tudtuk fogadni a 2. jatekos lépését!\n");
                              exit(16);
                          }
                          jatekos2[rcvsize-1] = '\0';
                          if(strcmp(jatekos2,"feladom")==0)
                          {
                              player1win=true;
                              break;
                          }
                           else
                          {
                              sor=(int)jatekos2[0]-'1';
                              oszlop=(int)jatekos2[2]-'1';
                              if(palya1[sor][oszlop]==' ')
                              {     
                                    sprintf(buffer, "NEMTALALT\n");
                                  bytes = strlen(buffer);
                                  trnmsize = send(p2socket, buffer, bytes, flags);
                                  if(trnmsize<0)
                                  {
                                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2.játékost hogy nem találta el a hajót!\n");
                                    exit(9);
                                  }
                                    palya1[sor][oszlop]='X';
                                    jatekos1_mutat[sor][oszlop]='X';
                                      
                              }
                              else if(palya1[sor][oszlop]=='O')
                              {
                                  sprintf(buffer, "TALALT\n");
                                  bytes = strlen(buffer);
                                  trnmsize = send(p2socket, buffer, bytes, flags);
                                  if(trnmsize<0)
                                  {
                                    fprintf(stderr, "Hiba! Nem tudtuk ertesiteni a 2.játékost hogy eltalálta a hajót!\n");
                                    exit(9);
                                  }
                                palya1[sor][oszlop]='@';
                                jatekos1_mutat[sor][oszlop]='@';
                                elsojatekos_hajo--;
                                if(elsojatekos_hajo==0)
                                  {
                                    player2win=true;
                                    break;
                                  }
                              }
                          }
                          sprintf(buffer, "PALYA\n");
                          bytes = strlen(buffer);
                          trnmsize = send(p2socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                              fprintf(stderr, "Hiba! Nem tudtuk ertesiteni az 2. jatekost, hogy megkapta -e a pálya adatait!\n");
                              exit(17);
                          }
                          
                          sprintf(buffer, " -- PALYA ADATAI --\n %s\n",jatekos1_mutat[0]);
                          bytes = strlen(buffer);

                          trnmsize = send(p2socket, buffer, bytes, flags);
                          if(trnmsize<0)
                          {
                            fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a pálya adatait az 2. jatekosnak!\n");
                            exit(19);
                          }
                          kirajzol(jatekos2_mutat);
                          kirajzol(jatekos1_mutat);
                      }
              //ha valamelyik játékos nyer befejeződik a tippelős fázis és jön az új játékra a lehetőség
              char choice1[5];
              char choice2[5];

              sprintf(buffer, "PLAY_AGAIN\n");
              bytes = strlen(buffer);

              trnmsize = send(p1socket, buffer, bytes, flags);
              if(trnmsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk megkerdezni az 1. jatekost, hogy akar-e visszavagot!\n");
                  exit(21);
              }

              rcvsize = recv(p1socket, choice1, 5, flags);

              if(rcvsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk fogadni, hogy az 1. jatekos akar-e visszavagot!\n");
                  exit(22);
              }
              choice1[rcvsize-1] = '\0';


              trnmsize = send(p2socket, buffer, bytes, flags);
              if(trnmsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk megkerdezni a 2. jatekost, hogy akar-e visszavagot!\n");
                  exit(23);
              }

              rcvsize = recv(p2socket, choice2, 5, flags);

              if(rcvsize<0)
              {
                  fprintf(stderr, "Hiba! Nem tudtuk fogadni, hogy a 2. jatekos akar-e visszavagot!\n");
                  exit(24);
              }
              choice2[rcvsize-1] = '\0';
              //ha mindeketten igenre szavaznak a ciklus újra kezdődik egyébként kilépünk a ciklusból
              if((strcmp(choice1, "igen")==0 && strcmp(choice2, "igen")==0))
              {
                  continue;
              }
              else
              {
                  break;
              }

      }
      //eredmény kiküldése
      sprintf(buffer, "GAME_OVER\n");
      bytes = strlen(buffer);
         
      trnmsize = send(p1socket, buffer, bytes, flags);
      if(trnmsize<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk megmondani az 1. jatekosnak, hogy vege a jateknak!\n");
          exit(25);
      }
         
      trnmsize = send(p2socket, buffer, bytes, flags);
      if(trnmsize<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk megmondani a 2. jatekosnak, hogy vege a jateknak!\n");
          exit(26);
      }

      sprintf(buffer, " :: VEGEREDMENY ::\n");
      if(player1win==true)
      {
          strcat(buffer, " :: AZ 1. JATEKOS NYERT! ::\n");
      }
      else if(player2win==true)
      {
          strcat(buffer, " :: A 2. JATEKOS NYERT! ::\n");
      }
      bytes = strlen(buffer);

      trnmsize = send(p1socket, buffer, bytes, flags);
      if(trnmsize<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a vegeredmenyt az 1. jatekosnak!\n");
          exit(27);
      }

      trnmsize = send(p2socket, buffer, bytes, flags);
      if(trnmsize<0)
      {
          fprintf(stderr, "Hiba! Nem tudtuk elkuldeni a vegeredmenyt a 2. jatekosnak!\n");
          exit(28);
      }
      //socketek lezárása
      close(p2socket);
      close(p1socket);
      close(serverSocket);
      exit(0);
}