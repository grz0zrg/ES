-- Ceci est un commentaire
-- Exemple d'utilisation basique de scripts Lua avec ES

-- Un nombre
nombre = 10+5;

-- Un nombre � virgules
flottant = 10.5;

-- Une cha�ne de caract�res
message = "Voici un exemple d'utilisation des scripts avec le moteur !"

-- Appel d'une fonction
affiche();
affiche();

-- Appel d'une fonction avec param�tres (ici la cha�ne d�fini plus haut)
affiche(message);

-- Appel des m�mes fonctions avec des textes diff�rents
affiche("Exemple utile non ?");
affiche("En plus c'est tr�s simple avec ES!");
affiche("Hop encore un nouveau message :)");
affiche("!!!!");
affiche("Bon allez, places aux messages par d�fauts");

--- Codes non pris en comptes car pas d�fini ---
-- Appel de la fonction affiche avec 2 arguments (au lieu de 1 et aucuns)
affiche("texte", "texte");

-- Appel d'une fonction non d�fini dans le programme
affiche_rien();

