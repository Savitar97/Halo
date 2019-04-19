import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class SajatKliens{

	
	public static final String DEFAULT_ADDRESS = "127.0.0.1";
	public static final int DEFAULT_PORT = 60000;

	public static final char[] SZAMOK = { '1', '2', '3', '4', '5'};

	public static boolean isCorrectInput(String input){
		

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
	

		public static boolean isCorrectChoice(String choice)
		{
		return ( choice.equals("igen") || choice.equals("nem") );
		}




	public static void main(String[] args) throws Exception{
		String serverAddress= DEFAULT_ADDRESS;
		int serverPort= DEFAULT_PORT;
		int counter=0;
		Socket socket = new Socket(serverAddress, serverPort);

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		BufferedReader socketIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		PrintWriter socketOut = new PrintWriter(socket.getOutputStream(), true);

		String myMsg = "";
		String serverMsg = "";
		System.out.println("Sikeres csatlakozas a szerverhez!");
		
		System.out.print("Mezők és sorok: ");
		for(int i=0; i<5; i++){
			System.out.print(SZAMOK[i] + " ");
		}
		System.out.println();
		while(true){
			
			// üzenet olvasása a szervertől
			serverMsg = socketIn.readLine();
			if(serverMsg.equals("WAIT"))
			{
				System.out.println("Várakozás a másik játékosra....");	
			}

			else if(serverMsg.equals("GAME_START"))
			{	
				System.out.println("Indul a jatek!!!");	
			}
			else if(serverMsg.equals("ENTER_MOVE"))
			{
				do{
					System.out.print("Add meg a lépést: ");
				}while(!isCorrectInput((myMsg = stdIn.readLine())));

				socketOut.println(myMsg);
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
			break;
			}


		}
		System.out.println("Kapcsolat bontasa.");
		socket.close();

	}
}