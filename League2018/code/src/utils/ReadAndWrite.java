package utils;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

import javax.imageio.ImageIO;
import javax.swing.JProgressBar;
import javax.swing.SwingUtilities;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import league.League;
import league.Team;

/**
 * Classe composta da metodi statici per gestire input/output di file
 * @author badjoker
 *
 */
public class ReadAndWrite {
	
	/**
	 * creo dinemicamente un XML da poter modificare senza problemi
	 * 
	 * @param filePath percorso del file XML
	 * @return un documento XML
	 */
	public static Document readXML(String filePath) {
		
		File f = new File(filePath);
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder dBuilder = null;
		Document doc = null;
		try {
			dBuilder = dbFactory.newDocumentBuilder();
			
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			doc = dBuilder.parse(f);
			
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return doc;
	}
	
	/**
	 * salva le immagini nella cartella temp del progetto
	 * 
	 * @param imgPath salva immagine nella cartella temp
	 * @param name nome immagine
	 * @return ritora true se tutto andato bene
	 */
	public static boolean saveIconLocally(String imgPath, String name) {
		
		try {
		    // retrieve image
		    BufferedImage bi = ImageIO.read(new File(imgPath));
		    
		    String outPath = TheBuilder.filesystem + "tempDir/" + name;
		    
		    File outputfile = new File(outPath);
		    
		    // creates output image
	        BufferedImage ri = new BufferedImage(100, 100, bi.getType());
	 
	        // scales the input image to the output image
	        Graphics2D g2d = ri.createGraphics();
	        g2d.drawImage(bi, 0, 0, 100, 100, null);
	        g2d.dispose();
	        
	        bi.flush();
	        
		    ImageIO.write(ri, "png", outputfile);
		    
		    ri.flush();
		    
		} catch (IOException e) {
			
			System.out.println("Cant save it");
		    
		}
		
		return true;
	}
	
	/**
	 * Salva il file XML del progetto nel percorso voluto da utente
	 * 
	 * @param doc XML dinamico
	 * @param path percorso di salvataggio
	 * @return true se il salvaggio avviene
	 */
	public static boolean saveXML(Document doc, String path) {
		
		Transformer transformer = null;
		try {
			transformer = TransformerFactory.newInstance().newTransformer();
		} catch (TransformerConfigurationException | TransformerFactoryConfigurationError e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		Result output = new StreamResult(new File(path));
		Source input = new DOMSource(doc);

		try {
			transformer.transform(input, output);
		} catch (TransformerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}
	
	/**
	 * metodo per gestione di file di TXT
	 * 
	 * @param path percorso 
	 * @param pBar progess bar
	 * @return true se tutto andato bene
	 * @throws IOException riporta errori
	 */
	public static boolean parseTXT(String path, JProgressBar pBar) throws IOException {
		
		FileReader f = new FileReader(path);
		BufferedReader br = new BufferedReader(f);
		String str;
		String arr[];
		Team team;
		
		/*
		 * qua posso sistema il problema del check di team doppi...
		 */
		while((str = br.readLine()) != null) {
			
			final int progress = 10;
			
			try {
		        SwingUtilities.invokeLater(new Runnable() {
		        	
		          public void run() {
		        	  
		        	  pBar.setValue(pBar.getValue() + progress);
		          }
		        });
		        Thread.sleep(20);
		      } catch (InterruptedException e) {
		    }
			
			
			arr = str.split(";");
		    
			team = new Team();
			team.setName(arr[0]);
			team.setTown(arr[1]);
			team.setLogo(arr[2]);
			
			League.addTeam(team);
			
		}
		
		f.close();
		
		return true;
		
	}
	
	/**
	 * esporto il calendario come semplice TXT
	 * @param doc XML dinamico
	 * @param pBar progress bar
	 * @throws IOException errori di output
	 */
	public static void exportToTXT(Document doc, JProgressBar pBar) throws IOException {
		
		String path = TheBuilder.filesystem + TheBuilder.theBuilder.getProjectID() + ".txt";
		
		FileWriter f = new FileWriter(path);
		
		String firstRow = "LEAGUE 2018 " + doc.getElementsByTagName("SPORT").item(0).getAttributes().item(0).getTextContent();
		
		f.write(firstRow + "\n");
		
		NodeList nList = doc.getElementsByTagName("WEEK");
		
		pBar.setMaximum(0);
		pBar.setMaximum(nList.getLength());
		
		String row;
		
		for(int i = 0; i < nList.getLength(); i++) {
			
			
			row = nList.item(i).getAttributes().item(0).getTextContent() + "\t" +
					nList.item(i).getChildNodes().item(0).getTextContent() + "-" +
					nList.item(i).getChildNodes().item(1).getTextContent() + "\t\t\t" +
					nList.item(i).getChildNodes().item(2).getTextContent() + "\n";
			
			f.write(row);
			
			pBar.setValue(i + 1);
		}
		
		f.close();
		
	}
	
	/**
	 * sposto i file dalla cartella Progetto alla temp o viceversa
	 * @param from da dove
	 * @param to a dove
	 * @param toDel se concellare i file subito
	 * @param limiter substringa del percorso
	 */
	public static void copyFromTempToLocalDir(File from, File to, boolean toDel, int limiter) {
		
		
		/*
		 *  tempDir, projectDir, true // prendo il name di tempo dir e salvo come destinazione + name
		 *  
		 *  projectDir, TheBuilder.tempDir, false // 
		 */
		String fixedPath = to.getPath();
		String s;
		for(File file: from.listFiles()) {
			
			if(!file.isDirectory()) {
				
				s = file.toPath().toString();
				s = s.substring(limiter, s.length()); // /name
				s = fixedPath +  "/" + s;
				
				to = new File(s);
				try {
					Files.copy(file.toPath(), to.toPath(), StandardCopyOption.REPLACE_EXISTING);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				if(toDel) file.delete();
			}
		}
		
	}
}

