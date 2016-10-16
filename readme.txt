Rédigé par Pacôme Perrotin le 15/10/2016.

Ce dossier est le rendu du projet en Statistiques Inférentielles.

Nous aborderons dans ce document les questions techniques.
Pour toute explication sur la philosophie du projet ou sur son fonctionnement général, nous vous référons au rapport de projet en pdf disponible dans le dossier doc.
Pour toute question technique sur le fonctionnement d'un morceau de code, nous vous renvoyons aux commentaires présents dans le code.
Si une quelconque question subsiste, soumetez-les à l'adresse : pacome.perrotin<at>gmail.com.

-- Compilation du programme --

L'ensemble des commandes çi-dessous doivent être exécutée à la racine de projet, c'est à dire à la location du readme.txt et du Makefile.

Pour compiler le programme, effectuer les commandes :
> make clear
> make all

-- Execution du programme --

Une fois compilé, le programme s'exécute avec la commande :

> bin/hmm [1] [2] [...]

Avec :
- [1] le chemin du corpus d'apprentissage
- [2] le chemin du corpus de test
- [...] un ensemble d'arguments optionnels parmis :
  - -v ou -s (verbose, silent)
  - -C ou -P (seulement compter, seulement perceptron)
  - -c=x fait varier la portion du corpus    }
  - -a=x fait varier la variable de lissage  } L'explication de ces arguments est trouvable dans le rapport du projet.
  - -I=x fait varier la variable d'itération }

Ces paramètres peuvent être rappelé lors d'une exécution du programme sans paramètre.

Exemples :
> bin/hmm data/ftb.train.encoded data/ftb.test.encoded -C -a=0.008 -c=1 -v
> bin/hmm data/ftb.train.encoded data/ftb.test.encoded -P -I=5 -c=0.5
> bin/hmm data/ftb.train.encoded data/ftb.test.encoded -s -c=0.01
