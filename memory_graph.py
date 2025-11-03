import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("memory_usage.csv")

plt.figure(figsize=(8,6))
for structure in df["structure"].unique():
    subset = df[df["structure"] == structure]
    plt.plot(subset["size"], subset["memory_bytes"], marker='o', label=structure)

plt.title("Memory usage vs Data size")
plt.xlabel("Data size (students)")
plt.ylabel("Memory usage (bytes)")
plt.legend()
plt.grid(True)
plt.show()
