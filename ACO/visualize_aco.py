import matplotlib.pyplot as plt
import numpy as np

iterations = []
best_lengths = []
current_lengths = []
pheromones = []

with open('aco_data.txt', 'r') as f:
    next(f)  
    for line in f:
        parts = line.strip().split()
        if len(parts) == 4:
            iterations.append(int(parts[0]))
            best_lengths.append(float(parts[1]))
            current_lengths.append(float(parts[2]))
            pheromones.append(float(parts[3]))

window_size = 30
averaged_iterations = []
averaged_lengths = []

for i in range(0, len(current_lengths), window_size):
    window = current_lengths[i:i+window_size]
    valid = [x for x in window if x != float('inf')]
    
    if valid:
        avg_idx = i + len(valid)//2
        if avg_idx < len(iterations):
            averaged_iterations.append(iterations[avg_idx])
            averaged_lengths.append(np.mean(valid))

fig, ax1 = plt.subplots(figsize=(14, 8))


ax1.set_xlabel('Итерации', fontsize=12, fontweight='bold')
ax1.set_ylabel('Длина пути', fontsize=12, fontweight='bold', color='red')

ax1.plot(iterations, best_lengths, 'r-', label='Лучший путь')

ax1.plot(averaged_iterations, averaged_lengths, 'go-', linewidth=2, 
         markersize=5, label=f'Средний путь (окно {window_size})')

ax1.fill_between(iterations, current_lengths, alpha=0.15, color='green')

ax1.tick_params(axis='y', labelcolor='red')
ax1.legend(loc='upper left', fontsize=11)
ax1.grid(True, alpha=0.3)

ax2 = ax1.twinx()
ax2.set_ylabel('Уровень феромонов', fontsize=12, fontweight='bold', color='blue')
ax2.plot(iterations, pheromones, 'b-', linewidth=2, alpha=0.7, label='Феромоны')
ax2.tick_params(axis='y', labelcolor='blue')
ax2.legend(loc='upper right', fontsize=11)

if averaged_lengths:
    stats = f'Лучший: {best_lengths[-1]:.2f}\n' \
            f'Средний: {averaged_lengths[-1]:.2f}\n' \
            f'Итераций: {len(iterations)}'
    ax1.text(0.02, 0.98, stats, transform=ax1.transAxes, 
             verticalalignment='top', fontsize=10,
             bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

plt.show()