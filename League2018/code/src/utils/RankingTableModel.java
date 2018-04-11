package utils;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.table.AbstractTableModel;

import league.League;
import league.Team;

@SuppressWarnings("serial")
public class RankingTableModel extends AbstractTableModel {

	String[] ColName = {"Index", "Logo", "Team", "Points"};
	
	public RankingTableModel() {
		
		
	}
	@Override
	public int getRowCount() {
		// TODO Auto-generated method stub
		return League.getRankingVector().size();
	}

	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return ColName.length;
	}
	
	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
		Team theTeam = League.getRankingVector().get(rowIndex);
		
		String path = TheBuilder.filesystem + "tempDir/" + theTeam.getName();
		if(theTeam.getLogo().equals("no logo")) path = TheBuilder.filesystem + "icons/noLogo.png";
		
		Icon aboutIcon = new ImageIcon(path);
		switch (columnIndex) {
		case 0: return rowIndex + 1;
		case 1: return aboutIcon;
		case 2: return theTeam.getName();
		case 3: return theTeam.getPoints();
		default: return "";
			
		}
	}
	
	public String getColumnName(int col) {
		
		return ColName[col];
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public Class getColumnClass(int col) {
		
		return getValueAt(0, col).getClass();
	}

}
