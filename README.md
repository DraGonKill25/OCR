# GIT Cheat sheet

## Vocabulaire

repo: projet git, dossier qui contient un sous dossier .git et dans lequel on
    peut utiliser les feature de gito

commit: ensemble de changements fais a un ou plusieurs fichiers depuis le
    commit precedent

branch: 'etiquette' associe a un commit, comme un drapeau qui est sur un commit
et qu'on peut deplacer

index: ensemble de changements qui sont pret a etre commit

## Commandes

### Basic

`git status`: affiche le status du repo avec les changements pas trackes, les
changements tracke mais pas dans l'index, et les changements dans l'index

`git add <file>` : ajoute les changements du fichier <file> dans l'index
`git commit -m "msg"` : crée un nouveau commit avec les changements de l'index
`git commit --amend` : ajoutes les changements de l'index dans le commit
precedent

### Log

`git log` : affiche les commits precedents
`git log --oneline --graph` : affiche les commits avec une seule ligne par
commit et un joli graph a cote
`git log --oneline --graph --all` : affiche tous les commits de toutes les
branches, pas que ce depuis la position actuelle
`git log -n 42` : affiche les 42 derniers commits

### Branch

`git branch` : affiche les branches
`git branch <name>` : cree la branche <name>
`git branch -d <name>` : supprime la branche <name>
`git switch <name>` : deplace la HEAD sur la branche <name>
`git switch -c <name>` : cree la branche <name> et deplace la HEAD dessus
(ancienne methode qui marche aussi `git checkout -b <name>`)

### Merge & Rebase
`git merge <ref>` : merge les changements de <ref> vers la position actuelle
`git merge --ff <ref>` : merge les changements en essayant de faire un fast
forward (option a vraie par defaut sauf si changee dans la config)
`git merge --no-ff <ref>` : merge sans fast forward meme quand c est possible
`git merge --ff-only <ref>` : merge en ne faisant qu'un fast forward, echoue
sinon
`git merge <ref> --no-edit` : merge avec le message par defaut de commit sans
demander a le changer

`git rebase <ref>` : copie les commit de la branche actuelle vers la <ref>
(`git rebase -i <ref>` truc avance : permet de vraiment manipuler les commit)

### Moving around
`git checkout <ref>` : permet de bouger la HEAD vers ou on veut, ca peut etre
une branche auquel cas c'est equivalent a un switch, ou un commit
`git bisect` : permet de parcourir tous les commit en faisant une recherche
dichotomique

### Reset

`git reset <ref>` : reset la branche actuelle vers la ref (cela peut etre une
ref de n'importe ou dans le repo, n'importe quel commit ou branche)
`git reset --hard <ref>` : reset la branche actuelle vers la ref
`git revert <ref>` : reset la branche actuelle vers la ref

`git reflog` : affiche le log de toutes les positions de la ref, meme les
commit ""caches""

### Stash

`git stash` : cree un pseudo commit local en sauvegardant tous les changement
locaux
`git stash pop` : recupere le dernier stash ajoute (le stash est un stack)
`git stash list` : list les differentes choses push dans le stash

### Remote
`git push` : push une branche sur la remote
`git push -u <remote-name> <branch>` : push une nouvelle branche <branch> sur
la remote <remote-name>
`git push -u <remote-name> <branch>:<remote-branch>` : push une nouvelle
branche <branch> sur la branche <remote-branch> de la remote <remote-name>
`git push --force` : /!\ DANGEUREUX : push une branche sur la remote en
permettant de ne pas fast forward, cela force la branche sur la remote a
bouger a l'endroit voulu
`git fetch` : met a jour les branches en remote, mais ne change pas les
branches locales
`git pull` : equivalent d'un git fetch puis git merge pour aussi mettre a jour
la branche locale
