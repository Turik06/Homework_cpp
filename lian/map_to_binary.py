import matplotlib.pyplot as plt
import numpy as np
import cv2
from scipy.ndimage import binary_fill_holes, binary_dilation


img = cv2.imread('map.bmp')

if img is None:
    print("Ошибка: не удалось загрузить изображение 'map.bmp'. Убедитесь, что файл находится в той же папке, что и скрипт.")
else:
    
    mask = np.all(img == [0, 0, 0], axis=-1)
    binary = mask.astype(np.uint8)

    filled = binary_fill_holes(binary).astype(np.uint8)

   
    structure = np.ones((5, 5), dtype=np.uint8)
    dilated = binary_dilation(filled, structure=structure, iterations=2).astype(np.uint8)

    
    plt.imshow(dilated, cmap='gray')
    plt.show()

   
    h, w = dilated.shape
    with open('binary_map.txt', 'w') as f:
        f.write(f"{h} {w}\n")
        np.savetxt(f, dilated, fmt='%d')
    
    print("Обработка завершена. Карта сохранена в 'binary_map.txt'")