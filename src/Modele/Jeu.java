package Modele;

public class Jeu {
    private int[][] cases;
    private int lignes;
    private int colonnes;
    private int joueur;
    final static int MANGE = 0;
    final static int PLEIN = 1;
    final static int JOUEUR1 = 0;
    final static int JOUEUR2 = 1;    

    public Jeu(int lignes, int colonnes) {
        init(lignes, colonnes);
        joueur = JOUEUR1;
    }

    public void init(int lignes, int colonnes) {
        if (lignes > 0 && colonnes > 0) {
            this.cases = new int[lignes][colonnes];
            for (int ligne = 0; ligne < lignes; ligne++) {
                for (int colonne = 0; colonne < colonnes; colonne++) {
                    cases[ligne][colonne] = PLEIN;
                }
            }
            this.lignes = lignes;
            this.colonnes = colonnes;
        }
    }

    public boolean clickable(int ligne, int colonne) {
        return cases[ligne][colonne] == 1;
    }

    public void click(int ligne, int colonne) {
        if (clickable(ligne, colonne)) {
            for (int i = 0; ligne + i < lignes; i++) {
                for (int j = 0; colonne + j < colonnes; j++) {
                    cases[ligne + i][colonne + j] = MANGE;
                }
            }
            joueur = (joueur == JOUEUR1) ? JOUEUR2 : JOUEUR1;
        }
    }
    
    public boolean defaite(int ligne, int colonne) {
        if (ligne == 0 && colonne == 0) {
            return true;
        }
        return false;
    }

    public boolean defaiteProchainJoueur() {
        if (lignes >= 2) {
            if (cases[1][0] == MANGE) {
                if (colonnes >= 2) {
                    return (cases[0][1] == MANGE);
                } else {
                    return true;
                }
            }
        } else if (colonnes >= 2) {
            return (cases[0][1] == MANGE);
        }
        return true;
    }

    public boolean mangeable(int ligne, int colonne) {
        return (cases[ligne][colonne] == PLEIN);
    }
    
    // appels
    public int colonnes() {
        return colonnes;
    }

    public int lignes() {
        return lignes;
    }
    public int[][] plateau() {
        return cases;
    }
}