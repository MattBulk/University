package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import league.Calendar;
import league.League;
import utils.TheBuilder;

/**
 * questa classe ascolta gli eventi che arrivato dal menu e gestisce la comunicazione tra UI, Calendar, League e Builder
 * 
 * @author badjoker
 *
 */
public class EveDropper implements ActionListener {
	
	private String eventName;
	
	public EveDropper() {}
	/**
	 * gestione dei bottoni, un grande switch 
	 * 
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		
		eventName = e.getActionCommand();
		
		switch(eventName) {
		
		case "new":
			TheGui.theGuiInstance.createSportDialog();
			if(TheBuilder.theBuilder.isActiveProject()) enable();
			break;
			
		case "open":
			TheGui.theGuiInstance.createFileChooser(eventName);
			if(TheBuilder.theBuilder.isActiveProject()) enable();
			break;
			
		case "save":
			TheGui.theGuiInstance.createFileChooser(eventName);
			break;
			
		case "close":
			if(TheBuilder.theBuilder.getProjectSTATE() == "saved") closeForReal();
			else TheGui.theGuiInstance.createPopUp("close without saving?", "close");
			break;
			
		case "add":
			if(TheBuilder.theBuilder.isActiveCalendar()) TheGui.theGuiInstance.createPopUp("You can't add team on Active Calendar", "add");
			else TheGui.theGuiInstance.createTeamDialog(eventName);
			break;
		case "modify":
			TheGui.theGuiInstance.createTeamDialog(eventName);
			break;
			
		case "create Calendar":
			if(League.getTeamVector().size() < 2) TheGui.theGuiInstance.createPopUp("Please add at least 2 four teams", "teams");
			else if(TheBuilder.theBuilder.isActiveCalendar()) TheGui.theGuiInstance.createPopUp("Override existing Calendar?", "cal");
			else TheBuilder.theBuilder.createCalendar();
			break;
			
		case "delete Calendar":
			if(!TheBuilder.theBuilder.isActiveCalendar()) TheGui.theGuiInstance.createPopUp("Where is no Calendar", "noCal");
			else {
				Calendar.deleteCalendar();
				TheGui.theGuiInstance.createPopUp("Calendar deleted", "delCal");
			}
			break;
		
		case "create ranking":
			if(!TheBuilder.theBuilder.isActiveCalendar()) TheGui.theGuiInstance.createPopUp("No Calendar, No Party", "noRan");
			else TheBuilder.theBuilder.createRanking();
			break;
		}
		
	}
	
	/**
	 * chiude il progetto e resetta le variabili ed i menu
	 */
	
	public static void closeForReal() {
		
		TheGui.theGuiInstance.setEnableItemsInMenu(false, 0, "close");
		TheGui.theGuiInstance.setEnableItemsInMenu(true, 0, "open");
		TheGui.theGuiInstance.setEnableItemsInMenu(true, 0, "new");
		TheGui.theGuiInstance.setEnableItemsInMenu(false, 0, "save");
		
		TheGui.theGuiInstance.setEnableMenu(false, 1);
		TheGui.theGuiInstance.setEnableMenu(false, 2);
		
		TheBuilder.theBuilder.theDirtyCleaner();
		TheBuilder.theBuilder.setActiveProject(false);
		TheBuilder.theBuilder.setProjectSTATE("saved");
		TheBuilder.theBuilder.setActiveCalendar(false);
		
	}
	
	/**
	 * attiva i menu dopo la creazione di un progetto
	 */
	private static void enable() {
		
		TheGui.theGuiInstance.setEnableItemsInMenu(false, 0, "new");
		TheGui.theGuiInstance.setEnableItemsInMenu(false, 0, "open");
		TheGui.theGuiInstance.setEnableItemsInMenu(true, 0, "close");
		TheGui.theGuiInstance.setEnableItemsInMenu(true, 0, "save");
		
		TheGui.theGuiInstance.setEnableMenu(true, 1);
		TheGui.theGuiInstance.setEnableMenu(true, 2);
		
	}

}
