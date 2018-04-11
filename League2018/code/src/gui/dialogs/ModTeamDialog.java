package gui.dialogs;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import gui.TheGui;
import league.League;
import league.Team;
import utils.TheBuilder;

/**
 * pannello per rimuovere e modificare team
 * 
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class ModTeamDialog extends JDialog implements ActionListener {

	private JTextField nF = new JTextField(40);
	private JTextField nT = new JTextField(40);
	private JTextField nL = new JTextField(40);
	private JTextField status = new JTextField(40);
	
	private JButton logoBtn = new JButton("add logo");
	private JButton rmLogoBtn = new JButton("remove logo");
	private JButton submitBtn = new JButton("submit"); 
	
	private JComboBox<String> list;
	
	private Team tempTeam;
	
	private int currentItem;
	
	/**
	 * add dei componenti del pannello
	 */
	
	public ModTeamDialog() {
		// TODO Auto-generated constructor stub
		setLayout(new GridLayout(2, 1));
		setModal(true);
		setSize(500, 450);
		setLocationRelativeTo(null);
        setResizable(false);  
		setTitle("modify or delete team/s from the League");
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		
		JPanel pan = new JPanel();
		this.add(pan);
		
		JLabel listLabel = new JLabel("Choose the team to modify or delete:");
		pan.add(listLabel);
		
		list = new JComboBox<>();
		
		addItemsToComboList();
		
		pan.add(list);
		
		JButton modBtn = new JButton("mod");
		modBtn.addActionListener(this);
		pan.add(modBtn);
		
		//list.addItemListener(aListener);
		
		JLabel n = new JLabel("Nome: ");
		JLabel t = new JLabel("Town: ");
		JLabel l = new JLabel("add Logo: ");
		
		nL.setEditable(false);
		nL.setText("no logo");
		
		logoBtn.addActionListener(this);
		logoBtn.setEnabled(false);
		submitBtn.addActionListener(this);
		submitBtn.setEnabled(false);
		rmLogoBtn.addActionListener(this);
		rmLogoBtn.setEnabled(false);
		
		nF.setEditable(false);
		
		pan.add(n);
		pan.add(nF);
		pan.add(t);
		pan.add(nT);
		pan.add(l);
		pan.add(nL);
		pan.add(logoBtn);
		pan.add(rmLogoBtn);
		pan.add(submitBtn);
		
		JPanel pan2 = new JPanel();
		this.add(pan2);
		
		JButton deleteTeam = new JButton("Delete current Selected Team");
		deleteTeam.addActionListener(this);
		pan2.add(deleteTeam);
		
		JLabel stateLabel = new JLabel("STATE");
		
		pan2.add(status);
		pan2.add(stateLabel);
		
		setVisible(true);
	}
	
	/**
	 * gestione degli eventi
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		
		switch (e.getActionCommand()) {
		case "mod":
			if(League.getTeamVector().size() != 0) setField();
			break;
		case "add logo":
			openFileChooser("findLogo");
			break;
		case "remove logo":
			nL.setText("no logo");
			break;
		case "submit":
			if(nF.getText().length() != 0 && nT.getText().length() != 0) sendMod();
			else status.setText("Please fill in Name and Town fields");	
			break;
		default:
			if(League.getTeamVector().size() != 0 && !TheBuilder.theBuilder.isActiveCalendar()) deleteItem();
			else TheGui.theGuiInstance.createPopUp("You can't remove team on Active Calendar", "mod");
			break;
		}
		
	}
	
	/**
	 * cancello una squadra
	 */
	private void deleteItem() {
		// TODO Auto-generated method stub
		currentItem = list.getSelectedIndex();
		League.removeTeam(currentItem);
		status.setText("! ! ! Item deleted ! ! !");
		addItemsToComboList();
		
		TheBuilder.theBuilder.setProjectSTATE("unsaved");
	}
	/**
	 * spedisce la modifica
	 */
	private void sendMod() {
		
		tempTeam = new Team();
		tempTeam.setName(nF.getText());
		tempTeam.setTown(nT.getText());
		tempTeam.setLogo(nL.getText());
		
		if(League.modifyTeam(tempTeam, currentItem)) addItemsToComboList();
		
		logoBtn.setEnabled(false);
		rmLogoBtn.setEnabled(false);
		submitBtn.setEnabled(false);
		
		TheBuilder.theBuilder.setProjectSTATE("unsaved");
		
	}
	/**
	 * setto i campi
	 */
	private void setField() {
		
		currentItem = list.getSelectedIndex();
		nF.setText(League.getTeamVector().get(currentItem).getName());
		nT.setText(League.getTeamVector().get(currentItem).getTown());
		nL.setText(League.getTeamVector().get(currentItem).getLogo());
		
		logoBtn.setEnabled(true);
		rmLogoBtn.setEnabled(true);
		submitBtn.setEnabled(true);
		
	}
	/**
	 * apertura dialog
	 * 
	 * @param s String del tipo di url
	 */
	private void openFileChooser(String s) {
		// TODO Auto-generated method stub
		
		OpenSaveDialog o = new OpenSaveDialog();
		String str = o.init(s);
		if(str != null) nL.setText(str);
		
	}
	
	/**
	 * update e prima aggiunta degli elemento alla JcomboList
	 */
	private void addItemsToComboList() {
		
		list.removeAllItems();
		
		for (int i = 0; i < League.getTeamVector().size(); i++) {
			
			list.addItem(League.getTeamVector().get(i).getName());
		}
		
		list.updateUI();
	}
	
}
