import utils.TheBuilder;

/**
 * @author badjoker
 * 
 * la Main lancia il programma e il Builder
 *
 */

public class Main {
	

	@SuppressWarnings("unused")
	private TheBuilder theB;
	
	/**
	 * 
	 * @param s Stringa contenente il tipo di sport selezionato se esiste
	 */
	public Main(String s) {
		
		init(s);

	}
	
	/**
	 * 
	 * @param args array del main
	 * 
	 */

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		if(args.length == 0) {
			new Main(" ");
		
		}
		else new Main(args[0]);
	}
	
	/**
	 * Il metodo init lancia l'assemblatore le varie astrazioni del programma
	 * 
	 * @param s Stringa dello sport
	 */
	
	private void init(String s) {
		
		theB = new TheBuilder(s);

	}

}
