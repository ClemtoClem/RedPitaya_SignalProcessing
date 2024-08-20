import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
from matplotlib.widgets import Button, TextBox
from scipy.signal import find_peaks
import os, sys, configparser

def parse_cfg_to_dict(cfg_path):
	config = configparser.ConfigParser()
	config.read(cfg_path)
	
	config_dict = {}
	for section in config.sections():
		#print(section)
		config_dict[section] = {}
		for key, value in config.items(section):
			config_dict[section][key] = value
	return config_dict

def list_directories(path):
	try:
		directories = [d for d in os.listdir(path) if os.path.isdir(os.path.join(path, d))]
		# Trier les répertoires : si le nom est composé de chiffres, trier par nombre, sinon par ordre alphabétique
		directories.sort(key=lambda x: (not x.isdigit(), int(x) if x.isdigit() else x))
		return directories
	except PermissionError:
		return []

def navigate_directory(current_path):
	previous_path = None
	
	while True:
		directories = list_directories(current_path)
		
		if not directories:
			#print("Aucun sous-répertoire trouvé ou accès refusé.")
			return current_path
		
		print(f"\nContenu du répertoire: {current_path}")
		print("0. ./ (Choisir ce répertoire)")
		if previous_path:
			print("1. ../ (Retour au répertoire précédent)")
			for idx, directory in enumerate(directories):
				print(f"{idx + 2}. {directory}")
		else:
			for idx, directory in enumerate(directories):
				print(f"{idx + 1}. {directory}")
		print("-1. Quitter")

		choice = input("Sélectionnez un numéro pour naviguer dans un sous-répertoire (ou 0 pour sélectionner le répertoire actuel): ")
		
		if choice.isdigit():
			choice = int(choice)
			if choice == 0:
				return current_path
			elif previous_path and choice == 1:
				current_path = previous_path
				previous_path = os.path.dirname(previous_path)
			elif previous_path:
				if 2 <= choice <= len(directories) + 1:
					previous_path = current_path
					current_path = os.path.join(current_path, directories[choice - 2])
				else:
					print("Choix invalide. Veuillez réessayer.")
			elif choice == -1:
				print("Au revoir.")
				sys.exit()
			else:
				if 1 <= choice <= len(directories):
					previous_path = current_path
					current_path = os.path.join(current_path, directories[choice - 1])
				else:
					print("Choix invalide. Veuillez réessayer.")
		else:
			print("Entrée non valide. Veuillez entrer un numéro.")

def select_directory():
	start_path = os.getcwd()
	selected_directory = navigate_directory(start_path + "/data")
	print(f"\nVous avez sélectionné le répertoire : {selected_directory}")
	return selected_directory

def read_csv(filename, axis_name=""):
	"""
	Fonction pour lire un fichier CSV et le convertir en dictionnaire
	:param filename: Le chemin vers le fichier CSV
	:param typePlot: Le type de graphique à tracer (time, freq, ...)
	:return: Un dictionnaire avec les titres comme clés et les données comme valeurs
	"""

	signals = {}
	titles = []

	with open(filename, 'r') as file:
		lines = file.readlines()

		# Les lignes suivantes contiennent les valeurs des signaux
		for line in lines:
			values = line.split(",")
			title = values[0].strip()  # Supprimer les espaces autour du titre
			signals[title] = []
			titles.append(title)

			# Conversion des données en float ou en complex
			for val in values[1:]:
				val = val.strip()  # Supprimer les espaces autour de la valeur
				try:
					if title == axis_name or 'j' not in val:
						signals[title].append(float(val))
					else:
						signals[title].append(complex(val))
				except ValueError:
					signals[title].append(0)

	# Conversion des listes en arrays numpy
	for key, value in signals.items():
		signals[key] = np.array(value)

	return signals, titles

def plot_data(ax: plt.Axes, signals:dict, plot_type, axis_form="linear", title="", ignore_signals=[], unit="", 
			  title_fontsize=16, axis_label_fontsize=14, tick_label_fontsize=14, usedColorMap = False, axis_name="", calculateModule = True):
	"""
	Fonction pour tracer les données
	:param ax: L'axe à utiliser pour tracer les données
	:param signals: Le dictionnaire avec les données à tracer
	:param plot_type: Le type de graphique à tracer (Time, Frequency, PointsPerPeriod)
	:param axis_form: Le type d'axe à utiliser pour tracer les données (linear, semilogx, semilogy, loglog)
	:param title: Le titre du graphique
	:param ignore_signals: La liste des signaux à ignorer lors du tracé
	:param unit: L'unité à utiliser pour tracer les données (k, m, u, n)
	:param title_fontsize: La taille de la police du titre du graphique
	:param axis_label_fontsize: La taille de la police des étiquettes des axes
	:param tick_label_fontsize: La taille de la police des numérotations des axes
	:param usedColorMap: La colormap à utiliser pour tracer les données
	:param axis_name: Le nom de l'axe à utiliser pour tracer les données
	"""

	axis_type = plot_type
	if plot_type == "freq_radian":
		axis_type = "frequency"
	elif plot_type == "time_radian":
		axis_type = "time"
	elif plot_type == "loop":
		axis_type = "loop_axis"
	axis = None
	if axis_name != "":
		axis = signals[axis_name]
	else:
		axis = signals[axis_type]

	unit_multiplicator = 1
	if unit == "k":
		unit_multiplicator = 1 / 1000
	elif unit == "m":
		unit_multiplicator = 1000
	elif unit == "u":
		unit_multiplicator = 1000000
	elif unit == "n":
		unit_multiplicator = 1000000000

	cmap = None
	norm = None
	color = None
	if usedColorMap:
		# Définir une colormap du rouge au bleu
		colors = [(0/255, 255/255, 0/255), (89/255, 0/255, 140/255)]
		cmap = mcolors.LinearSegmentedColormap.from_list('cmap', colors, N=len(signals)-1)
		norm = mcolors.Normalize(vmin=0, vmax=len(signals)-2)

	# Parcourir les colonnes restantes pour tracer chaque série de données
	for idx, (name, signal) in enumerate(signals.items()):
		if name != axis_type and name != axis_name:
			if len(axis) == len(signal) and (name not in ignore_signals):
				if axis_type == "frequency" and calculateModule:
					signal = np.abs(signal) / len(signal)
				if usedColorMap:
					color = cmap(norm(idx))
				if axis_form == "linear":
					ax.plot(axis * unit_multiplicator, signal, label=name, color=color)
				elif axis_form == "semilogx":
					ax.semilogx(axis * unit_multiplicator, signal, label=name, color=color)
				elif axis_form == "semilogy":
					ax.semilogy(axis * unit_multiplicator, signal, label=name, color=color)
				elif axis_form == "loglog":
					ax.loglog(axis * unit_multiplicator, signal, label=name, color=color)
				else:
					print("Error: axis_form not recognized")
					print("The possible values are: regular, semilogx, semilogy, loglog")
					return
				
	# Ajouter des étiquettes et une légende
	if plot_type == "time":
		ax.set_xlabel(f'Temps [{unit}s]', fontsize=axis_label_fontsize)
		ax.set_ylabel('Amplitude [V]', fontsize=axis_label_fontsize)
	elif plot_type == "frequency":
		ax.set_xlabel(f'Fréquence [{unit}Hz]', fontsize=axis_label_fontsize)
		ax.set_ylabel(f'Amplitude [V]', fontsize=axis_label_fontsize)
	elif plot_type == "pointsPerPeriod":
		ax.set_xlabel('Nombre de périodes', fontsize=axis_label_fontsize)
		ax.set_ylabel('Amplitude [V]', fontsize=axis_label_fontsize)
	elif plot_type == "freq_radian":
		ax.set_xlabel(f'Fréquence [{unit}Hz]', fontsize=axis_label_fontsize)
		ax.set_ylabel('Phase [rad]', fontsize=axis_label_fontsize)
	elif plot_type == "time_radian":
		ax.set_xlabel(f'Temps [{unit}s]', fontsize=axis_label_fontsize)
		ax.set_ylabel('Phase [rad]', fontsize=axis_label_fontsize)
	elif plot_type == "loop":
		ax.set_xlabel(f'Nb de tests', fontsize=axis_label_fontsize)
		ax.set_ylabel('Temps [s]', fontsize=axis_label_fontsize)

	if title != "":
		ax.set_title(title, fontsize=title_fontsize)
	else:
		ax.set_title(f"Affichage du spectre", fontsize=title_fontsize)
	ax.legend()
	ax.grid(True)

	# Ajuster la taille de la police des numérotations des axes
	ax.tick_params(axis='both', which='major', labelsize=tick_label_fontsize)
	ax.tick_params(axis='both', which='minor', labelsize=tick_label_fontsize)


class SimplePlot:
	def __init__(self, directory: str):
		# Nom du fichier CSV contenant les signaux
		filename1 = directory + '/signals.csv'
		filename2 = directory + '/spectrums.csv'

		# Lecture des données à partir du fichier CSV
		self.signals, _ = read_csv(filename1)
		self.spectrums, _ = read_csv(filename2)

		# Création de la figure et des sous-graphiques
		fig, self.ax = plt.subplots(figsize=(16/1.5, 9/1.8))
		gs = fig.add_gridspec(2, hspace=0.3)

		# Ajout du bouton pour basculer entre les affichages des signaux et des FFT
		self.previous_button_ax = plt.axes([0.005, 0.96, 0.025, 0.035])
		self.previous_button = Button(self.previous_button_ax, '<<')
		self.previous_button.on_clicked(self.previous_plot)
		self.next_button_ax = plt.axes([0.97, 0.96, 0.025, 0.035])
		self.next_button = Button(self.next_button_ax, '>>')
		self.next_button.on_clicked(self.next_plot)

		# Affichage initial des signaux
		self.current_plot_index = 0
		self.change_plot(0)

		plt.tight_layout()
		plt.show()

		# Fonctions pour basculer entre les plot
	def next_plot(self, event):
		self.change_plot(1)

	def previous_plot(self, event):
		self.change_plot(-1)

	def change_plot(self, direction):
		self.current_plot_index = (self.current_plot_index + direction) % 2

		self.ax.clear()
		self.ax.set_visible(True)
		if self.current_plot_index == 0:
			plot_data(self.ax, self.signals, 'time', 'linear', title="Signaux", usedColorMap=False)
		elif self.current_plot_index == 1:
			plot_data(self.ax, self.spectrums, 'frequency', 'loglog', title="Spectres", usedColorMap=False)
			
		plt.draw()  # Rafraîchir l'écran

class DemodulationPlot:
	def __init__(self, directory: str):
		# Nom du fichier CSV contenant les signaux
		filename1 = directory + '/signals.csv'

		# Nom du fichier CSV contenant les FFT
		fft_filename1 = directory + '/spectrums.csv'
		fft_filename2 = directory + '/spectrums_amplitudes.csv'

		# Lecture des signaux à partir du fichier CSV
		self.signals, _ = read_csv(filename1)

		# Lecture des FFT à partir du fichier CSV
		self.fft_signals, _ = read_csv(fft_filename1)
		self.fft_amplitudes, _ = read_csv(fft_filename2)

		# Création de la figure et des sous-graphiques
		fig, self.ax = plt.subplots(figsize=(16/1.5, 9/1.8))
		gs = fig.add_gridspec(2, hspace=0.3)

		# Ajout du bouton pour basculer entre les affichages des signaux et des FFT
		self.previous_button_ax = plt.axes([0.005, 0.96, 0.025, 0.035])
		self.previous_button = Button(self.previous_button_ax, '<<')
		self.previous_button.on_clicked(self.previous_plot)
		self.next_button_ax = plt.axes([0.97, 0.96, 0.025, 0.035])
		self.next_button = Button(self.next_button_ax, '>>')
		self.next_button.on_clicked(self.next_plot)

		# Affichage initial des signaux
		self.current_plot_index = 0
		self.change_plot(0)

		plt.tight_layout()
		plt.show()

	def plot_filter(self, ax: plt.Axes, fc, Q=0.5, A=1, fmin=10, fmax=100000, num_points=16384, title="Filtre"):
		# Calculate the filter
		freqs = np.logspace(np.log10(fmin), np.log10(fmax), num_points)
		filter = np.zeros(len(freqs))

		for i in range(len(freqs)):
			filter[i] = A * (fc ** 2) / np.sqrt((fc ** 2 - freqs[i] ** 2) ** 2 + (freqs[i] * fc / Q) ** 2)

		# Plot the filter
		ax.loglog(freqs, filter, label=title + f" Q={Q} fc={fc} A={A}")

	def calculate_rise_time(self, time, signal):
		# Définir les seuils bas et haut
		min_signal = np.min(signal)
		max_signal = np.max(signal)
		low_threshold = min_signal + 0.1 * (max_signal - min_signal)
		high_threshold = min_signal + 0.9 * (max_signal - min_signal)

		# Trouver les index où le signal dépasse les seuils
		try:
			low_index = np.where(signal > low_threshold)[0][0]
			high_index = np.where(signal > high_threshold)[0][0]

			# Assurer que high_index est après low_index
			if high_index <= low_index:
				raise ValueError("L'index de seuil haut est avant ou égal à l'index de seuil bas.")

			# Calculer le temps de montée
			rise_time = time[high_index] - time[low_index]
		except IndexError:
			raise ValueError("Le signal ne dépasse pas les seuils spécifiés.")
		
		return rise_time, low_index, high_index
		

	def plot_signal_with_rise_time(self, ax: plt.Axes, signals: dict, signal_name: str):
		time = signals['time']
		if not signal_name in signals:
			raise ValueError(f"Le signal '{signal_name}' n'existe pas dans les données fournies.")
		signal = signals[signal_name]

		rise_time, low_index, high_index = self.calculate_rise_time(time, signal)
		print("rise time :", rise_time, "s, low_index :", low_index, ", high_index :", high_index)

		ax.plot(time * 1000, np.real(signal), label=signal_name)
		ax.axvline(time[low_index] * 1000, color='r', linestyle='--', label='10% threshold')
		ax.axvline(time[high_index] * 1000, color='g', linestyle='--', label='90% threshold')
		ax.set_xlabel('Time (ms)')
		ax.set_ylabel('Amplitude')
		ax.set_title(f'Signal with Rise Time: {rise_time:.2e} s')
		ax.legend()
		ax.grid(True)

	# Fonctions pour basculer entre les plot
	def next_plot(self, event):
		self.change_plot(1)

	def previous_plot(self, event):
		self.change_plot(-1)

	def change_plot(self, direction):
		self.current_plot_index = (self.current_plot_index + direction) % 5

		self.ax.clear()
		self.ax.set_visible(True)
		if self.current_plot_index == 0:
			filtered_signals = { 'time': self.signals['time']}
			for key in self.signals:
				if key[:6] == 'signal':
					filtered_signals[key] = self.signals[key]
			plot_data(self.ax, filtered_signals, 'time', 'linear', title="Signal", usedColorMap=False)
		if self.current_plot_index == 1:
			filtered_signals = { 'time': self.signals['time']}
			for key in self.signals:
				if key[:9] == 'amplitude':
					filtered_signals[key] = self.signals[key]
			plot_data(self.ax, filtered_signals, 'time', 'linear', title="Amplitude", usedColorMap=False)
		if self.current_plot_index == 2:
			filtered_signals = { 'time': self.signals['time']}
			for key in self.signals:
				if key[:5] == 'phase':
					filtered_signals[key] = self.signals[key]
			plot_data(self.ax, filtered_signals, 'time_radian', 'linear', title="Phase", usedColorMap=False)
		elif self.current_plot_index == 3:
			plot_data(self.ax, self.fft_signals, 'frequency', 'loglog', unit="k", title = "Transformée de Fourier discrète du signal", usedColorMap=False)
		elif self.current_plot_index == 4:
			plot_data(self.ax, self.fft_amplitudes, 'frequency', 'loglog', unit="k", title = "Transformée de Fourier discrète de l'amplitude du signal démodulé", usedColorMap=False)
			#plot_filter(ax, 1000, 1.0, 0.2/np.sqrt(2))
			
		plt.draw()  # Rafraîchir l'écran

class FrequencyScanningPlot:
	def __init__(self, directory: dict, configs: dict):
		self.configs = configs
		self.loadAll = False
		if configs["program"]["debug"] == "1":
			letter = ''
			while letter != 'y' and letter != 'n':
				letter = input("Voulez-vous charger tous les fichiers ? (y/n) ")

			if letter == 'y':
				self.loadAll = True
			else:
				self.loadAll = False
		
		
		if self.loadAll == True:
			filename1  = directory + '/signals.csv'
			filename2  = directory + '/amplitudes.csv'
			filename2b = directory + '/phases.csv'
		
		filename3  = directory + '/amplitudeOfMovement.csv'
		filename4  = directory + '/phaseOfMovement.csv'

		# Lecture des fichiers CSV
		if self.loadAll:
			self.signals, self.titles_signals = read_csv(filename1)
			self.amplitudes, self.titles_amplitudes = read_csv(filename2)
			self.phases, self.titles_phases = read_csv(filename2b)
		self.amplitudeOfMovement, _ = read_csv(filename3)
		self.phaseOfMovement, _ = read_csv(filename4)

		# Création de la figure et des sous-graphiques
		fig, self.ax = plt.subplots(figsize=(16/1.2, 9/1.8))
		gs = fig.add_gridspec(2, hspace=0.3)

		# Ajout du bouton pour basculer entre les affichages des signaux et des FFT
		self.previous_button_ax = plt.axes([0.005, 0.96, 0.025, 0.035])
		self.previous_button = Button(self.previous_button_ax, '<<')
		self.previous_button.on_clicked(self.previous_plot)
		self.next_button_ax = plt.axes([0.97, 0.96, 0.025, 0.035])
		self.next_button = Button(self.next_button_ax, '>>')
		self.next_button.on_clicked(self.next_plot)
		self.previous_signal_button_ax = plt.axes([0.035, 0.96, 0.025, 0.035])
		self.previous_signal_button = Button(self.previous_signal_button_ax, '-')
		self.previous_signal_button.on_clicked(self.previous_signal)
		self.next_signal_button_ax = plt.axes([0.94, 0.96, 0.025, 0.035])
		self.next_signal_button = Button(self.next_signal_button_ax, '+')
		self.next_signal_button.on_clicked(self.next_signal)

		# Affichage initial des signaux
		self.current_signal_index = 0
		if self.loadAll:
			self.current_plot_index = 6
		else:
			self.current_plot_index = 0
		self.change_plot(0)

		plt.tight_layout()
		plt.show()

	# Fonctions pour basculer entre les plot
	def next_plot(self, event):
		self.change_plot(1)

	def previous_plot(self, event):
		self.change_plot(-1)

	def next_signal(self, event):
		self.change_signal(1)

	def previous_signal(self, event):
		self.change_signal(-1)

	def change_signal(self, direction):
		self.current_signal_index = (self.current_signal_index + direction) % (len(self.signals)//2-1)
		self.change_plot(0)

	def change_plot(self, direction):
		self.current_plot_index = (self.current_plot_index + direction) % 8
		if self.loadAll == False and self.current_plot_index == 0:
			self.current_plot_index = 6
		
		self.ax.clear()
		self.ax.set_visible(True)
		if self.current_plot_index == 0:
			self.next_signal_button_ax.set_visible(True)
			self.previous_signal_button_ax.set_visible(True)
			name = self.titles_signals[2*self.current_signal_index+1]
			signal = { 'time': self.signals['time'], name: self.signals[name] }
			freq = name.split("_")[-1]
			plot_data(self.ax, signal, 'time', 'linear', title = "Signal du channel 1 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
		elif self.current_plot_index == 1:
			self.next_signal_button_ax.set_visible(True)
			self.previous_signal_button_ax.set_visible(True)
			name = self.titles_signals[2*self.current_signal_index+2]
			signal = { 'time': self.signals['time'], name: self.signals[name] }
			freq = name.split("_")[-1]
			plot_data(self.ax, signal, 'time', 'linear', title = "Signal du channel 2 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
		if self.current_plot_index == 2:
			self.next_signal_button_ax.set_visible(True)
			self.previous_signal_button_ax.set_visible(True)
			name = self.titles_phases[2*self.current_signal_index+1]
			phase = { 'time': self.phases['time'], name: self.phases[name] }
			freq = name.split("_")[-1]
			plot_data(self.ax, phase, 'time_radian', 'linear', title = "phase du channel 1 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
		elif self.current_plot_index == 3:
			self.next_signal_button_ax.set_visible(True)
			self.previous_signal_button_ax.set_visible(True)
			name = self.titles_phases[2*self.current_signal_index+2]
			phase = { 'time': self.phases['time'], name: self.phases[name] }
			freq = name.split("_")[-1]
			plot_data(self.ax, phase, 'time_radian', 'linear', title = "phase du channel 2 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
		elif self.current_plot_index == 4:
			name = self.titles_amplitudes[2*self.current_signal_index+1]
			amplitude = { 'time': self.amplitudes['time'], name: self.amplitudes[name] }
			freq = name.split("_")[-1]
			plot_data(self.ax, amplitude, 'time', 'linear', title = "Amplitude calculée à partir du signal 1 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
		elif self.current_plot_index == 5:
			name = self.titles_amplitudes[2*self.current_signal_index+2]
			amplitude = { 'time': self.amplitudes['time'], name: self.amplitudes[name] }
			freq = name.split("_")[-1]
			plot_data(self.ax, amplitude, 'time', 'linear', title = "Amplitude calculée à partir du signal 2 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
		elif self.current_plot_index == 6:
			self.next_signal_button_ax.set_visible(False)
			self.previous_signal_button_ax.set_visible(False)
			plot_data(self.ax, self.amplitudeOfMovement, 'frequency', 'linear', title = "Affichage de l'amplitude de déplacement", unit="k", usedColorMap = False, calculateModule = False)
			freq_res = float(self.configs["measures"]["amplitude_max_frequency"])/1000
			self.ax.axvline(freq_res, color='r', linestyle='--', label='frequence de résonnance ' + str(freq_res) + 'KHz')
		elif self.current_plot_index == 7:
			plot_data(self.ax, self.phaseOfMovement, 'freq_radian', 'linear', title = "Affichage de la phase de déplacement", unit="k", usedColorMap = False, calculateModule = False)
			freq_res = float(self.configs["measures"]["phase_max_frequency"])/1000
			self.ax.axvline(freq_res, color='r', linestyle='--', label='frequence de résonnance ' + str(freq_res) + 'KHz')
		self.ax.grid(True)
		self.ax.legend()
		plt.tight_layout()
		plt.draw()  # Rafraîchir l'écran

if __name__ == "__main__":
	directory = select_directory()

	# lire les données du fichier de descriptions
	config_dict = parse_cfg_to_dict(directory + "/descriptions.cfg")

	name = config_dict['program']["name"]

	if name == "acquire" or name == "spectrum":
		simple = SimplePlot(directory)
	elif name == "demodulation":
		dem = DemodulationPlot(directory)
	elif name == "frequencyScanning":
		freq = FrequencyScanningPlot(directory, config_dict)


