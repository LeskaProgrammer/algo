import numpy as np
import matplotlib.pyplot as plt

data = []
with open("data_quick.txt", "r", encoding="utf-8") as f:
    for line in f:
        if not line.strip() or line.startswith("#"):
            continue
        t, n, q, h = line.split()
        data.append((int(t), int(n), float(q), float(h)))

data = np.array(data)
types = [0, 1, 2]
titles = {
    0: "Случайные массивы",
    1: "Обратно отсортированные массивы",
    2: "Почти отсортированные массивы",
}

for t in types:
    mask = data[:, 0] == t
    n = data[mask, 1]
    quick = data[mask, 2]
    intro = data[mask, 3]

    plt.figure(figsize=(10, 5))
    plt.plot(n, quick, label="QUICK SORT (классический)")
    plt.plot(n, intro, label="INTROSORT (QUICK+HEAP+INSERTION)")
    plt.xlabel("Размер массива N")
    plt.ylabel("Время, мкс")
    plt.title(titles[t])
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()

plt.show()
