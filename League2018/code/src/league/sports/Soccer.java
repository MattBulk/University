/**
 * 
 */
package league.sports;

import utils.InfoStrings;

/**
 * @author badjoker
 *
 */
public class Soccer extends Sport {

	private final int num = 11;
	private final int points = 3;
	
	@Override
	public String name() {
		// TODO Auto-generated method stub
		return "Soccer";
	}
	

	@Override
	public String rules() {
		// TODO Auto-generated method stub
		return InfoStrings.soccer();
	}

	@Override
	public void statistics() {
		// TODO Auto-generated method stub
		System.out.println("richiesta delle statistiche");

	}

	@Override
	public int teamNumber() {
		// TODO Auto-generated method stub
		return num;
	}

	@Override
	public int pointForGame(int t, String score) {
		// TODO Auto-generated method stub
		String arr[];
		
		arr = score.split("-");
		
		int h = Integer.parseInt(arr[0]);
		int v = Integer.parseInt(arr[1]);
		
		if(h == v) return 1;
		else if(h > v && t == 0) return points;
		else if(v > h && t == 1) return points;
		return 0;
		
	}


	@Override
	public String regEx() {
		// TODO Auto-generated method stub
		return "[0-9]{1,2}-[0-9]{1,2}";
	}

}
