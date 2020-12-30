import csv
from kmeas import kmeans
import numpy as np
import matplotlib.pyplot as plt

K:int = 3

if __name__ == '__main__':
    with open("WineData.csv") as f:
        f_csv = csv.reader(f)
        wineData = list(f_csv)
        data = np.array([i[1:] for i in wineData], dtype=np.float)

        a = kmeans(data, K=K)
        a.run()
        clusters = a.clusters

        # 根据给出的聚类计算准确度
        right, total = 0, 0
        for i in range(K):
            for id in clusters[i]:
                if int(wineData[id][0]) == i + 1:
                    right += 1
                total += 1
        print("accuracy:", right / total)

        SSE: float = 0
        centroids = a.centroids
        for item in zip(centroids, np.array([data[cluster] for cluster in clusters])):
            SSE += np.sum((item[1] - item[0]) ** 2)
        print("SSE:", SSE)

        # show plot
        styles = ["ro", "go", "bo"]
        for i in range(K):
            plt.plot(data[clusters[i]][:, 5], data[clusters[i]][:, 6],
                     styles[i])
        plt.show()
