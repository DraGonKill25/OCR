# OCR

#POUR CREER ET MERGE LES BRANCHS

``git branch ``: creer une nouvelle branche

``git checkout (nom de la branch)`` : permet de se deplacer de branche en branche

``git merge`` : fusionne plusieurs branches on se place sur la branche qu'on veux mettre a jours
    et on merge la branche qui contient les modifications


#POUR VOIR LES BRANCH QUI ONT BESOIN D'ETRE MERGE
``git branch --no-merged``


#SI LE MERGE NE MARCHE PAS UTILISER :
``git mergetool`` : lance un assistant de fusion des branches


#POUR VOIR VOS COMMITS :
``git log``


#POUR FIXER UN POINT DANS VOS COMMITS POUR SAVOIR QUELLE COMMIT RECUPERER SI CA PLANTE :
``git stash`` : vos pouver ainsi creer une etiquette et la verifier avec "-s" et "-v"


#POUR SUPPRIMER DES COMMITS FAIS PAR DES CAMARADES PAS TRES GENTIL QUI POLLUE LE REPO
``git reset --hard HEAD``
