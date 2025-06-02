#!/bin/bash

# Controlla se sono stati forniti i parametri
if [ "$#" -ne 2 ]; then
    echo "Uso: $0 <stringa_da_cercare> <directory>"
    exit 1
fi

STRINGA=$1
DIRECTORY=$2

# Cerca la stringa nei file della directory, ignorando i file binari
grep -rnw --binary-files=without-match "$DIRECTORY" -e "$STRINGA" | while IFS=: read -r file riga; do
    # Estrai solo il numero di riga
    numero_riga=$(echo "$riga" | cut -d':' -f1)
    echo "Trovato in $file, riga $numero_riga"
    # Apri l'editor di testo sulla riga specificata
    nano "+$numero_riga" "$file" < /dev/tty
done
