import pandas as pd
import matplotlib.pyplot as plt

# Зчитування CSV
df = pd.read_csv("experiment_results.csv")  # твій файл

# Побудова графіка
plt.figure(figsize=(8, 5))

for structure in df['structure'].unique():
    subset = df[df['structure'] == structure]
    plt.plot(subset['size'], subset['total_ops'],
             marker='o', label=structure)

plt.xscale('log')  # щоб осі 100–100000 виглядали рівномірно
plt.xticks([100, 1000, 10000, 100000])
plt.xlabel("Кількість студентів")
plt.ylabel("Кількість операцій за 10 секунд")
plt.title("Порівняння швидкодії структур даних")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
