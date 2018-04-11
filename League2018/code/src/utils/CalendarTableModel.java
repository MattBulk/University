package utils;

import javax.swing.table.AbstractTableModel;

import org.w3c.dom.NodeList;

import gui.TheGui;
import league.League;

@SuppressWarnings("serial")
public class CalendarTableModel extends AbstractTableModel {

	String[] ColName = {"Matchday", "TeamOne", "TeamTwo", "Score"};
	
	public CalendarTableModel() {
		
		
	}

	@Override
	public int getRowCount() {
		// TODO Auto-generated method stub
		if(TheBuilder.theBuilder.isActiveCalendar()) return League.getDoc().getElementsByTagName("WEEK").getLength();
		return 0;
	}

	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return ColName.length;
	}

	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
		if(TheBuilder.theBuilder.isActiveCalendar()) {
			
			NodeList nList = League.getDoc().getElementsByTagName("WEEK");
			
			switch (columnIndex) {
			case 0: return nList.item(rowIndex).getAttributes().item(0).getTextContent();
			case 1: return nList.item(rowIndex).getChildNodes().item(0).getTextContent();
			case 2: return nList.item(rowIndex).getChildNodes().item(1).getTextContent();
			case 3: return nList.item(rowIndex).getChildNodes().item(2).getTextContent();
			default: return "";
				
			}
			
		}
		
		return null;
	}
	
	public String getColumnName(int col) {
		
		return ColName[col];
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public Class getColumnClass(int col) {
		
		return getValueAt(0, col).getClass();
	
	}
	
	public boolean isCellEditable(int rowIndex, int columnIndex) {
		
		if(columnIndex == 3) return true;
		return false;
		
		
	}
	
	public void setValueAt(Object value, int rowIndex, int columnIndex) {
		
		NodeList nList = League.getDoc().getElementsByTagName("WEEK");
		
		if(columnIndex == 3) {
			if(checkString(value.toString())) {
				
				nList.item(rowIndex).getChildNodes().item(2).setTextContent(value.toString());
				
			}
			
			else TheGui.theGuiInstance.createPopUp("usa formato N-N", "Regex");
			
		}
		
		fireTableDataChanged();
		TheBuilder.theBuilder.setProjectSTATE("unsaved");
	}
	
	private boolean checkString(String str) {
		
		boolean b = str.matches(League.getCurrentSport().regEx());
		
		return b;
	}

}


