package gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.print.PrinterException;
import java.text.MessageFormat;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTable.PrintMode;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;

import league.League;

/**
 * Una classe padre con gran parte dei componenti che verranno utilizzato dalle classi figlio
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class TabViewDefault extends JPanel implements TableModelListener, ActionListener {
	
	protected JTable table;
	protected JButton printBtn, updateBtn;
	protected TableModel dataModel;
	
	protected JPanel p;
	protected JPanel p2;
	
	/**
	 * 
	 * @param text testo della tab
	 * @param dataModel modello astratto della table
	 * @param rowHeight altezza delle righe
	 */
	public TabViewDefault(String text, TableModel dataModel, int rowHeight) {
		
		setLayout(new BorderLayout(30, 30));
		
		JLabel label = new JLabel(text);
		
		updateBtn = new JButton("update");
		updateBtn.addActionListener(this);
	    
		p = new JPanel();
	    
	    printBtn = new JButton("print");
	    printBtn.addActionListener(this);
	    
	    this.dataModel = dataModel;
	    
	    table = new JTable(dataModel);
	    
	    table.setRowHeight(rowHeight);
	    
	    table.getModel().addTableModelListener(this);
	    
	    table.setPreferredScrollableViewportSize(table.getPreferredSize());
	    
	    table.setAutoscrolls(true);
	    
	    p.add(label);
	    p.add(updateBtn);
	    p.add(printBtn);
	    
	    this.add(p, BorderLayout.NORTH);
	    
	    p2 = new JPanel();
	    JScrollPane scrollPane = new JScrollPane(table);
	    scrollPane.setPreferredSize(new Dimension(1000, 600));
	    
	    //scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
	    table.setFillsViewportHeight(true);
	    p2.add(scrollPane);
	    this.add(p2, BorderLayout.CENTER);
	
	}
	
	/**
	 * gestisce gli eventi tabella cambiata
	 */
	@Override
	public void tableChanged(TableModelEvent e) {
		// TODO Auto-generated method stub
		
		
	}

	/**
	 * gestisce i bottoni e spedisce un evento update alla tabella
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if(e.getActionCommand() == "print") {
			
			if(table.getRowCount() > 0) printTheTable();
			else {
				JOptionPane.showMessageDialog(this, "Where's nothing to print, do something!");
				
			}
			
		}
		else {
			
			try {
				((AbstractTableModel) dataModel).fireTableDataChanged();
			} catch (Exception e2) {
				// TODO: handle exception
				System.out.println(e2);
			}
			
		}
		
	}
	
	/**
	 *  metodo per la gestione della stampa
	 */
	private void printTheTable() {
		
		try {
			
			String srt = "LEAGUE 2018 " + League.getDoc().getElementsByTagName("SPORT").item(0).getAttributes().item(0).getTextContent();
			MessageFormat header = new MessageFormat(srt);
			MessageFormat footer = new MessageFormat("Page - {0}");
			
		    boolean complete = table.print(PrintMode.FIT_WIDTH, header, footer, true, null, false, null);
		    if (complete) {
		        /* show a success message  */
		    	JOptionPane.showMessageDialog(this,
                        "Printing Complete",
                        "Printing Result",
                        JOptionPane.INFORMATION_MESSAGE);
		        
		    } else {
		        /*show a message indicating that printing was cancelled */
		    	JOptionPane.showMessageDialog(this,
                        "Printing Cancelled",
                        "Printing Result",
                        JOptionPane.INFORMATION_MESSAGE);
		    }
		} catch (PrinterException pe) {
		    /* Printing failed, report to the user */
			JOptionPane.showMessageDialog(this,
                    "Printing Failed: " + pe.getMessage(),
                    "Printing Result",
                    JOptionPane.ERROR_MESSAGE);
		}
		
		
	}

}
