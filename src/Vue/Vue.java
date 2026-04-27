package Vue;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import Modele.Jeu;

public class Vue extends JComponent{
    Jeu jeu;
    JFrame frame;
    private Timer timer;
    int marge;
    int width;
    int height;
    int largeurCarre;
    int hauteurCarre;
    int curseurLigne;
    int curseurColonne;

    public Vue(Jeu jeu, JFrame frame) {
        this.jeu = jeu;
        this.frame = frame;

        curseurLigne = 0;
        curseurColonne = 0;

        timer = new Timer(16, new ActionListener() {
           @Override
            public void actionPerformed(ActionEvent e) {
                repaint();
            } 
        });
        timer = timer;
    }

    public void click(int x, int y) {
        int ligne = obtiensLigne(y);
        int colonne = obtiensColonne(x);
        if (ligne >= 0 && colonne >= 0 && jeu.clickable(ligne, colonne)) {
            jeu.click(ligne, colonne);
            updateLabel();
        }

        repaint();
    }

    public void curseurCoordonnees(int x, int y) {
        int ligne = obtiensLigne(y) ;
        int colonne = obtiensColonne(x);
        if (ligne != -1 && colonne != -1) {
            curseurLigne = ligne;
            curseurColonne = colonne;
        }

        repaint();
    }

    private void updateLabel(){
        
    }

    public int obtiensLigne(int y){
        for (int ligne = 0; ligne < jeu.lignes(); ligne++){
            if (y >= (ligne * (marge + hauteurCarre)) && (y <= (ligne * (marge + hauteurCarre) + hauteurCarre))){
                return ligne;
            }
        }
        return -1;
    }

    public int obtiensColonne(int x) {
        for (int colonne = 0; colonne < jeu.colonnes(); colonne++) {
            if (x >= (colonne * (marge + largeurCarre)) && (x <= (colonne * (marge + largeurCarre) + largeurCarre))) {
                return colonne;
            }
        }
        return -1;
    }

    public void keyPressed(int keyCode) {
        if (keyCode == KeyEvent.VK_EQUALS) {
            jeu.init(jeu.lignes() + 1, jeu.colonnes() + 1);
            repaint();
        } else if (keyCode == KeyEvent.VK_MINUS) {
            jeu.init(jeu.lignes() - 1, jeu.colonnes() - 1);
            repaint();
        }
    }
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        width = getWidth();
        height = getHeight();
        marge = (int) (0.01 * Math.min(width, height));

        largeurCarre = (int) (width * (1 - 0.01 * jeu.colonnes()) / jeu.colonnes());
        hauteurCarre = (int) (height * (1 - 0.01 * jeu.lignes()) / jeu.lignes());

        g.clearRect(0, 0, width, height);

        // Dessin gaufre
        for (int ligne = 0; ligne < jeu.lignes(); ligne++) {
            for (int colonne = 0; colonne < jeu.colonnes(); colonne++) {
                int x = (int) colonne * (marge + largeurCarre);
                int y = (int) ligne * (marge + hauteurCarre);
                
                if (jeu.plateau()[ligne][colonne] == 1) {
                    if (ligne == curseurLigne && colonne == curseurColonne){
                        g.setColor(Color.RED);
                    } else if (ligne >= curseurLigne && colonne >= curseurColonne) {
                        g.setColor(Color.BLUE);
                    } else {
                        g.setColor(Color.ORANGE);
                    }
                } else {
                    g.setColor(Color.WHITE);
                }
                g.fillRect(x, y, largeurCarre, hauteurCarre);
                g.setColor(Color.BLACK);
                g.drawRect(x, y, largeurCarre, hauteurCarre);
            }
        }

        // Dessin case empoisonnée
        g.setColor(Color.GREEN);
        g.fillOval(largeurCarre / 4, hauteurCarre / 4, largeurCarre / 2, hauteurCarre / 2);
    }
}
