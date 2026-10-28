import matplotlib.pyplot as plt
import numpy as np

hilos = [1, 2, 4, 6, 8, 12, 16]

tiempo_pi_sec = 5.621186
tiempos_pi = [5.715296, 3.043997, 1.517046, 0.971561, 0.780936, 0.670230, 0.669008]

tiempo_mv_sec = 0.021365
tiempos_mv = [0.022427, 0.012132, 0.006316, 0.005426, 0.004549, 0.005009, 0.004449]

lista_hilos = [4, 8]
lista_ss_tiempos = [0.002584, 0.005323]
lista_cs_tiempos = [0.005948, 0.017386]
lista_ss_nodos_perdidos = [2450, 7977]
lista_cs_nodos_perdidos = [0, 0]


# CALCULOS
speedup_pi = [tiempo_pi_sec / t for t in tiempos_pi]
efi_pi = [s / h for s, h in zip(speedup_pi, hilos)]

speedup_mv = [tiempo_mv_sec / t for t in tiempos_mv]
efi_mv = [s / h for s, h in zip(speedup_mv, hilos)]

# ==========================================
# FIGURA 1 RENDIMIENTO PI Y MATRIZ-VECTOR

fig1, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 5))

# Speedup
ax1.plot(hilos, speedup_pi, marker='o', color='blue', label='Cálculo de Pi')
ax1.plot(hilos, speedup_mv, marker='s', color='green', label='Matriz-Vector')
ax1.plot(hilos, hilos, linestyle='--', color='red', label='Ideal (Lineal)')
ax1.set_title('Speedup vs Hilos')
ax1.set_xlabel('Número de Hilos')
ax1.set_ylabel('Speedup')
ax1.set_xticks(hilos)
ax1.legend()
ax1.grid(True, linestyle='--', alpha=0.7)

# Eficiencia
ax2.plot(hilos, efi_pi, marker='o', color='blue', label='Cálculo de Pi')
ax2.plot(hilos, efi_mv, marker='s', color='green', label='Matriz-Vector')
ax2.set_title('Eficiencia vs Hilos')
ax2.set_xlabel('Número de Hilos')
ax2.set_ylabel('Eficiencia')
ax2.set_ylim(0, 1.1)
ax2.set_xticks(hilos)
ax2.legend()
ax2.grid(True, linestyle='--', alpha=0.7)

# Tiempos
ax3.plot(hilos, tiempos_pi, marker='o', color='blue', label='Pi (Pthreads)')
ax3.axhline(y=tiempo_pi_sec, color='blue', linestyle='--', alpha=0.5, label='Pi Secuencial')
ax3.set_title('Tiempo de Ejecución vs Hilos')
ax3.set_xlabel('Número de Hilos')
ax3.set_ylabel('Tiempo Pi (segundos)', color='blue')
ax3.tick_params(axis='y', labelcolor='blue')
ax3.set_xticks(hilos)

ax3b = ax3.twinx()  
ax3b.plot(hilos, tiempos_mv, marker='s', color='green', label='Matriz-Vector (Pthreads)')
ax3b.axhline(y=tiempo_mv_sec, color='green', linestyle='--', alpha=0.5, label='MV Secuencial')
ax3b.set_ylabel('Tiempo Matriz-Vector (segundos)', color='green')
ax3b.tick_params(axis='y', labelcolor='green')

fig1.tight_layout()
fig1.savefig('rendimiento_pi_mv.png', dpi=300)

# ==========================================
# LISTA ENLAZADA

fig2, (ax4, ax5) = plt.subplots(1, 2, figsize=(12, 5))

x = np.arange(len(lista_hilos))
width = 0.35

# Tiempos de ejecución
rects1 = ax4.bar(x - width/2, lista_ss_tiempos, width, label='Sin Sync (RACE)', color='red')
rects2 = ax4.bar(x + width/2, lista_cs_tiempos, width, label='Con Sync (Mutex)', color='green')
ax4.set_ylabel('Tiempo (segundos)')
ax4.set_title('Lista Enlazada: Impacto de la Sincronización')
ax4.set_xticks(x)
ax4.set_xticklabels([f'{h} Hilos' for h in lista_hilos])
ax4.legend()
ax4.grid(axis='y', linestyle='--', alpha=0.7)

# Nodos Perdidos
ax5.bar(x - width/2, lista_ss_nodos_perdidos, width, label='Sin Sync (Nodos Perdidos)', color='red')
ax5.bar(x + width/2, lista_cs_nodos_perdidos, width, label='Con Sync (0 Perdidos)', color='green')
ax5.set_ylabel('Cantidad de Nodos')
ax5.set_title('Condición de Carrera en la Lista Enlazada')
ax5.set_xticks(x)
ax5.set_xticklabels([f'{h} Hilos' for h in lista_hilos])
ax5.legend()
ax5.grid(axis='y', linestyle='--', alpha=0.7)

fig2.tight_layout()
fig2.savefig('resultados_lista.png', dpi=300)

print("Gráficos guardados como 'rendimiento_pi_mv.png' y 'resultados_lista.png'")