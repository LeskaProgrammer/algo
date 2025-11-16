import numpy as np
import matplotlib.pyplot as plt

N = []
S_wide = []
S_narrow = []
rel_err_wide = []
rel_err_narrow = []

with open("data.txt", encoding="utf-8") as f:
    next(f)
    for line in f:
        parts = line.split()
        if not parts:
            continue
        N.append(int(parts[0]))
        S_wide.append(float(parts[1]))
        S_narrow.append(float(parts[2]))
        rel_err_wide.append(float(parts[3]))
        rel_err_narrow.append(float(parts[4]))

S_exact = 0.25 * np.pi + 1.25 * np.arcsin(0.8) - 1.0

plt.figure(figsize=(12, 6))
plt.plot(N, S_wide, label="Широкий прямоугольник")
plt.plot(N, S_narrow, label="Узкий прямоугольник")
plt.axhline(S_exact, linestyle="--", label="Точная площадь (≈0.944)")
plt.xlabel("Число точек N")
plt.ylabel("Оценка площади пересечения")
plt.title("Оценка площади пересечения трёх окружностей")
plt.legend()
plt.grid(True)

plt.figure(figsize=(12, 6))
plt.plot(N, rel_err_wide, label="Широкий прямоугольник")
plt.plot(N, rel_err_narrow, label="Узкий прямоугольник")
plt.xlabel("Число точек N")
plt.ylabel("Относительная ошибка (S_approx - S_exact) / S_exact")
plt.title("Относительная ошибка метода Монте-Карло")
plt.legend()
plt.grid(True)

plt.show()
