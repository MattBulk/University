package league;

import java.util.Vector;


import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import utils.TheBuilder;

/**
 * classe per la realizzazione del Calendario del campionato
 * 
 * @author badjoker
 *
 */

public class Calendar {
	
	private int numDays;
	private int gamesForRound;
	
	private Vector<Team> clonedTeam;
	private Vector<String> couples;
	
	/**
	 * inizializzo il vettore e lo clono per lavorarci in sicurezza
	 * 
	 * @param teamVect passo il vettore delle squadre
	 */
	@SuppressWarnings("unchecked")
	public Calendar(Vector<Team> teamVect) {
		
		clonedTeam = (Vector<Team>) teamVect.clone();
		
		couples = new Vector<String>();
		
		gamesForRound = clonedTeam.size() / 2;
		
		createTournament();
		
		League.getDoc().getElementsByTagName("CALENDAR").item(0).getAttributes().item(0).setTextContent("ON");
		
		removeAllChildren("WEEK");
		
		saveCalendarXML();
		
		clonedTeam.clear();
		couples.clear();
		
	}
	
	/**
	 * cancello il calendatio
	 */
	public static void deleteCalendar() {
		
		removeAllChildren("WEEK");
		League.getDoc().getElementsByTagName("CALENDAR").item(0).getAttributes().item(0).setTextContent("OFF");
		TheBuilder.theBuilder.setActiveCalendar(false);
		TheBuilder.theBuilder.setProjectSTATE("unsaved");
		League.getRankingVector().clear();
		
	}
	
	/**
	 * resetto i risultato del calendario
	 */
	public static void resetCalendarWeeksResults() {
		
		NodeList nList = League.getDoc().getElementsByTagName("WEEK");
		
		for(int i = 0; i < nList.getLength(); i++) {
			
			nList.item(i).getChildNodes().item(2).setTextContent("0-0");
			
		}
		
	}
	
	/**
	 * cancello i figli del tag prescelto
	 * 
	 * @param tag Stringa col nome del tag XML
	 */
	private static void removeAllChildren(String tag) {
		
		while(League.getDoc().getElementsByTagName(tag).getLength() > 0) {
			
			Element e = (Element) League.getDoc().getElementsByTagName(tag).item(0);
			
			e.getParentNode().removeChild(e);
			
		}
		
	}
	
	/**
	 * implementazione del calendario, round robin come algoritmo, ed inserimento in vettore
	 */
	
	private void createTournament() {

	    numDays = (clonedTeam.size() - 1);

	    clonedTeam.remove(0);

	    int teamsSize = clonedTeam.size();

	    for (int day = 0; day < numDays; day++)
	    {

	        int teamIdx = day % teamsSize;
	        
	        if(day % 2 == 0) couples.add(clonedTeam.get(teamIdx).getName() + "-" + League.getTeamVector().get(0).getName());
	        else couples.add(League.getTeamVector().get(0).getName() + "-" + clonedTeam.get(teamIdx).getName());
	        

	        for (int idx = 1; idx < gamesForRound; idx++)
	        {               
	            int firstTeam = (day + idx) % teamsSize;
	            int secondTeam = (day  + teamsSize - idx) % teamsSize;
	          
	            
	            if(day % 2 == 0) couples.add(clonedTeam.get(firstTeam).getName() + "-" + clonedTeam.get(secondTeam).getName());
		        else couples.add(clonedTeam.get(secondTeam).getName() + "-" + clonedTeam.get(firstTeam).getName());
	            
	        }
	    }
	}
	
	/**
	 * ora passo al salvataggio vero e proprio del calendario nel XML
	 */
	private void saveCalendarXML() {
		
		String arr[];
		
		int id = 1;
		int tempID = 0;
		
		for (int i = 0; i < couples.size(); i++) {
			
			if(tempID == gamesForRound) {
				tempID = 0;
				id++;
			}
			
			tempID++;
			
			arr = couples.get(i).split("-");
			
			addWeeklyGame(arr[0], arr[1], "FIRSTROUND", id);
			addWeeklyGame(arr[1], arr[0], "SECONDROUND", id + numDays);
			
		}
			
	}
	
	/**
	 * sotto metodo per aggiungere le partite in andare a ritorno in un colpo solo
	 * @param t1 prime squadra
	 * @param t2 seconda squadra
	 * @param round andata o ritorno
	 * @param weekID numero della giornata
	 */
	private void addWeeklyGame(String t1, String t2, String round, int weekID) {
		
		
		NodeList nList = League.getDoc().getElementsByTagName(round);
		
		Element node =  League.getDoc().createElement("WEEK");
		
		Element team1 =  League.getDoc().createElement("SQUAD");
		team1.setTextContent(t1);
		
		Element team2 =  League.getDoc().createElement("SQUAD");
		team2.setTextContent(t2);
		
		Element result =  League.getDoc().createElement("RESULT");
		result.setTextContent("0-0");
		
		node.appendChild(team1);
		node.appendChild(team2);
		node.appendChild(result);
		
		node.setAttribute("id", Integer.toString(weekID));
		
		nList.item(0).appendChild(node);
		
		League.getDoc().normalize();
		
	}

}
