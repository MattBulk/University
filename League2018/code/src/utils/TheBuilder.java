package utils;

import java.io.File;
import java.util.Date;

import gui.InfoView;
import gui.TheGui;
import league.Calendar;
import league.League;

public class TheBuilder {
	
	public static TheBuilder theBuilder;
	
	@SuppressWarnings("unused")
	private TheGui theF;
	private League theL;
	
	private boolean activeProject;
	private boolean activeCalendar;
	
	private String projectID;
	private String projectMOD;
	private String projectSTATE;
	
	private File projectDir;
	
	public static final File tempDir = new File("src/storage/tempDir/"); 
	public static final String filesystem = "src/storage/";
	
	private Date date;
	
	public TheBuilder() { init(); }
	
	public TheBuilder(String s) { init(); }
	
	/**
	 * init della GUI
	 */
	private void init() {
		
		theBuilder = this;
		theF = new TheGui();
		
	}
	
	/**
	 * 
	 * @param s Stringa con lo sport
	 * @param type caso new o load
	 */
	public void initTheL(String s, String type) {
		
		projectMOD = type;
		projectSTATE = "saved";
		
		if(type == "new") {
			
			createProjectID(s);
			theL = new League(s);
		}
			
		else theL = new League(s, type);
		
		InfoView.setMainLabel(TheBuilder.theBuilder.getProjectID());
		InfoView.setTextArea(League.getCurrentSport().rules());
		
		TheGui.theGuiInstance.setLeagueView(true);
	}
	
	/**
	 * crea il nome del progetto
	 * 
	 * @param s nome sport
	 */
	private void createProjectID(String s) {
		// TODO Auto-generated method stub
		
		date = new Date();
		String d = date.toString();
		d = d.replaceAll(":", " ");
		
		projectID = s + " " + d;
	}
	
	/**
	 * crea la cartella quando salvo il progetto per la prima volta
	 */
	public void createProjectFolder() {
		// TODO Auto-generated method stub
		projectDir = new File(TheBuilder.filesystem + TheBuilder.theBuilder.getProjectID());
		
		if(!projectDir.exists()) {
			projectDir.mkdir();
		}
		
		if(projectMOD.equals("open")) for (File f : projectDir.listFiles()) f.delete();
		
		ReadAndWrite.copyFromTempToLocalDir(tempDir, projectDir, false, 20);
	}
	
	/**
	 * crea istanza per la creazione di un calendario
	 */
	public void createCalendar() {
		// TODO Auto-generated method stub
		@SuppressWarnings("unused")
		Calendar c = new Calendar(League.getTeamVector());
		
		setActiveCalendar(true);
		
		projectSTATE = "unsaved";
		
		TheGui.theGuiInstance.createPopUp("League Calendar Created", "calCreated");
		
	}
	
	/**
	 * chiama il metodo per creare la Classifica
	 */
	public void createRanking() {
		
		if(theL.createRanking()) TheGui.theGuiInstance.createPopUp("League Ranking Created", "okRanking");
		else TheGui.theGuiInstance.createPopUp("Something went wrong", "okRanking");
	}
	
	/**
	 * pulisce i file temp
	 */
	public void theDirtyCleaner() {
		
		League.getTeamVector().clear();
		TheGui.theGuiInstance.setLeagueView(false);
		
		for (File f : tempDir.listFiles()) f.delete();
	}
	
	/////////////////////////////////////////////GETTER AND SETTER ////////////////////////////////////////////
	
	/**
	 * @return the activeProject
	 */
	public boolean isActiveProject() {
		return activeProject;
	}

	/**
	 * @param activeProject the activeProject to set
	 */
	public void setActiveProject(boolean activeProject) {
		this.activeProject = activeProject;
	}

	/**
	 * @return the projectID
	 */
	public String getProjectID() {
		return projectID;
	}

	/**
	 * @param projectID the projectID to set
	 */
	public void setProjectID(String projectID) {
		this.projectID = projectID;
	}

	/**
	 * @return the projectMOD
	 */
	public String getProjectMOD() {
		return projectMOD;
	}

	/**
	 * @return the projectSTATE
	 */
	public String getProjectSTATE() {
		return projectSTATE;
	}

	/**
	 * @param projectSTATE the projectSTATE to set
	 */
	public void setProjectSTATE(String projectSTATE) {
		this.projectSTATE = projectSTATE;
	}

	/**
	 * @return the activeCalendar
	 */
	public boolean isActiveCalendar() {
		return activeCalendar;
	}

	/**
	 * @param activeCalendar the activeCalendar to set
	 */
	public void setActiveCalendar(boolean activeCalendar) {
		this.activeCalendar = activeCalendar;
	}

	

}
