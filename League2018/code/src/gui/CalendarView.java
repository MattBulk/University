package gui;

import java.awt.event.ActionEvent;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JProgressBar;
import javax.swing.JTextField;
import javax.swing.event.TableModelEvent;
import javax.swing.table.TableModel;

import league.Calendar;
import league.League;
import utils.ReadAndWrite;
import utils.RowFilterUtils;
import utils.TheBuilder;

/**
 * estensione della classe madre TabView per la gestione del Calendario
 * 
 * @author badjoker
 *
 */

@SuppressWarnings("serial")
public class CalendarView extends TabViewDefault {
	
	private JButton resetBtn; 
	private JProgressBar pBar;
	private JButton exportBtn;
	
	private JTextField filterField;
	
	/**
	 * 
	 * @param text : passa il testo da imporre alla label
	 * @param dataModel : passo il modello specifico della table
	 * @param rowHeight : passo l'altezza in pixel della righe di tabella
	 */
	
	public CalendarView(String text, TableModel dataModel, int rowHeight) {
		
		super(text, dataModel, rowHeight);
		
		resetBtn = new JButton("reset scores");
		resetBtn.addActionListener(this);
		
		this.p.add(resetBtn, 2);
		
		exportBtn = new JButton("export");
		exportBtn.addActionListener(this);
		
		this.p.add(exportBtn, 2);
		
		pBar = new JProgressBar();
	    pBar.setMinimum(0);
	    pBar.setStringPainted(true);
	    
	    this.p.add(pBar);
	    
	    filterField = RowFilterUtils.createRowFilter(table);

	    this.p.add(filterField);
	    
	}
	
	/**
	 * il metodo gestisce i vari casi dei bottoni del pannello Calendar
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		super.actionPerformed(e);
		
		switch (e.getActionCommand()) {
		case "export":
			if(TheBuilder.theBuilder.isActiveCalendar()) {
				
				System.out.println("ok");
				try {
					
					 ReadAndWrite.exportToTXT(League.getDoc(), pBar);
					
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				
			}
			
			break;
		case "reset scores":
			
			if(TheBuilder.theBuilder.isActiveCalendar()) Calendar.resetCalendarWeeksResults();
			break;
		}
		
	}
	
	/**
	 * metodo per catturare le modifiche effettuate alla table
	 */
	@Override
	public void tableChanged(TableModelEvent e) {
		// TODO Auto-generated method stub
		pBar.setValue(0);
		
		
	}

}
