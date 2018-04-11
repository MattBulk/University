package utils;

import javax.swing.*;

import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;

/**
 * esempio di classe con uso dei generics per gestione dei textField filtro. 
 * Usando i generics wildcard posso fare una sorta di casting
 * 
 * @author badjoker
 *
 */

public class RowFilterUtils {
	
    public static JTextField createRowFilter(JTable table) {
        
    	RowSorter<? extends TableModel> rs = table.getRowSorter();
        if (rs == null) {
            
        	table.setAutoCreateRowSorter(true);
            rs = table.getRowSorter();
            
        }

        TableRowSorter<? extends TableModel> rowSorter = (rs instanceof TableRowSorter) ? (TableRowSorter<? extends TableModel>) rs : null;

        if (rowSorter == null) {
            throw new RuntimeException("Cannot find appropriate rowSorter: " + rs);
        }

        final JTextField tf = new JTextField(15);
        tf.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                try {
                	update(e);
				} catch (Exception e2) {
					// TODO: handle exception
					System.out.println(e);
					
				}
            	
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
            	try {
                	update(e);
				} catch (Exception e2) {
					// TODO: handle exception
					System.out.println(e);
					
				}
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
            	try {
                	update(e);
				} catch (Exception e2) {
					// TODO: handle exception
					System.out.println(e);
					
				}
            }

            private void update(DocumentEvent e) {
                String text = tf.getText();
                
                if (text.trim().length() == 0) {
                    rowSorter.setRowFilter(null);
                } else {
                	
                    rowSorter.setRowFilter(RowFilter.regexFilter("(?i)" + text));
                }
            }
        });

        return tf;
    }
}
