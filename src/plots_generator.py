import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results_k.csv")

plt.plot(df["k"], df["avg_time_ms"], marker="o")
plt.xlabel("Liczba kopii k")
plt.ylabel("Średni czas [ms]")
plt.title("Zależność czasu od liczby kopii k")
plt.grid(True)
plt.show()
