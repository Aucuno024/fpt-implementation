package Modele;

public class Jeu {
    int[][] cases;
    int lignes;
    int colonnes;

    Jeu(int lignes, int colonnes) {
        init(lignes, colonnes);
    }

    public void init(int lignes, int colonnes) {
        this.cases = new int[lignes][colonnes];
        for (int ligne = 0; ligne < lignes; ligne++) {
            for (int colonne = 0; colonne < colonnes; colonne++) {
                cases[ligne][colonne] = 1;
            }
        }
        this.lignes = lignes;
        this.colonnes = colonnes;
    }

    public boolean clickable(int ligne, int colonne) {
        return cases[ligne][colonne] == 1;
    }

    public void click(int ligne, int colonne) {
        for (int i = 0; ligne + i < lignes; i++) {
            for (int j = 0; colonne + j < colonnes; j++) {
                cases[ligne + i][colonne + j] = 0;
            }
        }
    }
    
    public boolean defaite(int ligne, int colonne) {
        if (ligne == 0 && colonne == 0) {
            return true;
        }
        return false;
    }

    public boolean defaiteProchainJoueur() {
        if (lignes >= 2 ) {
            if (cases[1][0] == 0) {
                if (colonnes >= 2) {
                    return (cases[0][1] == 0);
                } else {
                    return true;
                }
            }
        } else if (colonnes >= 2) {
            return (cases[0][1] == 0);
        }
        return true;
    }
}
