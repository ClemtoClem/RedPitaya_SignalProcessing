import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Button, TextBox
from scipy.signal import find_peaks
from python.display import *


def plot_filter(ax: plt.Axes, fc, Q=0.5, A=1, fmin=10, fmax=100000, num_points=16384, title="Filtre"):
	# Calculate the filter
	freqs = np.logspace(np.log10(fmin), np.log10(fmax), num_points)
	filter = np.zeros(len(freqs))

	for i in range(len(freqs)):
		filter[i] = A * (fc ** 2) / np.sqrt((fc ** 2 - freqs[i] ** 2) ** 2 + (freqs[i] * fc / Q) ** 2)

	# Plot the filter
	ax.loglog(freqs, filter, label=title + f" Q={Q} fc={fc} A={A}")

def calculate_rise_time(time, signal):
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
	

def plot_signal_with_rise_time(ax: plt.Axes, signals: dict, signal_name: str):
	time = signals['time']
	if not signal_name in signals:
		raise ValueError(f"Le signal '{signal_name}' n'existe pas dans les données fournies.")
	signal = signals[signal_name]

	rise_time, low_index, high_index = calculate_rise_time(time, signal)
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
def next_plot(event):
	change_plot(1)

def previous_plot(event):
	change_plot(-1)

def change_plot(direction):
	global current_plot_index
	current_plot_index = (current_plot_index + direction) % 5

	ax.clear()
	ax.set_visible(True)
	if current_plot_index == 0:
		filtered_signals = { 'time': signals['time']}
		for key in signals:
			if key[:6] == 'signal':
				filtered_signals[key] = signals[key]
		plot_data(ax, filtered_signals, 'time', 'linear', title="Signal", usedColorMap=False)
	if current_plot_index == 1:
		filtered_signals = { 'time': signals['time']}
		for key in signals:
			if key[:9] == 'amplitude':
				filtered_signals[key] = signals[key]
		plot_data(ax, filtered_signals, 'time', 'linear', title="Amplitude", usedColorMap=False)
	if current_plot_index == 2:
		filtered_signals = { 'time': signals['time']}
		for key in signals:
			if key[:5] == 'phase':
				filtered_signals[key] = signals[key]
		plot_data(ax, filtered_signals, 'time_radian', 'linear', title="Phase", usedColorMap=False)
	elif current_plot_index == 3:
		plot_data(ax, fft_signals, 'frequency', 'loglog', title = "Transformée de Fourier discrète des signaux acquis", usedColorMap=False)
	elif current_plot_index == 4:
		plot_data(ax, fft_amplitudes, 'frequency', 'loglog', title = "Transformée de Fourier discrète de l'amplitude démodulée", usedColorMap=False)
		#plot_filter(ax, 1000, 1.0, 0.2/np.sqrt(2))
		
	plt.draw()  # Rafraîchir l'écran

directory = select_directory()

# Nom du fichier CSV contenant les signaux
filename1 = directory + '/signals.csv'

# Nom du fichier CSV contenant les FFT
fft_filename1 = directory + '/spectrums.csv'
fft_filename2 = directory + '/spectrums_amplitudes.csv'

# Lecture des signaux à partir du fichier CSV
signals, _ = read_csv(filename1)

# Lecture des FFT à partir du fichier CSV
fft_signals, _ = read_csv(fft_filename1)
fft_amplitudes, _ = read_csv(fft_filename2)

# Création de la figure et des sous-graphiques
fig, ax = plt.subplots(figsize=(16/1.5, 9/1.8))
gs = fig.add_gridspec(2, hspace=0.3)

# Ajout du bouton pour basculer entre les affichages des signaux et des FFT
next_button_ax = plt.axes([0.895, 0.96, 0.1, 0.035])
next_button = Button(next_button_ax, '>>')
next_button.on_clicked(next_plot)
previous_button_ax = plt.axes([0.005, 0.96, 0.1, 0.035])
previous_button = Button(previous_button_ax, '<<')
previous_button.on_clicked(previous_plot)

# Affichage initial des signaux
current_plot_index = 0
change_plot(0)

plt.tight_layout()
plt.show()
