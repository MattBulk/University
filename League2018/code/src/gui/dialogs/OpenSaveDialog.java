package gui.dialogs;

import javax.swing.JFileChooser;
import league.League;
import utils.ReadAndWrite;
import utils.TheBuilder;

/**
 * estensione del JFileChooser
 * @author badjoker
 *
 */
@SuppressWarnings("serial")
public class OpenSaveDialog extends JFileChooser {
	
	private int userSelection;
	private String str;
	
	/**
	 * metodo di creazione del instanza
	 */
	public OpenSaveDialog() {
		

	}
	
	/**
	 * lancio vero e proprio del metodo con tipo di stringa
	 * 
	 * @param type stringa con il tipo
	 * @return un valore stringa con il tipo il messaggio o URL assegnato
	 */
	public String init(String type) {
		
		if(type.equals("save")) userSelection = this.showSaveDialog(this.getParent());
		else userSelection = this.showOpenDialog(this.getParent());
		 
		if (userSelection == JFileChooser.APPROVE_OPTION) {
			
			switch (type) {
			case "save":
				TheBuilder.theBuilder.createProjectFolder();
				ReadAndWrite.saveXML(League.getDoc(), this.getSelectedFile().getAbsolutePath());
				TheBuilder.theBuilder.setProjectSTATE("saved");
				break;
			case "loadTeams":
				str = this.getSelectedFile().getAbsolutePath();
				break;
			case "findLogo":
				str = this.getSelectedFile().getAbsolutePath();
				break;
			default:
				TheBuilder.theBuilder.initTheL(this.getSelectedFile().getAbsolutePath(), type);
				break;
			}
		    
		}
		else str = null;
	
		return str;
		
	}

}
