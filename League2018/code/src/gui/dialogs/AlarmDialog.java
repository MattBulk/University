package gui.dialogs;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;


import gui.EveDropper;
import utils.TheBuilder;

/**
 * implemazione di un sistema di Alarm a messaggio
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class AlarmDialog extends JDialog implements ActionListener {
	
private String type;

/**
 * 
 * @param alarm Stringa con il messaggio
 * @param type tipo di allarme
 */
	
public AlarmDialog(String alarm, String type) {
		
		JPanel p = new JPanel();
		JLabel l = new JLabel(alarm);
		this.add(p);
		p.add(l);
		
		this.type = type;
		
		//JDialog Stuff
		setModal(true);
		setSize(350, 150);
		setLocationRelativeTo(null);
        setResizable(false);  
		setTitle("ALARM");
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		JButton btn = new JButton("ok");
		btn.addActionListener(this);
		
		p.add(btn);
		
		setVisible(true);

	}

	/**
	 * gestione degli eventi in base alla variabile type
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		switch (type) {
		case "cal":
			TheBuilder.theBuilder.createCalendar();
			break;
		case "delCal":
			
			break;
		case "close":
			EveDropper.closeForReal();
			break;
		}
		
		dispose();
	}
}
