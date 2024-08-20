#!/usr/bin/env python3

import numpy as np
from matplotlib import pyplot as plt

N = 16384  # Number of samples
fs = 125e6/16  # Sampling frequency in Hz
f1 = 11.3e3
a1 = 0.8
f2 = 12e3
a2 = 0.2
dt = (1/fs)

repeat = 8
t = np.zeros(repeat * N)
x = np.zeros(repeat * N)

# Create the base signal for one period
for i in range(N):
	t[i] = i /N
	x[i] = a1 * np.sin(2 * np.pi * t[i]) + a2 * np.sin(2 * np.pi * f2/f1 * t[i])

# Repeat the signal
for j in range(0, repeat):
	for i in range(N):
		k = i + j * N
		t[k] = k * dt / f1
		x[k] = x[i]

# Plot the signal
fig, ax = plt.subplots(1, 1, sharex=True, figsize=(16/1.2, 9/1.8))
ax.plot(t, x)
ax.set_title('Custom waveform')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Amplitude (V)')
plt.show()
