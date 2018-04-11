package league.sports;

import utils.InfoStrings;

public class Basket extends Sport {
	
	private final int players = 5;
	private final int points = 2;
	
	@Override
	public String name() {
		// TODO Auto-generated method stub
		return "Basket";
	}

	@Override
	public int teamNumber() {
		// TODO Auto-generated method stub
		return players;
	}

	@Override
	public String rules() {
		// TODO Auto-generated method stub
		return InfoStrings.basket();
	}

	@Override
	public void statistics() {
		// TODO Auto-generated method stub

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
		return "[0-9]{1,3}-[0-9]{1,3}";
	}

}
