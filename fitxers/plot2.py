import numpy as np
import matplotlib.pyplot as plt

fm = 200/3

# Leer el archivo de la señal
signal_file = 'res_x.txt'
signal = np.loadtxt(signal_file)

# Leer el archivo de la autocorrelación
autocorr_file = 'res_r.txt'
autocorr = np.loadtxt(autocorr_file)

# Definir el eje de tiempo
t = np.arange(len(signal)) / fm  

# Definir el periodo de pitch
period = np.argmax(autocorr[50:100]) + 50  # Buscar el máximo entre las muestras 50 y 100
pitch_period = period / fm  # Convertir a tiempo en segundos

# Crear el subplot
fig, axs = plt.subplots(2, 1)

# Plot de la señal y el pitch period
axs[0].plot(t, signal, label='Señal sonora')
axs[0].set_xlabel('Tiempo')
axs[0].set_xlim(0,3)
axs[0].set_ylabel('Amplitud')
axs[0].legend()

# Plot de la autocorrelación y el primer máximo secundario
axs[1].plot(t[:len(autocorr)], autocorr, label='Autocorrelación')
axs[1].axvline(x=t[50 + np.argmax(autocorr[50:100])], color='r', linestyle='--', label='Primer máximo secundario')
axs[1].set_xlabel('Tiempo')
axs[1].set_xlim(0,3)
axs[1].set_ylabel('Autocorrelación')
axs[1].legend()

# Mostrar el gráfico
plt.show()