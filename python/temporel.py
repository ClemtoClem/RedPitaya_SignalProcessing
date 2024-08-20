import pandas as pd
import matplotlib.pyplot as plt
from python.display import *

if __name__ == "__main__":
	directory = select_directory()
	fichier_csv = directory + "/signals.csv"
	
	# Lecture des données à partir du fichier CSV
	signals, _ = read_csv(fichier_csv)

	ignore_signals = []

	# Appel de la fonction pour tracer les données
	fig, ax = plt.subplots(figsize=(16/1.2, 9/1.8))
	plot_data(ax, signals, "time", axis_form = "linear", title = "Affichage des signal", unit="m", usedColorMap=False, ignore_signals=ignore_signals)
	plt.tight_layout()
	plt.show()
