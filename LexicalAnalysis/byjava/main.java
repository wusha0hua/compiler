import java.io.*;
public class main
{
	public static void main(String[] args)
	{
		String path = args[0];
		StringBuilder text = new StringBuilder();

	try	
	{

		File f = new File(path);

		InputStreamReader isr = new InputStreamReader(new FileInputStream(f)); 
		
		BufferedReader br = new BufferedReader(isr);
		
		String line; 
		while((line = br.readLine()) != null)
		{
			text.append(line + '\n');
		}

		System.out.println(text);
		
		
		
		br.close();		
	}
	catch(IOException e)
	{

	}
		LexicalAnalysis l = new LexicalAnalysis();
		
		l.LoadText(text.toString());
		if(l.Analyse())
		{
			l.PrintToken();
		}
		else
		{
			System.out.println("error");
		}
	}


}
