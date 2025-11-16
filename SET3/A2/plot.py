import pandas as pd
import matplotlib.pyplot as plt

path = "data.txt"

with open(path, "r", encoding="utf-8") as f:
    header = f.readline().lstrip("#").strip().split()

df = pd.read_csv(path, delim_whitespace=True, comment="#", header=None)
df.columns = header

type_names = {
    0: "Случайные массивы",
    1: "Обратно отсортированные массивы",
    2: "Почти отсортированные массивы",
}

for t in sorted(df["type"].unique()):
    sub = df[df["type"] == t]

    plt.figure(figsize=(10, 6))
    plt.plot(sub["size"], sub["merge"], label="MergeSort")
    plt.plot(sub["size"], sub["hybrid_5"], label="Hybrid, t=5")
    plt.plot(sub["size"], sub["hybrid_10"], label="Hybrid, t=10")
    plt.plot(sub["size"], sub["hybrid_20"], label="Hybrid, t=20")
    plt.plot(sub["size"], sub["hybrid_30"], label="Hybrid, t=30")
    plt.plot(sub["size"], sub["hybrid_50"], label="Hybrid, t=50")

    plt.xlabel("Размер массива")
    plt.ylabel("Время, мкс (усреднённое)")
    plt.title(type_names.get(t, f"type={t}"))
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()
