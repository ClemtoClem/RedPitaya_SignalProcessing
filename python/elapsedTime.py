import pandas as pd
import matplotlib.pyplot as plt
from python.display import *

if __name__ == "__main__":
	directory = select_directory()
	fichier_csv2 = directory + "/elapsedTime.csv"
	signals2, _ = read_csv(fichier_csv2)

	# Appel de la fonction pour tracer les données
	fig, ax = plt.subplots(figsize=(16/1.2, 9/1.8))
	plot_data(ax, signals2, "loop", axis_form = "linear", title = "Mesure du temps de réponse de la RedPitaya")
	plt.tight_layout()
	plt.show()
