package league;

/**
 * Classe per informazioni squadra
 * 
 * @author badjoker
 *
 */

public class Team {
	
	private String name;
	private String town;
	private String logo;
	private int points;
	
	
	public Team() {}
	
	public Team(String name, String town) {
		
		this.setName(name);
		this.setTown(town);
		
	}
	/**
	 * 
	 * @param name nome squadra
	 * @param town citta
	 * @param path percorso logo squadra
	 */
	public Team(String name, String town, String path) {
		
		this(name, town);
		this.logo = path;
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @param name the name to set
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * @return the town
	 */
	public String getTown() {
		return town;
	}

	/**
	 * @param town the town to set
	 */
	public void setTown(String town) {
		this.town = town;
	}

	/**
	 * @return the image
	 */
	public String getLogo() {
		return logo;
	}

	/**
	 * 
	 * @param logo percorso logo
	 */
	public void setLogo(String logo) {
		this.logo = logo;
	}

	/**
	 * @return the points
	 */
	public int getPoints() {
		return points;
	}

	/**
	 * @param points the points to set
	 */
	public void setPoints(int points) {
		this.points = points;
	}

}
