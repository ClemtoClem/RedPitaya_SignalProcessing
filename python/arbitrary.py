import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import write
from math import gcd
from functools import reduce

# Fonction pour calculer le PGCD de plusieurs nombres
def gcd_multiple(numbers):
	return reduce(gcd, numbers)

# Paramètres des sinusoïdes
amplitudes = [1.0, 0.5, 0.3]  # Amplitudes des sinusoïdes
frequencies = [12000, 13100, 1760] # Fréquences des sinusoïdes en Hz

# Trouver la période fondamentale
sampling_rate = 125e6  # Taux d'échantillonnage en Hz
fundamental_frequency = gcd_multiple(frequencies)  # Fréquence fondamentale
period = 1.0 / fundamental_frequency  # Période fondamentale en secondes

# Créer le vecteur de temps pour une période complète
t = np.linspace(0, period, int(sampling_rate * period), endpoint=False)

# Initialiser le signal
signal = np.zeros_like(t)

# Somme des sinusoïdes
for amplitude, frequency in zip(amplitudes, frequencies):
	signal += amplitude * np.sin(2 * np.pi * frequency * t)

# Normaliser le signal pour éviter la saturation (si nécessaire)
max_amplitude = np.max(np.abs(signal))
if max_amplitude > 1.0:
	signal = signal / max_amplitude

# Afficher le signal
plt.plot(t, signal)
plt.xlabel('Temps (s)')
plt.ylabel('Amplitude')
plt.title('Somme de n sinus - Période complète')
plt.show()

# Sauvegarder le signal en fichier audio (optionnel)
write('somme_sinus_periode_complete.wav', sampling_rate, signal.astype(np.float32))
