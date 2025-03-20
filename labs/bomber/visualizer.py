import numpy as np
import matplotlib.pyplot as plt
from scipy.special import comb

def count_visits(n):
    grid = np.zeros((n+1, n+1), dtype=int)
    for r in range(n+1):
        for c in range(n+1):
            grid[r, c] = comb(r + c, r, exact=True) * comb(2*n - r - c, n - r, exact=True)
    return grid

def plot_heatmap(n):
    n = n - 1
    visits = count_visits(n)
    
    plt.figure(figsize=(8, 6))
    plt.imshow(visits, cmap='hot', interpolation='nearest')
    plt.colorbar(label="Times Walked On")
    
    for r in range(n+1):
        for c in range(n+1):
            plt.text(c, r, str(visits[r, c]), ha='center', va='center', color='blue')

    plt.xticks(range(n+1))
    plt.yticks(range(n+1))
    plt.title(f"Path Visit Frequency Heatmap for {n+1}x{n+1} Grid")
    plt.xlabel("Column")
    plt.ylabel("Row")
    plt.show()




# Change the grid size as needed
plot_heatmap(5)
