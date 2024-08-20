#!/bin/bash

# Nom du fichier de sortie
output_file="output.log"

# Exécuter make et rediriger la sortie standard et d'erreur
make > "$output_file" 2>&1

# Afficher un message
echo "Sortie de make et g++ écrite dans $output_file"