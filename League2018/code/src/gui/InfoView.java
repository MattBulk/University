package gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
/**
 * Un semplice pannello per la visione delle regole di ogni sport
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class InfoView extends JPanel {
	
	private JPanel p, p2;
	private static JLabel mainLabel;
	private static JTextArea textArea;
	
	/**
	 * aggiunge i componenti al pannello
	 */
	public InfoView() {
		
		setLayout(new BorderLayout(30, 30));
		
		mainLabel = new JLabel();
	    
		p = new JPanel();

	    p.add(mainLabel);
	    
	    this.add(p, BorderLayout.NORTH);
	    
	    Font f = new Font("Arial", 28, 15);
	    textArea = new JTextArea();
	    textArea.setColumns(20);
	    textArea.setFont(f);
        textArea.setLineWrap(true);
        textArea.setRows(5);
        textArea.setWrapStyleWord(true);
        textArea.setEditable(false);
	    
	    p2 = new JPanel();
	    JScrollPane scrollPane = new JScrollPane(textArea);
	    scrollPane.setPreferredSize(new Dimension(1000, 600));
	    
	    p2.add(scrollPane);
	    this.add(p2, BorderLayout.CENTER);
	}
	
	/**
	 * metodo statico chiamato per settare il testo della main Label
	 * @param title Titolo del progetto
	 */
	public static void setMainLabel(String title) {
		
		mainLabel.setText(title);
	}
	
	/**
	 * metodo statico chiamato per settare il testo del body
	 * @param bodyText testo di Area Body
	 */
	public static void setTextArea(String bodyText) {
		
		textArea.setText(bodyText);
	}
}
