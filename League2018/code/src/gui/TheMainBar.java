package gui;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

/**
 * classe per la realizzazione del menu
 * 
 * @author badjoker
 *
 */

@SuppressWarnings("serial")
public class TheMainBar extends JMenuBar {
	
	private JMenu general;
	private JMenuItem newG;
	private JMenuItem openG;
	private JMenuItem saveG;
	private JMenuItem closeG;
	
	private JMenu team;
	private JMenuItem createT;
	private JMenuItem modifyT;
	
	private JMenu league;
	private JMenuItem rankingL;
	private JMenuItem calendarL;
	private JMenuItem deleteCalendarL;
	
	public TheMainBar() {
		
		init();
	}
	/**
	 * semplice creazione dei componenti del menu
	 */
	private void init() {
		
		general = new JMenu("File");
		
		newG = new JMenuItem("new");
		openG = new JMenuItem("open");
		saveG = new JMenuItem("save");
		closeG = new JMenuItem("close");
		
		closeG.setEnabled(false);
		saveG.setEnabled(false);
		
		
		newG.addActionListener(TheGui.eveDropper);
		openG.addActionListener(TheGui.eveDropper);
		saveG.addActionListener(TheGui.eveDropper);
		closeG.addActionListener(TheGui.eveDropper);
		
		general.add(newG);
		general.add(openG);
		general.add(saveG);
		general.add(closeG);
		
		this.add(general);
		
		team = new JMenu("Teams");
		
		createT = new JMenuItem("add");
		modifyT = new JMenuItem("modify");
		
		createT.addActionListener(TheGui.eveDropper);
		modifyT.addActionListener(TheGui.eveDropper);
		
		team.add(createT);
		team.add(modifyT);
		
		team.setEnabled(false);
		
		this.add(team);
		
		league = new JMenu("League");
		
		calendarL = new JMenuItem("create Calendar");
		rankingL = new JMenuItem("create ranking");
		deleteCalendarL = new JMenuItem("delete Calendar");
		
		league.add(calendarL);
		league.add(deleteCalendarL);
		league.add(rankingL);
		
		calendarL.addActionListener(TheGui.eveDropper);
		deleteCalendarL.addActionListener(TheGui.eveDropper);
		rankingL.addActionListener(TheGui.eveDropper);
		
		this.add(league);
		
		league.setEnabled(false);
		
		
	}
	
	/**
	 * 
	 * @param enable boolean per set enable disable ItemMenu
	 * @param nMenu quale menu
	 * @param itemName nome del ItemMenu
	 */
	public void setEnableByName(boolean enable, int nMenu, String itemName) {
		   
		JMenu menu = this.getMenu(nMenu);
		for (int i = 0 ; i <  menu.getItemCount(); i++) {
			
			JMenuItem item = menu.getItem(i);
		      
		    if(item.getText().equals(itemName)) item.setEnabled(enable); 
		      
		}   
	}
	
	/**
	 * 
	 * @param enable boolean per set enable disable ItemMenu
	 * @param nMenu quale menu
	 */
	
	public void setEnableByNumber(boolean enable, int nMenu) {
		
		this.getMenu(nMenu).setEnabled(enable);
	}

}
