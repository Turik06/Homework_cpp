import cv2
import numpy as np
import matplotlib.pyplot as plt
import sys
import os
from scipy.interpolate import splprep, splev

def load_map(file):
    with open(file, 'r') as f:
        lines = f.readlines()
        h, w = map(int, lines[0].split())
        grid = [list(map(int, lines[i].split())) for i in range(1, h + 1)]
    return np.array(grid), h, w

def load_path(file):
    if not os.path.exists(file):
        return []
    path = []
    with open(file, 'r') as f:
        for line in f:
            if line.strip():
                x, y = map(int, line.strip().split()[:2])
                path.append((x, y))
    return path

def smooth_path(path, smoothness=3):
    """Сглаживает путь используя B-сплайны"""
    if len(path) < 4:
        return path
    
    # Извлекаем координаты
    x = [p[0] for p in path]
    y = [p[1] for p in path]
    
    # Создаем параметрическое представление
    try:
        # k=3 для кубического сплайна, s - степень сглаживания
        tck, u = splprep([x, y], s=smoothness, k=min(3, len(path)-1))
        
        # Генерируем больше точек для плавной кривой
        u_new = np.linspace(0, 1, len(path) * 10)
        x_new, y_new = splev(u_new, tck)
        
        # Возвращаем сглаженный путь
        return [(int(x_new[i]), int(y_new[i])) for i in range(len(x_new))]
    except:
        # Если сглаживание не удалось, возвращаем оригинальный путь
        return path

def visualize(map_file, path_file):
    grid, h, w = load_map(map_file)
    path = load_path(path_file)
    
    img = np.ones((h, w, 3))
    img[grid == 1] = [0, 0, 0]
    
    if path:
        # Сглаживаем путь
        smooth = smooth_path(path, smoothness=50)  # Увеличьте значение для большего сглаживания
        
        # Рисуем оригинальный путь тонкой серой линией
        for i in range(len(path)-1):
            cv2.line(img, path[i], path[i+1], (0.5, 0.5, 0.5), 1)
        
        # Рисуем сглаженный путь синей линией
        for i in range(len(smooth)-1):
            cv2.line(img, smooth[i], smooth[i+1], (0, 0, 1), 2)
        
        # Точки пути (оригинальные)
        for p in path:
            cv2.circle(img, p, 3, (1, 1, 0), -1)  # Желтые точки
        
        cv2.circle(img, path[0], 5, (0, 1, 0), -1)
        cv2.circle(img, path[-1], 5, (1, 0, 0), -1)
        
    # Показываем только одну карту без масштабирования
    plt.figure(figsize=(16, 10))
    plt.imshow(img)
    plt.title('Карта с путём')
    plt.axis('off')
    
    plt.tight_layout()
    plt.savefig('result.png', dpi=150, bbox_inches='tight')
    print(f"Сохранено: result.png")
    
    if path:
        length = sum(np.sqrt((path[i][0]-path[i-1][0])**2 + (path[i][1]-path[i-1][1])**2) 
                     for i in range(1, len(path)))
        print(f"Точек: {len(path)}, Длина: {length:.2f}")
    
    plt.show()

if __name__ == "__main__":
    map_file = sys.argv[1] if len(sys.argv) > 1 else "binary_map.txt"
    path_file = sys.argv[2] if len(sys.argv) > 2 else "path_result.txt"
    visualize(map_file, path_file)