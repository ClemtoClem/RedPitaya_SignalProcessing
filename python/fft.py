import numpy as np
import matplotlib.pyplot as plt
from python.display import *

def inversion_bits(i,p): # p = nombre de bits
	c = "{0:0%db}"%p
	binaire = c.format(i)
	inv = binaire[::-1]
	return int(inv,2)

def fft(u,p):
	N=len(u)
	if N!=2**p:
		print("Erreur de taille")
		return
	A = np.zeros(N,dtype=np.complex64)
	B = np.zeros(N,dtype=np.complex64)
	for k in range(N):
		j = inversion_bits(k,p)
		A[j] = u[k]
	for q in range(1,p+1): # FFT à 2**q éléments
		taille = 2**q
		taille_precedente = 2**(q-1)
		nombre_tfd = 2**(p-q) # nombre de TFD à calculer
		phi = -1j*2*np.pi/taille
		for m in range(nombre_tfd):
			position = m*taille
			for i in range(taille_precedente):
				W = np.exp(phi*i)
				B[position+i] = A[position+i] + W*A[position+taille_precedente+i]
			for i in range(taille_precedente,taille):
				W = np.exp(phi*i)
				B[position+i] = A[position+i-taille_precedente] + W*A[position+i]
		(A,B)=(B,A) # échange des références des tableaux
	# normalise
	for k in range(N):
		A[k] = 2.0*np.abs(A[k])/N
	return A

if __name__ == "__main__":
	#fichier_csv = "./data/test.csv"

	# Lecture des données à partir du fichier CSV
	#signals: dict = read_csv(fichier_csv)

	#time = signals["time"]
	#signal = signals["signal(t)"]

	dt = 1/125e6
	N = 16384
	time = np.arange(0, N*dt, dt)
	freq = np.fft.fftfreq(N,dt)

	signal1 = 0.8*np.sin(2*np.pi*10000*time) + 0.2*np.sin(2*np.pi*10100*time)
	signal2 = 0.8*np.sin(2*np.pi*10000*time) + 0.2*np.sin(2*np.pi*20000*time)

	# N = len(signal)
	# P = np.int32(np.log2(N))
	# fs = 1/time[1]
	# freq = []
	# for k in range(N):
	# 	freq.append(fs*k/N)

	spectre1 = np.fft.fft(signal1)
	spectre2 = np.fft.fft(signal2)
			
	plt.figure(figsize=(16/1.5, 9/1.8))
	plt.loglog(freq, 2.0/N * np.abs(spectre1))
	plt.loglog(freq, 2.0/N * np.abs(spectre2))
	plt.show()