package league.sports;

/**
 * classe astratta con i metodi
 * 
 * @author badjoker
 *
 */

public abstract class Sport {
	
	public Sport() {}
	
	/**
	 * 
	 * @return Stringa con il nome dello sport
	 */
	public abstract String name();
	
	/**
	 * 
	 * @return int con il numero dei componenti della squadra
	 */
	public abstract int teamNumber();
	
	/**
	 * 
	 * @return Stringa delle regole
	 */
	public abstract String rules();
	
	/**
	 *  crea le statistica future della squadra
	 */
	public abstract void statistics();
	
	/**
	 * 
	 * @param t se il tipo gioca in casa o e' visitor
	 * @param score risultato della partita
	 * @return int con i punti da assegnare
	 */
	public abstract int pointForGame(int t, String score);
	
	/**
	 * 
	 * @return ritorna la stringa del regex per costringere utente ad inserire valori corretti
	 */
	public abstract String regEx();

}
