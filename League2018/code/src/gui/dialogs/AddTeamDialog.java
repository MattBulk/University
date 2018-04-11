package gui.dialogs;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JTextField;

import league.League;
import league.Team;
import utils.ReadAndWrite;
import utils.TheBuilder;

/**
 * menu per creare o caricare le squadre
 * 
 * @author badjoker
 *
 */

@SuppressWarnings("serial")
public class AddTeamDialog extends JDialog implements ActionListener {
	
	private JTextField nF = new JTextField(40);
	private JTextField nT = new JTextField(40);
	private JTextField nL = new JTextField(40);
	private JTextField status = new JTextField(40);
	
	private JProgressBar pBar;
	
	private Team tempTeam;
	
	/**
	 * aggiungo i componenti visibili al pannello
	 */
	public AddTeamDialog() {
		
		setLayout(new GridLayout(2, 1));
		setModal(true);
		setSize(500, 400);    
        setResizable(false); 
        setLocationRelativeTo(null);
        
		setTitle("add team/s to the League");
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		JPanel pan = new JPanel();
		
		this.add(pan);
		
		JLabel n = new JLabel("Nome: ");
		JLabel t = new JLabel("Town: ");
		JLabel l = new JLabel("add Logo: ");
		
		nL.setEditable(false);
		nL.setText("no logo");
		
		JButton btnLogo = new JButton("add logo");
		btnLogo.addActionListener(this);
		JButton submit = new JButton("submit");
		submit.addActionListener(this);
		
		pan.add(n);
		pan.add(nF);
		pan.add(t);
		pan.add(nT);
		pan.add(l);
		pan.add(nL);
		pan.add(btnLogo);
		pan.add(submit);
		
		JPanel pan2 = new JPanel();
		this.add(pan2);
		
		JButton btn = new JButton("add teams from file");
		
		btn.addActionListener(this);
		pan2.add(btn);
		
		JLabel stateLabel = new JLabel("STATE");
		status.setEditable(false);
		
		pBar = new JProgressBar();
		pBar.setStringPainted(true);
		
		pan2.add(status);
		pan2.add(stateLabel);
		pan2.add(pBar);
		
		setVisible(true);
		
	}
	
	/**
	 * metodo ascolto degli eventi con switch dei vari casi
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		switch (e.getActionCommand()) {
		case "add teams from file":
			openFileChooser("loadTeams");
			break;

		case "add logo":
			openFileChooser("findLogo");
			break;
			
		case "submit":
			if(League.checkTeamName(nF.getText())) status.setText("Team Already added, please add another one");
			else if(nF.getText().length() != 0 && nT.getText().length() != 0) sendData();
			else status.setText("Please fill in Name and Town fields");
			break;
		}
		
	}
	
	/**
	 * mando i dati inseriti alla classe league per essere inseriti nel XML e nel vettore dei team
	 */
	private void sendData() {
		// TODO Auto-generated method stub
		tempTeam = new Team();
		tempTeam.setName(nF.getText());
		tempTeam.setTown(nT.getText());
		tempTeam.setLogo(nL.getText());
		League.addTeam(tempTeam);
		
		resetField();
		status.setText("Sended");
		
		TheBuilder.theBuilder.setProjectSTATE("unsaved");
		
	}
	
	/**
	 * resetta campi
	 */
	private void resetField() {
		// TODO Auto-generated method stub
		nF.setText("");
		nT.setText("");
		nL.setText("no logo");
	}
	
	/**
	 * apre un pannello scelta file 
	 * 
	 * @param s Stringa con il nome 
	 */
	private void openFileChooser(String s) {
		// TODO Auto-generated method stub
		
		OpenSaveDialog o = new OpenSaveDialog();
		String str = o.init(s);
		if(str == null) return;
		else if(s == "loadTeams") {
			addFromFile(str);
			
			TheBuilder.theBuilder.setProjectSTATE("unsaved");
		}
		else if(str != null) nL.setText(str);
	}
	
	/**
	 * metodo che passa al metodo statico della classe ReadAndWrite per il parsing di un txt
	 * 
	 * @param srt url del file
	 */
	
	private void addFromFile(String srt) {

		try {
			
			boolean b = ReadAndWrite.parseTXT(srt, pBar);
			
			if(b) status.setText("Teams have been added correctly");
			
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
