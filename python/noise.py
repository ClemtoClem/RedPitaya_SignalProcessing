import numpy as np
import matplotlib.pyplot as plt
from python.display import *

if __name__ == "__main__":
	# Spécifiez le chemin vers votre fichier CSV
	fichier_csv = "./data/noiseRMS.csv"
	
	# Lecture des données à partir du fichier CSV
	signals: dict = read_csv(fichier_csv, "PointsPerPeriod")

	# Appel de la fonction pour tracer les données
	plot_data(signals, "PointsPerPeriod", "semilogx", title="Bruit RMS en fonction du nombre de points par période")
