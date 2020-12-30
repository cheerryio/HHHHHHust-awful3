from typing import List, NoReturn
import numpy as np

"""
kmeans算法实现: 
    1. 随机选取K个centroid
    2. assignment函数将点映射到各个centroid
    3. update函数通过平均数计算每一个cluster新的centroid

@param data: 2-d numpy array
@param K: number of clusters，default to 8
@param maxIter: max iteration number for single round of cluster，default to 300
@param tol: shift tolerance，default to 1e-4
"""


class kmeans():
    def __init__(self, data: np.ndarray, K: int = 8, maxIter: int = 300, tol: float = 1e-4):
        if not isinstance(data, np.ndarray):
            data = np.array(data, dtype=np.float)
        self.data: np.ndarray = data.reshape((data.shape[0], -1))
        self.K: int = K
        self.maxIter: int = maxIter
        self.tol: float = tol
        self.clusters: List[List] = [[0] * K]

    def initCentroids(self) -> NoReturn:
        """
        初始化最初的centroids

        :return:
        """
        self.centroids = np.array([self.data[i] for i in [0, 1, 2]], dtype=np.float)

    def assignment(self) -> NoReturn:
        self.lastClusters = self.clusters
        self.clusters = []
        for i in range(self.K):
            self.clusters.append([])
        for i in range(self.data.shape[0]):
            distances: np.ndarray = np.sum((self.centroids - self.data[i]) ** 2, axis=1)
            nearestCentroid: int = np.argsort(distances)[0]
            self.clusters[nearestCentroid].append(i)

    def update(self) -> NoReturn:
        self.lastCentroids = self.centroids
        centroids = []
        for i in range(self.K):
            average = np.average([self.data[m] for m in self.clusters[i]], axis=0)
            if average[np.isnan(average)]:
                average = np.array([1] * self.data.shape[-1], dtype=np.float)
            centroids.append(average)
        self.centroids = np.array(centroids, dtype=np.float)

    def run(self) -> NoReturn:
        self.initCentroids()
        self.assignment()
        self.update()
        iterRound: int = 1
        while iterRound < self.maxIter:
            self.assignment()
            self.update()
            if np.array_equal(self.lastClusters, self.clusters):
                print("lastClusters = clusters convergent")
                break
            if np.sum((self.lastCentroids - self.centroids) ** 2) < self.tol:
                print("tol convergent")
                break
            iterRound += 1
