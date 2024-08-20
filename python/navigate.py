import os
import sys

def list_directories(path):
    try:
        return [d for d in os.listdir(path) if os.path.isdir(os.path.join(path, d))]
    except PermissionError:
        return []

def navigate_directory(current_path):
    while True:
        print(f"\nContenu du répertoire: {current_path}")
        directories = list_directories(current_path)
        
        if not directories:
            print("Aucun sous-répertoire trouvé ou accès refusé.")
            return current_path
        
        for idx, directory in enumerate(directories):
            print(f"{idx + 1}. {directory}")
        print("0. Choisir ce répertoire")

        choice = input("Sélectionnez un numéro pour naviguer dans un sous-répertoire (ou 0 pour sélectionner le répertoire actuel): ")
        
        if choice.isdigit():
            choice = int(choice)
            if choice == 0:
                return current_path
            elif 1 <= choice <= len(directories):
                current_path = os.path.join(current_path, directories[choice - 1])
            else:
                print("Choix invalide. Veuillez réessayer.")
        else:
            print("Entrée non valide. Veuillez entrer un numéro.")

if __name__ == "__main__":
    start_path = os.getcwd()
    selected_directory = navigate_directory(start_path)
    print(f"\nVous avez sélectionné le répertoire : {selected_directory}")
