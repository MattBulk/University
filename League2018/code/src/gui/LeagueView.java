package gui;

import java.awt.GridLayout;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import utils.CalendarTableModel;
import utils.RankingTableModel;
import utils.TeamTableModel;

/**
 * Classe per la gestione dei pannelli View
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class LeagueView extends JPanel {
	
	/**
	 * creo le quattro tab inizializzandole con i diversi moduli astratti delle table
	 */
	public LeagueView() {
		
		super(new GridLayout(1, 1));
		JTabbedPane tabbedPane = new JTabbedPane();
		
		addIt(tabbedPane, "Teams", new TabViewDefault("Teams", new TeamTableModel(), 100));
	    addIt(tabbedPane, "Calendar", new CalendarView("Calendar", new CalendarTableModel(), 30));
	    addIt(tabbedPane, "Ranking", new TabViewDefault("Ranking", new RankingTableModel(), 100));
	    addIt(tabbedPane, "Info", new InfoView());
	    
	    this.add(tabbedPane);
		
	}
	
	/**
	 * metodo aggiunge tabelle al Pane
	 * 
	 * @param tabbedPane JTabbedPane trasforma Jpanel in tab
	 * @param text titolo della tab
	 * @param panel il pannello specifico
	 */
	
	private void addIt(JTabbedPane tabbedPane, String text, JPanel panel) {
		
	    tabbedPane.addTab(text, panel);
	    tabbedPane.setTabComponentAt(tabbedPane.getTabCount() - 1, new JLabel(text));
	}
	
	
}
