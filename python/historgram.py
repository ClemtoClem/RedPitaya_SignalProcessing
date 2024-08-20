import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from python.display import *

if __name__ == "__main__":
	directory = select_directory()
	# Spécifiez le chemin vers votre fichier CSV
	fichier_csv = directory + "elapsedTime2.csv"

	# Lecture des données à partir du fichier CSV
	data, _ = read_csv(fichier_csv)
	elapsed_time = data["elapsed_time"]

	# Tracer l'histogramme
	plt.hist(elapsed_time, density=True, histtype='step', bins=70, label='Histogramme')

	# Filtrer les données pour ne garder que celles entre 0.13 et 0.16
	mask1 = (elapsed_time > 0.11) & (elapsed_time < 0.155)
	data1 = elapsed_time[mask1]

	# Filtrer les données pour ne garder que celles entre 0.16 et 0.18
	mask2 = (elapsed_time > 0.155) & (elapsed_time < 0.176)
	data2 = elapsed_time[mask2]

	# Calculer la moyenne et l'écart-type pour les deux ensembles de données
	mu1, std1 = norm.fit(data1)
	mu2, std2 = norm.fit(data2)

	# Tracer les courbes de distribution normale ajustées
	xmin, xmax = plt.xlim()
	x = np.linspace(xmin, xmax, 1000)

	p1 = norm.pdf(x, mu1, std1)
	p1_max = np.max(p1)
	p1 = p1 * np.abs(80 / p1_max)
	plt.plot(x, p1, 'k', linewidth=2, label=f'Fit: mu1={mu1:.2f}, std1={std1:.2f}')

	p2 = norm.pdf(x, mu2, std2)
	p2_max = np.max(p2)
	p2 = p2 * np.abs(29 / p2_max)
	plt.plot(x, p2, 'r', linewidth=2, label=f'Fit: mu2={mu2:.2f}, std2={std2:.2f}')

	# Ajouter des labels et un titre
	plt.xlabel('Temps entre deux acquisitions (ms)', fontsize=16)
	plt.ylabel('Distribution', fontsize=16)
	plt.title('Distribution du temps entre deux acquisitions', fontsize=16)
	plt.legend()
	plt.grid(True)

	# Ajuster la taille de la police des numérotations des axes
	plt.tick_params(axis='both', which='major', labelsize=16)
	plt.tick_params(axis='both', which='minor', labelsize=16)

	# Afficher le graphique
	plt.show()

	# Afficher la moyenne et l'écart-type
	print(f"Moyenne 1 (mu): {mu1}")
	print(f"Écart-type 1 (sigma): {std1}")

	# Afficher la moyenne et l'écart-type
	print(f"Moyenne 2 (mu): {mu2}")
	print(f"Écart-type 2 (sigma): {std2}")
