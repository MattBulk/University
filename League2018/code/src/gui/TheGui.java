package gui;


import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JPanel;

import gui.dialogs.OpenSaveDialog;
import gui.dialogs.SportDialog;
import gui.dialogs.AddTeamDialog;
import gui.dialogs.AlarmDialog;
import gui.dialogs.ModTeamDialog;

/**
 * Classe generatrice di tutti i componenti necessari per la GUI, menu, JFrame, le LeagueView
 * 
 * @author badjoker
 *
 */
public class TheGui {
	
	
	private JFrame mainFrame;
	private TheMainBar mainBar;
	private JPanel background;
	private LeagueView leagueView;
	
	public static TheGui theGuiInstance;
	
	public static final EveDropper eveDropper = new EveDropper();
	
	/**
	 * creo una variabile statica alla classe stessa, non morira' fino alla fine del programma
	 * poi lancio l'init vero e proprio
	 */
	public TheGui() {
		
		theGuiInstance = this;
		
		initFrame();
	}
	
	/**
	 * metodo che aggiunge i componenti principali di Swing
	 */
	private void initFrame() {
		
		mainFrame = new JFrame("Campionato 2017/2018");
		
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		mainFrame.setPreferredSize(new Dimension(1024, 768));
		
		mainFrame.setResizable(false);
		mainFrame.setVisible(true);
		
		mainBar = new TheMainBar();
		mainFrame.setJMenuBar(mainBar);
		
		background = new JPanel();
		background.setBackground(Color.DARK_GRAY);
		
		mainFrame.getContentPane().add(background, BorderLayout.CENTER);
		
		createLeagueView();
		
		mainFrame.pack();
		
	} 
	
	/**
	 * stabilisce quali menuItem attivare
	 * 
	 * @param b setta componente enable o no
	 * @param n quale menu
	 * @param name String con il nome del menuItem
	 */
	protected void setEnableItemsInMenu(boolean b, int n, String name) {
		
		mainBar.setEnableByName(b, n, name);
		
	}
	
	/**
	 * stabilisce quali menu attivare
	 * 
	 * @param b boolean per enable
	 * @param nMenu numero del menu
	 */
	protected void setEnableMenu(boolean b, int nMenu) {
		
		mainBar.setEnableByNumber(b, nMenu);
	}
	
	/**
	 * crea il pannello per la gestione dello sport
	 */
	protected void createSportDialog() {
		
		@SuppressWarnings("unused")
		SportDialog d = new SportDialog();
		
	}
	
	/**
	 * classico select File
	 * 
	 * @param s passa la stringa al Save Dialog
	 */
	
	protected void createFileChooser(String s) {
		
		OpenSaveDialog o = new OpenSaveDialog();
		o.init(s);
		
	}
	
	/**
	 * creo unica istanza per la visualizzazione del Campionato, soluzione opinabile ma almeno in questo modo
	 * la memoria e' praticamente sempre la stessa utilizzata, ora come ora potrebbe risultare antiquata come
	 * soluzione.... 
	 * 
	 */
	
	private void createLeagueView() {
		
		leagueView = new LeagueView();
		leagueView.setBackground(Color.DARK_GRAY);
		leagueView.setVisible(false);
		
		mainFrame.getContentPane().add(leagueView, BorderLayout.NORTH);
		
		mainFrame.pack();
		
	}
	
	/**
	 * Essendo unica League in gioco invece di distruggerla preferisco nasconderla e riattivarla quando necessario
	 * @param v visible not visible
	 */
	public void setLeagueView(boolean v) {
		
		leagueView.setVisible(v);
		
	}
	
	/**
	 * crea un mio popup per gli allarmi ... ops JOptionPanel already existed ...
	 * 
	 * @param alarm Stringa di messaggio notifica
	 * @param type tipo di messaggio
	 */
	public void createPopUp(String alarm, String type) {
		
		@SuppressWarnings("unused")
		AlarmDialog popUp = new AlarmDialog(alarm, type);

	}
	
	/**
	 * metodo per la creazione di un pannello del menu per aggiungere o modificare una squadre
	 * @param type tipo del pannello da richiamare
	 */

	protected void createTeamDialog(String type) {
		// TODO Auto-generated method stub
		
		if(type.equals("add")) {
			@SuppressWarnings("unused")
			AddTeamDialog t = new AddTeamDialog();
		}
		else {
			@SuppressWarnings("unused")
			ModTeamDialog m = new ModTeamDialog();
		}
		
	}
 
}
