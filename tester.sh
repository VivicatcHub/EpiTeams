# contraintes techniques:
# lorsque vous tapez ldd myteams_cli et ldd myteams_server
# assurez-vous d'avoir libmyteams.so => not found dans la liste (!)
# assurez-vous de ne PAS avoir pthread
# lorsque vous tapez nm -D myteams_cli et nm -D myteams_server (si votre projet et en C et/ou C++)
# assurez-vous de ne PAS avoir fork
# assurez-vous de ne PAS avoir pthread
# assurez-vous d'avoir poll
# Il n'y a pas de norme si vous faites le projet en C/C++
# pas d'autorisation d'utiliser une lib externe (eg pour faire une BD)

#!/bin/bash

# Vérification de la présence des fichiers nécessaires
if [ ! -f "myteams_cli" ]; then
	echo "Erreur: myteams_cli n'est pas présent dans le répertoire."
	exit 1
fi

if [ ! -f "myteams_server" ]; then
	echo "Erreur: myteams_server n'est pas présent dans le répertoire."
	exit 1
fi

# Vérification des dépendances avec ldd
echo "Vérification des dépendances de myteams_cli avec ldd..."
ldd myteams_cli | grep "libmyteams.so" >/dev/null
if [ $? -ne 0 ]; then
	echo "Erreur: libmyteams.so est introuvable dans les dépendances de myteams_cli."
	exit 1
fi

echo "Vérification des dépendances de myteams_server avec ldd..."
ldd myteams_server | grep "libmyteams.so" >/dev/null
if [ $? -ne 0 ]; then
	echo "Erreur: libmyteams.so est introuvable dans les dépendances de myteams_server."
	exit 1
fi

# Vérification de l'absence de pthread et fork avec nm
echo "Vérification de l'absence de pthread et fork dans myteams_cli avec nm..."
nm -D myteams_cli | grep "pthread" >/dev/null
if [ $? -eq 0 ]; then
	echo "Erreur: pthread est présent dans myteams_cli."
	exit 1
fi

nm -D myteams_cli | grep "fork" >/dev/null
if [ $? -eq 0 ]; then
	echo "Erreur: fork est présent dans myteams_cli."
	exit 1
fi

echo "Vérification de l'absence de pthread et fork dans myteams_server avec nm..."
nm -D myteams_server | grep "pthread" >/dev/null
if [ $? -eq 0 ]; then
	echo "Erreur: pthread est présent dans myteams_server."
	exit 1
fi

nm -D myteams_server | grep "fork" >/dev/null
if [ $? -eq 0 ]; then
	echo "Erreur: fork est présent dans myteams_server."
	exit 1
fi

echo -e "\e[32mToutes les vérifications sont passées avec succès. Les contraintes techniques sont respectées.\e[0m"
