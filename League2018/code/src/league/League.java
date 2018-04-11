package league;

import java.io.File;
import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import league.sports.Sport;
import utils.ReadAndWrite;
import utils.TheBuilder;

/**
 * Classe di gestione del campionato
 * 
 * @author badjoker
 *
 */

public class League {
	
	private static Sport theCurrentSport;
	
	private static Document doc;
	
	private static Vector<Team> teamVector = new Vector<Team>();
	private static Vector<Team> rankingVector = new Vector<Team>();
	
	/**
	 * creazione progetto ex-novo
	 * 
	 * @param s tipo di sport
	 */
	public League(String s) {
		
	 	try {
			this.init(s);
			populateNewXML();
			
		} catch (ClassNotFoundException | InstantiationException | IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println(e.getMessage());
		}
		
	}
	
	/**
	 * secondo metodo per apertura di un progetto esistente
	 * 
	 * @param path percorso del file di salvataggio del progetto
	 * @param s tipo di sport
	 */
	public League(String path, String s) {
		
		setDoc(ReadAndWrite.readXML(path));
		
		populateOpenXML();
		
	}
	
	/**
	 * Questo metodo converte una stringa in una classe con vari metodi di casting i generics lo permetto, cosi con una var
	 * posso gestire x tipi di sport estesi dalla classe astratta Sport
	 * @param s
	 * @throws ClassNotFoundException classe non trovata
	 * @throws InstantiationException errore di creazione istanza
	 * @throws IllegalAccessException accesso illegale
	 */
	private void init(String s) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
		
		String sport = "league.sports." + s;
		
		@SuppressWarnings("unchecked")
		Class<? extends Sport> act = (Class<? extends Sport>) Class.forName(sport);
		
		theCurrentSport = act.newInstance();
		
		TheBuilder.theBuilder.setActiveProject(true);
		
		
		rankingVector.clear();
		
	}
	
	/**
	 * popolo XML doc da un XML caricato da File cosi da lavorarci freely
	 */
	
	private void populateOpenXML() {
		
		NodeList nList = getDoc().getElementsByTagName("TEAM");
		
		for(int i = 0; i < nList.getLength(); i++) {
			
			Team t = new Team();
			t.setName(nList.item(i).getChildNodes().item(0).getTextContent());
			t.setTown(nList.item(i).getChildNodes().item(1).getTextContent());
			t.setLogo(nList.item(i).getChildNodes().item(2).getTextContent());
			
			teamVector.addElement(t);
		}
		
		if(League.getDoc().getElementsByTagName("CALENDAR").item(0).getAttributes().item(0).getTextContent().equals("ON")) {
			
			TheBuilder.theBuilder.setActiveCalendar(true);
		}
		
		TheBuilder.theBuilder.setProjectID(getDoc().getElementsByTagName("SPORT").item(0).getAttributes().item(0).getTextContent());
		
		String s = TheBuilder.filesystem + TheBuilder.theBuilder.getProjectID();
		File projectDir = new File(s);
		
		ReadAndWrite.copyFromTempToLocalDir(projectDir, TheBuilder.tempDir, false, s.length()+1);
		
		String sport = getDoc().getElementsByTagName("SPORT").item(0).getTextContent();
		
		try {
			init(sport);
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InstantiationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	/**
	 * popolo XML per la prima volta nel caso di progetto nuovo
	 */
	private void populateNewXML() {
		
		setDoc(ReadAndWrite.readXML("src/storage/" + "new.xml"));
		
		getDoc().getElementsByTagName("SPORT").item(0).setTextContent(theCurrentSport.name());
		getDoc().getElementsByTagName("SPORT").item(0).getAttributes().item(0).setTextContent(TheBuilder.theBuilder.getProjectID());
		getDoc().getElementsByTagName("TEAM_NUM").item(0).setTextContent(Integer.toString(theCurrentSport.teamNumber()));
		
	}
	
	/**
	 * salvo i team in XML e in vettore
	 * 
	 * @param theTeam passo Team con i valori richiesto dal progetto
	 */
	public static void addTeam(Team theTeam) {
		
		if(!theTeam.getLogo().equals("no logo")) handleLogoImages(theTeam);
		
		teamVector.addElement(theTeam);
		
		NodeList nList = getDoc().getElementsByTagName("TEAMS");
		
		Element node = getDoc().createElement("TEAM");
		
		Element name = getDoc().createElement("NAME");
		name.setTextContent(theTeam.getName());
		
		Element town = getDoc().createElement("TOWN");
		town.setTextContent(theTeam.getTown());
		
		Element logo = getDoc().createElement("LOGO");
		logo.setTextContent(theTeam.getLogo());
		
		node.appendChild(name);
		node.appendChild(town);
		node.appendChild(logo);
		
		nList.item(0).appendChild(node);
		
		getDoc().normalize();
		
	}
	
	/**
	 * cancello il team
	 * 
	 * @param index indice vettore
	 */
	public static void removeTeam(int index) {
		
		Element e = (Element) getDoc().getElementsByTagName("TEAM").item(index);
		
		e.getParentNode().removeChild(e);
		
		getDoc().normalize();
		
		if(!teamVector.get(index).getLogo().equals("no logo")) {
			
			deleteLogofromHDD(index);
		}

		teamVector.removeElementAt(index);
	}
	
	/**
	 * modifico il team
	 * @param theTeam var team con modifiche
	 * @param index quale variabile
	 * @return boolean true se tutto e' andato bene
	 */
	public static boolean modifyTeam(Team theTeam, int index) {
		
		handleLogoImages(theTeam, index);
		
		teamVector.elementAt(index).setName(theTeam.getName());
		teamVector.elementAt(index).setTown(theTeam.getTown());
		teamVector.elementAt(index).setLogo(theTeam.getLogo());
		
		NodeList nList = getDoc().getElementsByTagName("TEAM");
		
		nList.item(index).getChildNodes().item(0).setTextContent(theTeam.getName());
		nList.item(index).getChildNodes().item(1).setTextContent(theTeam.getTown());
		nList.item(index).getChildNodes().item(2).setTextContent(theTeam.getLogo());
		
		getDoc().normalize();
		
		return true;
	}
	
	/**
	 * gestione del logo della squadra
	 * 
	 * @param theTeam
	 */
	private static void handleLogoImages(Team theTeam) {
		
		ReadAndWrite.saveIconLocally(theTeam.getLogo(), theTeam.getName());
		theTeam.setLogo(TheBuilder.filesystem + TheBuilder.theBuilder.getProjectID() + "/" + theTeam.getName());
	}
	
	/**
	 * seconda versione gestione logo in caso di modifica
	 * 
	 * @param theTeam
	 * @param index
	 */
	private static void handleLogoImages(Team theTeam, int index) {
		
		if(!teamVector.elementAt(index).getLogo().equals("no logo") && theTeam.getLogo().equals("no logo")) {
			
			deleteLogofromHDD(index);
			
		}
		
		//teamVector.elementAt(index).getLogo().equals("no logo") && 
		
		else if(!theTeam.getLogo().equals("no logo")) {
			
			ReadAndWrite.saveIconLocally(theTeam.getLogo(), theTeam.getName());
			theTeam.setLogo(TheBuilder.filesystem + TheBuilder.theBuilder.getProjectID() + "/" + theTeam.getName());
		}
		
	}
	
	/**
	 * cancella il logo
	 * @param index
	 */
	private static void deleteLogofromHDD(int index) {
		
		String str = TheBuilder.tempDir + "/" + teamVector.get(index).getName();
		File logo = new File(str);
		
		logo.delete();
	}
	
	/**
	 * check del nome doppio
	 * 
	 * @param s Stringa con il nome squadra
	 * @return boolean di check
	 */
	public static boolean checkTeamName(String s) {
		
		for (int i = 0; i < League.getTeamVector().size(); i++) {
			
			String t = League.getTeamVector().elementAt(i).getName().toLowerCase();
			
			if(t.equals(s.toLowerCase())) return true;
		}
		return false;
	}
	
	/**
	 * metodo per la creazione della classifica
	 * @return true se tutto andato bene
	 */
	public boolean createRanking() {
		
		@SuppressWarnings("unchecked")
		Vector<Team> tempTeams = (Vector<Team>) teamVector.clone();
		
		NodeList nList = League.getDoc().getElementsByTagName("WEEK");
		
		for (int i = 0; i < tempTeams.size(); i++) {
			
			int points = 0;
			
			// adding this code produces the Partial Rarking, need a better dev to provide it, I have to study Physics !
			
			//if(end <= 0) end = nList.getLength();
			//else end = (tempTeams.size()-1) * (tempTeams.size()/2);
			
			String t1, t2, sc;
			for(int j = 0; j < nList.getLength(); j++) {
				
				t1 = nList.item(j).getChildNodes().item(0).getTextContent();
				t2 = nList.item(j).getChildNodes().item(1).getTextContent();
				sc = nList.item(j).getChildNodes().item(2).getTextContent();
				
				if(tempTeams.get(i).getName().equals(t1)) points += theCurrentSport.pointForGame(0, sc);
				else if(tempTeams.get(i).getName().equals(t2)) points += theCurrentSport.pointForGame(1, sc);
				
			}
			
			tempTeams.get(i).setPoints(points);
		}
		
		rankingVector.clear();
		
		orderLeagueRanking(tempTeams);
		
		tempTeams.clear();
		
		return true;
	}
	
	/**
	 * ordinamento della classifica con un algoritmo di ricerca massimo
	 * @param unordered
	 */
	private void orderLeagueRanking(Vector<Team> unordered) {
		
		
		while(unordered.size() > 1) {
			
			Team max = unordered.get(0);
			int index = 0;
			for (int i = 1; i < unordered.size(); i++) {
				
				if(unordered.get(i).getPoints() > max.getPoints()) {
					max = unordered.get(i);
				    index = i;
				}
			}
			
			rankingVector.add(unordered.get(index));
			unordered.remove(index);
			
		}
		
		rankingVector.add(unordered.get(0));
		unordered.remove(0);
		
	}
	
	/*
	private int checkGameResult(int t, String score) {
		
		String arr[];
		
		arr = score.split("-");
		
		int h = Integer.parseInt(arr[0]);
		int v = Integer.parseInt(arr[1]);
		
		if(h == v) return 1;
		else if(h > v && t == 0) return theCurrentSport.pointForGame();
		else if(v > h && t == 1) return theCurrentSport.pointForGame();
		return 0;
		
		
	}
	*/
	
	////////////////////////////////////////// GETTER AND SETTER //////////////////////////////////////////

	/**
	 * @return the teamVector
	 */
	public static Vector<Team> getTeamVector() {
		return League.teamVector;
	}

	/**
	 * @param teamVector the teamVector to set
	 */
	public static void setTeamVector(Vector<Team> teamVector) {
		League.teamVector = teamVector;
	}
	
	/**
	 * @return the rankingVector
	 */
	public static Vector<Team> getRankingVector() {
		return League.rankingVector;
	}

	/**
	 * @param rankingVector the rankingVector to set
	 */
	public static void setRankingVector(Vector<Team> rankingVector) {
		League.rankingVector = rankingVector;
	}

	/**
	 * @return the doc
	 */
	public static Document getDoc() {
		return doc;
	}

	/**
	 * @param doc the doc to set
	 */
	public static void setDoc(Document doc) {
		League.doc = doc;
	}
	
	/**
	 * 
	 * @return variabile dello sport corrente
	 */
	public static Sport getCurrentSport() {
		
		return theCurrentSport;
		
	}
	
}
