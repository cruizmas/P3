import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf

data, samplerate = sf.read("./fonema.wav")
t = np.arange(0, len(data))/samplerate

corr = np.correlate(data, data, 'full') / len(data)
corr = corr[int(corr.size/2):]

min_index = np.argmin(corr)
max_index = np.argmax(corr[min_index:])
max_value = np.max(corr[min_index:])
fig, axs = plt.subplots(2)

axs[0].plot(t, data)
axs[1].plot(t, corr)
axs[1].plot((min_index+max_index)*1000/samplerate, max_value, 'ro', label='Temporal index = {}ms'.format((min_index+max_index)*1000/samplerate)) #MOSTRAR EL MÁXIMO DE LA AUTOCORRELACIÓN

axs[0].set_title('Voiced frame')
axs[1].set_title('Voiced frame autocorrelation')
axs[1].set_xlabel('time (ms)')
axs[1].legend()

fig.tight_layout()
plt.show()