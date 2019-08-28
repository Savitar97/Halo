import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class Akasztofa
{

	
	public static final String DEFAULT_ADDRESS = "127.0.0.1";
	public static final int DEFAULT_PORT = 35000;

	//felhasználónévbekérő fv
	public static boolean isUsernameCorrect(String username)
	{
		boolean correctname=false;
		if(username.length()<4||username.length()>8)
		{	
			System.out.println("A felhasználónévnek 4-től 8 karakter hosszú lehet!");
			return false;
		}
		if(!(isOnlyASCII(username, 'b')))
		{
			return false;
		}
		return true;
	}

	public static boolean isOnlyASCII(String testWord, char option)
	{
		boolean onlyASCII=true;
		switch(option)
		{
			case 'b':
			{
				for(int i=0; i<testWord.length(); i++)
				{

					if(!((((int) testWord.charAt(i) >= 48) && ((int) testWord.charAt(i) <= 57)) || (((int) testWord.charAt(i) >= 65) && ((int) testWord.charAt(i) <= 90)) || (((int) testWord.charAt(i) >= 97) && ((int) testWord.charAt(i) <= 122))))
					{

						System.out.println("Csak az angol abc kis és nagy betűit használhatod valamint számokat!");
						onlyASCII= false;
					}
					
				}
				break;
			}

			case 'l':
			{
				for(int i=0; i<testWord.length(); i++)
				{

					if(!((((int) testWord.charAt(i) >= 65) && ((int) testWord.charAt(i) <= 90)) || (((int) testWord.charAt(i) >= 97) && ((int) testWord.charAt(i) <= 122))))
					{

						System.out.println("Csak az angol abc kis és nagy betűit használhatod!");
						onlyASCII= false;
					}
					
				}
				break;
			}
			case 's':
			{
				for(int i=0; i<testWord.length(); i++)
				{

					if(!(((int) testWord.charAt(i) >= 97) && ((int) testWord.charAt(i) <= 122)))
					{

						System.out.println("Csak az angol abc kis és nagy betűit használhatod!");
						onlyASCII= false;
					}
					
				}
				break;
			}
			case 'n':
			{
				for(int i=0; i<testWord.length(); i++)
				{

					if(!(((int) testWord.charAt(i) >= 48) && ((int) testWord.charAt(i) <= 57)))
					{

						System.out.println("Csak számokat használhatsz!");
						onlyASCII= false;
					}
					
				}
				break;
			}
			default:
			{
				for(int i=0; i<testWord.length(); i++)
				{

					if(!((((int) testWord.charAt(i) >= 65) && ((int) testWord.charAt(i) <= 90)) || (((int) testWord.charAt(i) >= 97) && ((int) testWord.charAt(i) <= 122))))
					{

						System.out.println("Csak az angol abc kis és nagy betűit használhatod!");
						onlyASCII= false;
					}
					
				}
				break;
			}
		}
		return onlyASCII;
	}





	public static boolean isCorrectInput(String input)
	{
		
		//a feladás egy különleges eset ezért külön kezeljük
		if(input.equals("/feladom"))
		{
			return true;
		}

		if(input.equals("/help"))
		{
			return true;
		}

		if(input.length()>=16)
		{
			System.out.println("A bemenet maximum 16 karakter lehet!");
			return false;
		}
		if(!(isOnlyASCII(input,'s')))
		{
			return false;
		}

		return true;
	}
	
		//új játék kezdéséhez a vizsgálat
	public static boolean isCorrectChoice(String choice)
	{
		return ( choice.equals("igen") || choice.equals("nem") );
	}


	public static void drawGallow(int elet)
	{
		switch(elet)
		{
			case 10:
			{
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				break;
			}

			case 9:
			{
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 8:
			{
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 7:
			{
				System.out.println("_______");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 6:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 5:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|     O");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 4:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|     O");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 3:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|     O");
				System.out.println("|    /|");
				System.out.println("|     |");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 2:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|     O");
				System.out.println("|    /|\\");
				System.out.println("|     |");
				System.out.println("|");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 1:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|     O");
				System.out.println("|    /|\\");
				System.out.println("|     |");
				System.out.println("|    /");
				System.out.println("|__w___v____ww_");
				break;
			}
			case 0:
			{
				System.out.println("_______");
				System.out.println("|     |");
				System.out.println("|     |");
				System.out.println("|     O");
				System.out.println("|    /|\\");
				System.out.println("|     |");
				System.out.println("|    / \\");
				System.out.println("|__w___v____ww_");
				break;
			}
		}
	}


	public static void main(String[] args) throws Exception
	{
		String serverAddress= DEFAULT_ADDRESS;
		int serverPort= DEFAULT_PORT;
		Socket socket = new Socket(serverAddress, serverPort);
		int hp;
		String word_dotted;
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		BufferedReader socketIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		PrintWriter socketOut = new PrintWriter(socket.getOutputStream(), true);

		String myMsg = "";
		String serverMsg = "";
		System.out.println("Sikeres csatlakozas a szerverhez!");
		
		
		System.out.println();
		while(true){
			
			// üzenet olvasása a szervertől
			serverMsg = socketIn.readLine();
			if (serverMsg.equals("PLAYERNAME")) {
				do{
					System.out.print("Add meg a felhasználóneved: ");
				}while(!isUsernameCorrect((myMsg = stdIn.readLine())));
				socketOut.println(myMsg);
				
			}
			else if(serverMsg.equals("WAIT"))
			{
				System.out.println("Várakozás a másik játékosra....");	
			}

			else if(serverMsg.equals("GAME_START"))
			{	
				System.out.println();
				System.out.println();
				System.out.println("Indul a játék!!!");
				System.out.println("Az először csatlakozott játékos kezd ő az első játékos");
				System.out.println("Első_játékos:      Kitalálandó szó megadása");
				System.out.println("Második_játékos:   A megadott szó kitalálása");
				System.out.println();
				System.out.println("A szó maximális mérete 16 karakter lehet ékezetek nélkül!");
				System.out.println("A második játékos által megadható bemenetek:");
				System.out.println("/help - Megmutat egy karaktert 4 életért cserébe");
				System.out.println("/feladom - A második játékos feladja a játékot és azonnal veszít");
				System.out.println("Az angol abc egyik betűje, amely csak kisbetű lehet..");
				System.out.println("A teljes szó, amely az angol abc kis betüiből épülhet fel..");
			}
			
			else if(serverMsg.equals("SZO"))
			{
				//addig kérjük be az adatot amíg nem helyes
				do{
					System.out.print("Add meg a kitalálandó szót: ");
				}while(!isCorrectInput((myMsg = stdIn.readLine())));

				//üzenet küldése a lépésről a szervernek
				socketOut.println(myMsg);
			}
			/*else if(serverMsg.equals("SZOHOSSZ")){
				
				// fejléc beolvasása, kírása
				serverMsg = socketIn.readLine();
				szohossz=Integer.parseInt(serverMsg);
				System.out.println(szohossz);
			}*/
			else if(serverMsg.equals("WORD_DOTTED")){
				
				// fejléc beolvasása, kírása
				serverMsg = socketIn.readLine();
				word_dotted=serverMsg;
				System.out.println(word_dotted);
			}
			else if(serverMsg.equals("ELET")){
				
				// fejléc beolvasása, kírása
				serverMsg = socketIn.readLine();
				hp=Integer.parseInt(serverMsg);
				drawGallow(hp);	
				System.out.println(hp);
			}
			else if(serverMsg.equals("TIPP")){
				do{
					System.out.print("Add meg a tipped: ");
				}while(!isCorrectInput((myMsg = stdIn.readLine())));

				//üzenet küldése a lépésről a szervernek
				socketOut.println(myMsg);
			}
			
			else if(serverMsg.equals("PLAY_AGAIN")){
				
				System.out.println("Akarsz visszavágot játszani?");
				do{
					System.out.print("(igen/nem) ");
				}while(!isCorrectChoice((myMsg = stdIn.readLine())));

				socketOut.println(myMsg);
			}
			else if(serverMsg.equals("SESSION"))
			{
				serverMsg = socketIn.readLine();
				System.out.println(serverMsg);
				serverMsg = socketIn.readLine();
				System.out.println(serverMsg);
				serverMsg = socketIn.readLine();
				System.out.println(serverMsg);
			}
			else if(serverMsg.equals("GAME_OVER"))
			{
				serverMsg = socketIn.readLine();
				System.out.println(serverMsg);
				serverMsg = socketIn.readLine();
				System.out.println(serverMsg);
				serverMsg = socketIn.readLine();
				System.out.println(serverMsg);
				break;
			}
			


		}
		
		
		System.out.println("Kapcsolat bontása.");
		socketOut.close();
		stdIn.close();
		socketIn.close();
		socket.close();

	}
}

/*
1_______
2|     |
3|     |
4|     O    X_X  U DIED
5|    /|\   ===========
6|     |
7|    / \
8|__w___v____ww_

10: Semmi
9: Talaj
8: Oszlop
7: Keresztfa
6: Kötél
5: Fej
4: Test
3: Bal kéz
2: Jobb kéz
1: Bal láb
0: Jobb láb
*/
