package gui.dialogs;


import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JPanel;

import utils.TheBuilder;

/**
 * Pannello iniziale per scegliere lo sport preferito
 * 
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class SportDialog extends JDialog implements ActionListener {
	
	
	private JComboBox<String> list;
	private String[] items = {"Soccer", "Basket", "VolleyBall"};
	private JButton btn;
	
	/**
	 * aggiungo i componenti
	 */
	public SportDialog() {
		
		JPanel p = new JPanel();
		
		this.add(p);
		
		//JDialog Stuff
		setModal(true);
		setSize(250, 100);
		setLocationRelativeTo(null);
        setResizable(false);  
		setTitle("Choose the League");
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		list = new JComboBox<>(items);
		
		p.add(list);
		
		btn = new JButton("ok");
		btn.addActionListener(this);
		
		p.add(btn);
		
		setVisible(true);

	}
	
	/**
	 * inizializzo la League spedendo la stringa al metodo
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		String str = (String) list.getSelectedItem();
		TheBuilder.theBuilder.initTheL(str, "new");
		dispose();
	}


}
