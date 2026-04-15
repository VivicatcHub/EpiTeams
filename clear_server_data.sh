#!/bin/sh

if [ ! -d "data" ]; then
	printf '"data" folder does not exist. Deletion program not required.\n'
	return
fi

printf 'This will delete the "data" folder and all its content.\n'
printf 'Do you want to continue? [y/N]: '
read -r answer

case "$answer" in
y | Y | yes | YES | Yes)
	rm -rf -- "data"
	printf '"data" folder deleted.\n'
	;;
*)
	printf 'Operation cancelled.\n'
	;;
esac
