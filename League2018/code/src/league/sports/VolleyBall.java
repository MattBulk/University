package league.sports;

import utils.InfoStrings;

public class VolleyBall extends Sport {
	
	private final int players = 6;
	private final int points = 3;
	@Override
	public String name() {
		// TODO Auto-generated method stub
		return "VolleyBall";
	}

	@Override
	public int teamNumber() {
		// TODO Auto-generated method stub
		return players;
	}

	@Override
	public String rules() {
		// TODO Auto-generated method stub
		return InfoStrings.volleyball();
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
		else if(h > v && t == 0) return points; // Did we win at home ???
		else if(v > h && t == 1) return points; // Did we win as visitors ???
		else if(Math.abs(h-v) == 1) return 1;	// DId we at least got one point ???
		return 0;
	}

	@Override
	public String regEx() {
		// TODO Auto-generated method stub
		return "[0-3]{1}-[0-3]{1}";
	}

}
