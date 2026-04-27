import javax.swing.*;

import java.awt.BorderLayout;
import java.awt.event.*;

import Modele.Jeu;
import Vue.Vue;

public class Gaufre {
    public static void main(String[] args) {        
        JFrame frame = new JFrame("Gaufre Empoisonnée");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(1000, 800);

        Jeu jeu = new Jeu(10, 10);
        Vue vue = new Vue(jeu, frame);

        frame.add(vue);
        
        JButton nouvellePartie = new JButton("Nouvelle partie");
        JButton annuler = new JButton("Annuler");
        JButton refaire = new JButton("Refaire");

        Box barre = Box.createHorizontalBox();
        barre.add(nouvellePartie);
        barre.add(Box.createGlue());
        Box boutonsDroite = Box.createVerticalBox();
        boutonsDroite.add(annuler);
        boutonsDroite.add(refaire);
        barre.add(boutonsDroite);

        nouvellePartie.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                jeu.init(jeu.lignes(), jeu.colonnes());
                vue.repaint();
            }
        });

        annuler.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // annuler
            }
        });

        refaire.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // refaire
            }
        });

        frame.add(vue);
        frame.add(barre, BorderLayout.SOUTH);

        vue.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                vue.click(e.getX(), e.getY());
                vue.requestFocusInWindow();
            }
        });

        vue.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseMoved(MouseEvent e) {
                vue.curseurCoordonnees(e.getX(), e.getY());
            }
        });

        vue.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                vue.keyPressed(e.getKeyCode());
            }
        });

        frame.setVisible(true);
        vue.requestFocusInWindow();
	}
}
