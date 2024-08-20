#!/usr/bin/env python3

import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

def generate_perlin_noise(size, scale=10):
	def fade(t):
		return 6 * t**5 - 15 * t**4 + 10 * t**3
	
	def lerp(a, b, t):
		return a + t * (b - a)
	
	def gradient(h, x, y):
		vectors = np.array([[0, 1], [0, -1], [1, 0], [-1, 0]])
		g = vectors[h % 4]
		return g[:, :, 0] * x + g[:, :, 1] * y
	
	lin = np.linspace(0, scale, size, endpoint=False)
	x, y = np.meshgrid(lin, lin)
	
	p = np.arange(256, dtype=int)
	np.random.shuffle(p)
	p = np.stack([p, p]).flatten()
	
	xi = x.astype(int) & 255
	yi = y.astype(int) & 255
	xf = x - xi
	yf = y - yi
	
	u = fade(xf)
	v = fade(yf)
	
	n00 = gradient(p[p[xi] + yi], xf, yf)
	n01 = gradient(p[p[xi] + yi + 1], xf, yf - 1)
	n10 = gradient(p[p[xi + 1] + yi], xf - 1, yf)
	n11 = gradient(p[p[xi + 1] + yi + 1], xf - 1, yf - 1)
	
	x1 = lerp(n00, n10, u)
	x2 = lerp(n01, n11, u)
	return lerp(x1, x2, v)

def generate_combined_sinusoidal(linespace, num_waves=10):
	combined_wave = np.zeros(linespace.shape)
	for _ in range(num_waves):
		amplitude = np.random.uniform(0.5, 1.5)
		frequency = np.random.uniform(0.5, 2.0)
		phase = np.random.uniform(0, 2 * np.pi)
		combined_wave += amplitude * np.sin(frequency * linespace + phase)
	return combined_wave

N = 200 # Number of samples

fs = 400 # Sampling frequency in Hz

decimation1 = 128
t1 = np.linspace(0, N/(fs/decimation1), N)
f1 = 10
#x1 = np.sin(2*np.pi*f1*t1)

x1 = generate_combined_sinusoidal(t1, num_waves=5)

decimation2 = 2
t2 = np.linspace(0, N/(fs/decimation2), N)
f2 = 10
x2 = np.sin(2*np.pi*f2*t2)

#fig, ax = plt.subplots(2, 1, sharex=True, figsize=(16/2, 9/2))

plt.subplot(2, 1, 1)
plt.stem(t1, x1)

# Ajuster la taille de la police des numérotations des axes
plt.tick_params(axis='both', which='major', labelsize=16)
plt.tick_params(axis='both', which='minor', labelsize=16)
plt.title(f'Signal généré avec une décimation de {decimation1}', fontsize=16)
plt.xlabel('Temps (s)', fontsize=16)

plt.subplot(2, 1, 2)
plt.stem(t2, x2)

# Ajuster la taille de la police des numérotations des axes
plt.tick_params(axis='both', which='major', labelsize=16)
plt.tick_params(axis='both', which='minor', labelsize=16)
plt.title(f'Signal généré avec une décimation de {decimation2}', fontsize=16)
plt.xlabel('Temps (s)', fontsize=16)

plt.tight_layout()
plt.show()
