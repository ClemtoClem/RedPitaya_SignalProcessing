import numpy as np
import matplotlib.pyplot as plt

# Paramètres du signal sinusoidal
decimation = 1
N = 16384
Fs = 125e6/decimation  # Fréquence d'échantillonnage en Hz
T = N/Fs	   # Durée du signal en secondes
f1 = 10e3	 # Fréquence du signal en Hz
f2 = 10e3+100	 # Fréquence du signal en Hz
f3 = 20e3

t = np.arange(0, T, 1/Fs)  # Vecteur temps

# Générer le signal sinusoidal
signal1 = 0.8 * np.sin(2 * np.pi * f1 * t) + 0.2 * np.sin(2 * np.pi * f2 * t)
signal2 = 0.8 * np.sin(2 * np.pi * f1 * t) + 0.2 * np.sin(2 * np.pi * f3 * t)

# Calcul de la transformée de Fourier (FFT)
fft_signal1 = np.fft.fft(signal1)
fft_signal2 = np.fft.fft(signal2)
N = len(fft_signal1)
freq = np.fft.fftfreq(N, d=1/Fs)  # Vecteur fréquence

# Sélectionner la moitié positive du spectre FFT (fréquences non négatives)
freq_pos1 = freq[:N//32]
fft_signal_pos1 = 2.0/N * np.abs(fft_signal1[:N//32])
freq_pos2 = freq[:N//32]
fft_signal_pos2 = 2.0/N * np.abs(fft_signal2[:N//32])

# Affichage du signal temporel
# plt.figure(figsize=(16/1.5, 9/1.8))

# plt.subplot(3, 1, 1)
# plt.plot(t, signal)

# plt.subplot(3, 1, 2)
# plt.plot(t, signal2)

# plt.subplot(3, 1, 3)
# plt.plot(t, signal)
# plt.plot(t, signal2)
# plt.title('Signal Sinusoidal')
# plt.xlabel('Temps (s)')
# plt.ylabel('Amplitude (V)')

# Affichage du spectre de fréquence (FFT)
#plt.subplot(2, 1, 2)
plt.loglog(freq_pos1, fft_signal_pos1)
plt.loglog(freq_pos2, fft_signal_pos2)
plt.title('Transformée de Fourier (FFT)')
plt.xlabel('Fréquence (Hz)')
plt.ylabel('Amplitude (V)')

plt.tight_layout()

plt.show()
