-- Ceci est un commentaire
-- Exemple d'utilisation basique de scripts Lua avec ES

-- Un nombre
nombre = 10+5;

-- Un nombre à virgules
flottant = 10.5;

-- Une chaîne de caractéres
message = "Voici un exemple d'utilisation des scripts avec le moteur !"

-- Appel d'une fonction
affiche();
affiche();

-- Appel d'une fonction avec paramétres (ici la chaîne défini plus haut)
affiche(message);

-- Appel des mêmes fonctions avec des textes différents
affiche("Exemple utile non ?");
affiche("En plus c'est très simple avec ES!");
affiche("Hop encore un nouveau message :)");
affiche("!!!!");
affiche("Bon allez, places aux messages par défauts");

--- Codes non pris en comptes car pas défini ---
-- Appel de la fonction affiche avec 2 arguments (au lieu de 1 et aucuns)
affiche("texte", "texte");

-- Appel d'une fonction non défini dans le programme
affiche_rien();

