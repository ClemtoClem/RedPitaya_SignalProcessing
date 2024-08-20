import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Button, TextBox
from scipy.signal import find_peaks
from python.display import *

# Fonction pour afficher les signaux
def plot_signals(ax: plt.Axes, signals: dict, title = "Signals", xlabel = "Time", ylabel = "Amplitude [V]"):
	if len(signals) == 0 or "time" not in signals.keys():
		return

	plot_data(ax, signals, 'time', 'linear', title, unit='m', usedColorMap = False)

# Fonction pour afficher les FFT avec des axes logarithmiques
def plot_FFT(ax: plt.Axes, fft_signals: dict, title = "Tranformée de Fourier", ylabel = "Amplitude (V)", xlabel = "Frequency [Hz]"):
	if len(fft_signals) == 0 or "freq" not in fft_signals.keys():
		return

	plot_data(ax, fft_signals, 'freq', 'loglog', title, usedColorMap = False)

# Fonctions pour basculer entre les plot
def next_plot(event):
	change_plot(1)

def previous_plot(event):
	change_plot(-1)

def next_signal(event):
	change_signal(1)

def previous_signal(event):
	change_signal(-1)

def change_signal(direction):
	global current_signal_index
	current_signal_index = (current_signal_index + direction) % (len(signals)//2-1)
	change_plot(0)

def change_plot(direction):
	global current_plot_index
	current_plot_index = (current_plot_index + direction) % 8
	
	ax.clear()
	ax.set_visible(True)
	if current_plot_index == 0:
	# 	next_signal_button_ax.set_visible(True)
	# 	previous_signal_button_ax.set_visible(True)
	# 	name = titles_signals[2*current_signal_index+1]
	# 	signal = { 'time': signals['time'], name: signals[name] }
	# 	freq = name.split("_")[-1]
	# 	plot_data(ax, signal, 'time', 'linear', title = "Signal du channel 1 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
	# elif current_plot_index == 1:
	# 	next_signal_button_ax.set_visible(True)
	# 	previous_signal_button_ax.set_visible(True)
	# 	name = titles_signals[2*current_signal_index+2]
	# 	signal = { 'time': signals['time'], name: signals[name] }
	# 	freq = name.split("_")[-1]
	# 	plot_data(ax, signal, 'time', 'linear', title = "Signal du channel 2 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
	# if current_plot_index == 2:
	# 	next_signal_button_ax.set_visible(True)
	# 	previous_signal_button_ax.set_visible(True)
	# 	name = titles_phases[2*current_signal_index+1]
	# 	phase = { 'time': phases['time'], name: phases[name] }
	# 	freq = name.split("_")[-1]
	# 	plot_data(ax, phase, 'time_radian', 'linear', title = "phase du channel 1 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
	# elif current_plot_index == 3:
	# 	next_signal_button_ax.set_visible(True)
	# 	previous_signal_button_ax.set_visible(True)
	# 	name = titles_phases[2*current_signal_index+2]
	# 	phase = { 'time': phases['time'], name: phases[name] }
	# 	freq = name.split("_")[-1]
	# 	plot_data(ax, phase, 'time_radian', 'linear', title = "phase du channel 2 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
	# elif current_plot_index == 4:
	# 	name = titles_amplitudes[2*current_signal_index+1]
	# 	amplitude = { 'time': amplitudes['time'], name: amplitudes[name] }
	# 	freq = name.split("_")[-1]
	# 	plot_data(ax, amplitude, 'time', 'linear', title = "Amplitude calculée à partir du signal 1 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
	# elif current_plot_index == 5:
	# 	name = titles_amplitudes[2*current_signal_index+2]
	# 	amplitude = { 'time': amplitudes['time'], name: amplitudes[name] }
	# 	freq = name.split("_")[-1]
	# 	plot_data(ax, amplitude, 'time', 'linear', title = "Amplitude calculée à partir du signal 2 pour un signal généré à "+ freq + " Hz", unit="m", usedColorMap = False)
	# elif current_plot_index == 6:
		next_signal_button_ax.set_visible(False)
		previous_signal_button_ax.set_visible(False)
		amplitude_redpitaya = { 'frequency': amplitudeOfMovement['frequency'], 'Red Pitaya STEMlab 125-14': amplitudeOfMovement['AmplitudeOfMovement'] }
		plot_data(ax, amplitude_redpitaya, 'frequency', 'linear', title = "Affichage de l'amplitude de déplacement", unit="k", usedColorMap = False, calculateModule = False)
		amplitude_zuric = { 'frequency': zuric_signals['frequency'], 'HF2 Zurich HF2LI': zuric_signals['r'] }
		plot_data(ax, amplitude_zuric, 'frequency', 'linear', title = "Affichage de l'amplitude de déplacement", unit="k", usedColorMap = False, calculateModule = False)
	# elif current_plot_index == 7:
	elif current_plot_index == 1:
		phase_redpitaya = { 'frequency': phaseOfMovement['frequency'], 'Red Pitaya STEMlab 125-14': phaseOfMovement['PhaseOfMovement'] }
		plot_data(ax, phase_redpitaya, 'freq_radian', 'linear', title = "Affichage de la phase de déplacement", unit="k", usedColorMap = False, calculateModule = False)
		phase_zuric = { 'frequency': zuric_signals['frequency'], 'HF2 Zurich phase': zuric_signals['phase'] }
		plot_data(ax, phase_zuric, 'freq_radian', 'linear', title = "Affichage de la phase de déplacement", unit="k", usedColorMap = False, calculateModule = False)
	elif current_plot_index == 2:
		x = amplitudeOfMovement["AmplitudeOfMovement"] * np.cos(phaseOfMovement["PhaseOfMovement"])
		y = amplitudeOfMovement["AmplitudeOfMovement"] * np.sin(phaseOfMovement["PhaseOfMovement"])
		plot_data(ax, {'frequency':amplitudeOfMovement['frequency'], 'x': x, 'y': y}, 'frequency', 'linear', title = "Affichage de X et Y", unit="k", usedColorMap = False, calculateModule = False)
	plt.tight_layout()
	plt.draw()  # Rafraîchir l'écran

directory = 'data/2024-07-26_17:35:36'

# filename1  = directory + '/signals.csv'
# filename2  = directory + '/amplitudes.csv'
# filename2b = directory + '/phases.csv'
filename3  = directory + '/amplitudeOfMovement.csv'
filename4  = directory + '/phaseOfMovement.csv'

filename5  = 'data/zuric.csv'

# Lecture des fichiers CSV
# signals, titles_signals = read_csv(filename1)
# amplitudes, titles_amplitudes = read_csv(filename2)
# phases, titles_phases = read_csv(filename2b)
amplitudeOfMovement, _ = read_csv(filename3)
phaseOfMovement, _ = read_csv(filename4)

zuric_signals, titles_signals = read_csv(filename5)

# Création de la figure et des sous-graphiques
fig, ax = plt.subplots(figsize=(16/1.2, 9/1.8))
gs = fig.add_gridspec(2, hspace=0.3)

# Ajout du bouton pour basculer entre les affichages des signaux et des FFT
previous_button_ax = plt.axes([0.005, 0.96, 0.025, 0.035])
previous_button = Button(previous_button_ax, '<<')
previous_button.on_clicked(previous_plot)
next_button_ax = plt.axes([0.97, 0.96, 0.025, 0.035])
next_button = Button(next_button_ax, '>>')
next_button.on_clicked(next_plot)
previous_signal_button_ax = plt.axes([0.035, 0.96, 0.025, 0.035])
previous_signal_button = Button(previous_signal_button_ax, '-')
previous_signal_button.on_clicked(previous_signal)
next_signal_button_ax = plt.axes([0.94, 0.96, 0.025, 0.035])
next_signal_button = Button(next_signal_button_ax, '+')
next_signal_button.on_clicked(next_signal)

# Affichage initial des signaux
current_signal_index = 0
current_plot_index = 0
change_plot(0)

plt.tight_layout()
plt.show()
