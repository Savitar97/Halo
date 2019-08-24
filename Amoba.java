import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class Amoba{

	
	public static final String DEFAULT_ADDRESS = "127.0.0.1";
	public static final int DEFAULT_PORT = 60000;
	// a megadható értékek a lépéshez
	public static final char[] SZAMOK = { '1', '2', '3', '4', '5'};

	//felhasználónévbekérő fv
	public static boolean isCorrectName(String name)
	{
		boolean correctname=false;
		if(name.length()<2||name.length()>7)
		{	
			System.out.println("A névnek minimum 2, maximum 7 karakternek kell lennie");
			return false;
		}
		for(int i=0; i<name.length(); i++)
		{
		
			if(!((((int) name.charAt(i) >= 65) && ((int) name.charAt(i) <= 90)) || (((int) name.charAt(i) >= 97) && ((int) name.charAt(i) <= 122))))
			{
			
			System.out.println("Csak az angol abc betűit használhatod!");
				return false;
			}
			
		}
			return true;
	}





	public static boolean isCorrectInput(String input){
		
		//a feladás egy különleges eset ezért külön kezeljük
		if(input.equals("feladom"))
		{
			return true;
		}
	
		if(input.length()!=3)
		{
			System.out.println("A bemenetnek 3 karakter hosszunak kell lennie!");
			return false;
		}
		if(input.charAt(1)!='-')
		{
			System.out.println("A bemenetnek a 2.karakterének - nak kell lennie");
			return false;
		}

		for(int i=0; i<3; i++)
		{
			if(i==1)
			{continue;}

			boolean isCorrectCharacter = false;
			
			for(int j=0; j<5; j++)
				{
					if(input.charAt(i)==SZAMOK[j])
					{
						isCorrectCharacter = true;
						break;
					}
				}
			
			if(!isCorrectCharacter)
			{
				System.out.println("Csak a megfelelő sor,oszlopot adhatod meg!");
				return false;
			}
			
		}

		return true;
		}
	
		//új játék kezdéséhez a vizsgálat
		public static boolean isCorrectChoice(String choice)
		{
		return ( choice.equals("igen") || choice.equals("nem") );
		}


		public static void kirajzol(String palya)
		{
			  System.out.println("     |     |     |     |     |");
		      System.out.println("  "+palya.charAt(1)+"  |  "+palya.charAt(2)+"  |  "+palya.charAt(3)+"  |  "+palya.charAt(4)+"  |  "+palya.charAt(5)+"  |");
		      System.out.println("_____|_____|_____|_____|_____|");
		      System.out.println("     |     |     |     |     |");
		      System.out.println("  "+palya.charAt(6)+"  |  "+palya.charAt(7)+"  |  "+palya.charAt(8)+"  |  "+palya.charAt(9)+"  |  "+palya.charAt(10)+"  |");
		      System.out.println("_____|_____|_____|_____|_____|");
		      System.out.println("     |     |     |     |     |");
		      System.out.println("  "+palya.charAt(11)+"  |  "+palya.charAt(12)+"  |  "+palya.charAt(13)+"  |  "+palya.charAt(14)+"  |  "+palya.charAt(15)+"  |");
		      System.out.println("_____|_____|_____|_____|_____|");
		      System.out.println("     |     |     |     |     |");
		      System.out.println("  "+palya.charAt(16)+"  |  "+palya.charAt(17)+"  |  "+palya.charAt(18)+"  |  "+palya.charAt(19)+"  |  "+palya.charAt(20)+"  |");
		      System.out.println("_____|_____|_____|_____|_____|");
		      System.out.println("     |     |     |     |     |");
		      System.out.println("  "+palya.charAt(21)+"  |  "+palya.charAt(22)+"  |  "+palya.charAt(23)+"  |  "+palya.charAt(24)+"  |  "+palya.charAt(25)+"  |");
		      System.out.println("     |     |     |     |     |");
		}


	public static void main(String[] args) throws Exception
	{
		String serverAddress= DEFAULT_ADDRESS;
		int serverPort= DEFAULT_PORT;
		Socket socket = new Socket(serverAddress, serverPort);
		String palya="";
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
					System.out.print("Add meg a felhasználónevet: ");
				}while(!isCorrectName((myMsg = stdIn.readLine())));
				socketOut.println(myMsg);
				
			}
			else if(serverMsg.equals("WAIT"))
			{
				System.out.println("Várakozás a másik játékosra....");	
			}

			else if(serverMsg.equals("GAME_START"))
			{	
				System.out.println("Indul a jatek!!!");
				System.out.println("Az először csatlakozott játékos kezd ő az első játékos");
				System.out.println("Első_játékos:      X");
				System.out.println("Második_játékos:   O");
				System.out.println();
				System.out.print("Mezők és sorok:(A lépéshez ezekből a variációkból lehet választani a lépés megadása szám-szám alakban történik) ");
				for(int i=0; i<5; i++)
				{
					System.out.print(SZAMOK[i] + " ");
				}
				System.out.println();
			}
			
			else if(serverMsg.equals("ENTER_MOVE"))
			{
				//addig kérjük be az adatot amíg nem helyes
				do{
					System.out.print("Add meg a lépést: ");
				}while(!isCorrectInput((myMsg = stdIn.readLine())));

				//üzenet küldése a lépésről a szervernek
				socketOut.println(myMsg);
			}
			else if(serverMsg.equals("PALYA")){
				
				// fejléc beolvasása, kírása
				serverMsg = socketIn.readLine();
				serverMsg = socketIn.readLine();
				palya=serverMsg;
				kirajzol(palya);
			}
			
			else if(serverMsg.equals("PLAY_AGAIN")){
				
				System.out.println("Akarsz visszavagot jatszani?");
				do{
					System.out.print("(igen/nem) ");
				}while(!isCorrectChoice((myMsg = stdIn.readLine())));

				socketOut.println(myMsg);
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
		
		
		System.out.println("Kapcsolat bontasa.");
		socketOut.close();
		stdIn.close();
		socketIn.close();
		socket.close();

	}
}
