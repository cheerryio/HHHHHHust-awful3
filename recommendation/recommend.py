import numpy as np
from numpy.linalg import norm
from typing import List, Sequence, Dict, Tuple, NoReturn, Callable, Optional
from math import log

"""
基于内容的推荐算法的实现的实现:
data: List[Tuple[int,int,float]] 是 (userId,movieId,rating) 的列表。用户对某个电影的评分
moviesData: List[Tuple[int,List[str]]] 是 (movieId,[keywords]) 电影由哪些关键字标识

1. 生成ratingM矩阵 行表示userId, 列表示movieId，元素是用户对电影的评分
2. 计算tf-idf特征矩阵。
    计算idfi（逆向文件频率，inverse document frequency）：log(D/包含词语ti的文件数目)
    计算tfi,j（词频，term frequency）：在文件j中词语i出现的频率
    计算tfidfi,j：tfi,j * idfi
    
    具体实现:
    计算idfi的映射。D（电影总数直接得到）。对电影的所有feature进行遍历，由于默认假定数据集中
        同一个电影不会给出相同的feature，所有在计算时，Dict[feature] += 1 key是feature，value是
        包含feature的电影数目
    计算tfi,j：对一条电影的记录，词频是 1/该电影feature总数
    计算tfidf特征矩阵：矩阵每一行是一个电影，每一列是一个feature。
3. 计算m矩阵。我把m矩阵作为最后计算电影之间相似度使用的矩阵。
    当使用brute_force方法（原始方法），tdidf特征矩阵就是m
    当使用minhash算法，先对tdidf特征矩阵进行01化处理。如果该电影存在某feature，则tdidf矩阵中该feature位为1，否则为0
        利用n个哈希函数，生成 (self.movieN+1, n) 的m矩阵。minhash算法将feature的维度从原来变成了n维
4. 给定userId, movieId 计算用户对该电影的评分。如果用户对该电影评分过，返回评分
    找到该用户所有评分过的电影id，计算movieId和这些电影之间的余弦相似度。利用余弦相似度大于0的电影去计算用户对该电影
    的评分。
5. 为用户推荐电影。给定用户，针对该用户，对所有电影利用推荐算法进行打分。选取用户自身没有评分过并且评分高的前k个电影
    进行推荐。
    
"""


class contentRecommend:
    def __init__(self, data: Sequence[Tuple[int, int, float]], moviesData: Sequence[Tuple[int, List[str]]],
                 strategy: str = "brute_force", minhashDim: int = 10):
        self.data: List[Tuple[int, int, float]] = data
        self.moviesData = moviesData
        self.strategies: List[str] = ["brute_force", "minhash"]
        if strategy not in self.strategies:
            raise Exception("strategy not supported")
        self.strategy = strategy
        self.minhashDim = minhashDim
        self.userN: int = max([item[0] for item in self.data])
        self.movieN: int = max(max([item[1] for item in self.data]), max([item[0] for item in self.moviesData]))

    def makeRatingM(self) -> NoReturn:
        """
        生成ratingM矩阵。行代表用户，列代表电影，元素代表用户对电影评分

        :return:
        """
        self.ratingM = np.zeros((self.userN + 1, self.movieN + 1), dtype=np.float)
        for item in self.data:
            userId, movieId, rating = item
            self.ratingM[userId][movieId] = rating

    def makeIdfMap(self) -> NoReturn:
        """
        计算每个feature对应的idf

        :return:
        """
        featureCount: Dict[str, int] = {}
        for movieId, features in self.moviesData:
            for feature in features:
                if feature not in featureCount:
                    featureCount[feature] = 1
                else:
                    featureCount[feature] += 1
        self.idfMap: Dict[str, float] = {k: log(self.movieN / v) for k, v in featureCount.items()}
        self.featureMapId = {feature: index for index, feature in enumerate(self.idfMap.keys())}

    def makeTfIdfMatrix(self) -> NoReturn:
        """
        生成特征矩阵。行代表电影，列代表feature，元素代表tf-idf值

        :return:
        """
        self.featureN = len(self.idfMap.keys())
        self.tfIdfMatrix = np.zeros((self.movieN + 1, self.featureN), dtype=np.float)
        for movieId, features in self.moviesData:
            l: int = len(features)
            for feature in features:
                tf: float = 1 / l
                idf: float = self.idfMap[feature]
                tfIdf: float = tf * idf
                self.tfIdfMatrix[movieId][self.featureMapId[feature]] = tfIdf

    def makeM(self) -> NoReturn:
        """
        m是最终计算电影之间相似度的矩阵
        brute_force: m is tf-idf矩阵
        minhash: m列的维度相应变化

        :return:
        """
        if self.strategy in ["brute_force"]:
            self.m = self.tfIdfMatrix
        elif self.strategy in ["minhash"]:
            from random import randrange
            def makeHashes(size: int, n: int = 5) -> List[Callable[[np.ndarray], np.ndarray]]:
                def isCoPrime(a: int, b: int):
                    while b != 0:
                        a, b = b, a % b
                    return True if a == 1 else False

                hashes: List[Callable[[np.ndarray], np.ndarray]] = []
                for i in range(n):
                    a, b = randrange(2, 10000), randrange(2, 10000)
                    while not isCoPrime(a, size):
                        a, b = randrange(2, 10000), randrange(2, 10000)

                    def hash(X: np.ndarray) -> np.ndarray:
                        return (X * a + b) % size

                    hashes.append(hash)
                return hashes

            m: np.ndarray = self.tfIdfMatrix.copy()
            m[m > 0] = 1

            self.m = np.zeros((self.movieN + 1, self.minhashDim))
            n: int = self.tfIdfMatrix.shape[1]
            hashes = makeHashes(n, self.minhashDim)
            for index, hash in enumerate(hashes):
                X: np.ndarray = np.arange(0, n)
                X = hash(X)
                hashedM: np.ndarray = m[:, X]
                s = np.argmax(hashedM, axis=1)
                self.m[:, index] = s

    @staticmethod
    def cosineSim(A: np.ndarray, B: np.ndarray) -> float:
        """
        A*B/(|A|*|B|)

        :param A: 1-dim
        :param B: 1-dim
        :return:
        """
        a: float = np.dot(A, B)
        b: float = (norm(A) * norm(B))
        if b == 0:
            return -1
        return a / b

    @staticmethod
    def jarcardSim(A: np.ndarray, B: np.ndarray) -> float:
        assert A.shape == B.shape
        n: int = A.shape[0]
        a: int = np.sum(A == B)

        return a / n

    def makeSimVector(self, movieId: int, simMatrix: Optional[np.ndarray] = None) -> np.ndarray:
        """
        相似向量 (movieN+1, )

        :param movieId:
        :param simMatrix:
        :return:
        """
        if simMatrix != None:
            return simMatrix[movieId].copy()
        sims: np.ndarray = np.zeros((self.movieN + 1,), dtype=np.float)
        A: np.ndarray = np.asarray(self.m[movieId], dtype=np.float)
        for id in range(1, self.movieN + 1):
            if id == movieId:
                continue
            B: np.ndarray = np.asarray(self.m[id], dtype=np.float)
            if self.strategy in ["brute_force"]:
                sim: float = self.cosineSim(A, B)
            elif self.strategy in ["minhash"]:
                sim: float = self.jarcardSim(A, B)
            sims[id] = sim
        return sims

    def makeSimMatrix(self) -> np.ndarray:
        """
        相似矩阵 (movieN+1, movieN+1)

        :return:
        """
        simMatrix: np.ndarray = np.zeros((self.movieN + 1, self.movieN + 1), dtype=np.float)
        for m in range(1, self.movieN):
            for n in range(m + 1, self.movieN + 1):
                A: np.ndarray = np.asarray(self.m[m], dtype=np.float)
                B: np.ndarray = np.asarray(self.m[m], dtype=np.float)
                if self.strategy in ["brute_force"]:
                    sim: float = self.cosineSim(A, B)
                elif self.strategy in ["minhash"]:
                    sim: float = self.jarcardSim(A, B)
                simMatrix[m][n] = sim
                simMatrix[n][m] = sim
        self.simMatrix = simMatrix
        return simMatrix

    def scoreMovie(self, userId: int, movieId: int, simMatrix: Optional[np.ndarray] = None) -> float:
        """
        利用基于物品推荐算法计算用户对某电影评分

        :param userId:
        :param movieId:
        :param simMatrix: optional
        :return: 分数
        """
        if self.ratingM[userId][movieId] != 0:
            return self.ratingM[userId][movieId]

        ratings: np.ndarray = np.asarray(self.ratingM[userId], dtype=np.float)
        scored = np.nonzero(ratings)[0]  # userId用户打过分的movidId数组
        if scored.shape[0] == 0:
            return ratings[scored].mean()
        compareMovies: np.ndarray = scored[scored != movieId]  # 将自己从打过分的movidId数组去掉

        # 如果给了simMatrix, 直接从simMatrix里面拿电影之间的相似度。否则只有自己计算
        if simMatrix is None:
            sims: np.ndarray = np.zeros(compareMovies.shape, dtype=np.float)  # 记录相似度，按照compareMovies出现的movieId里面的顺序
            A: np.ndarray = np.asarray(self.m[movieId], dtype=np.float)
            for index, id in enumerate(compareMovies):
                B: np.ndarray = np.asarray(self.m[id], dtype=np.float)
                if self.strategy in ["brute_force"]:
                    sim: float = self.cosineSim(A, B)
                elif self.strategy in ["minhash"]:
                    sim: float = self.jarcardSim(A, B)
                sims[index] = sim
        else:
            sims: np.ndarray = simMatrix[movieId][compareMovies].copy()

        sims[sims < 0] = 0
        compareMovies, sims = compareMovies[sims > 0], sims[sims > 0]
        s = np.argsort(sims)[::-1]
        ratedMovies, ratedSims = compareMovies[s], sims[s]

        b: float = np.sum(ratedSims)
        a: float = np.sum(self.ratingM[userId][ratedMovies] * ratedSims)

        score: float = a / b

        return score

    def scoreMovies(self, userId: int, simMatrix: Optional[np.ndarray] = None) -> np.ndarray:
        scores: np.ndarray = np.zeros((self.movieN + 1,), dtype=np.float)
        for movieId in range(1, self.movieN + 1):
            scores[movieId] = self.scoreMovie(userId, movieId, simMatrix=simMatrix)
        return scores

    def recommendMovies(self, userId: int, k: int = 3, simMatrix: Optional[np.ndarray] = None) -> np.ndarray:
        """
        利用推荐系统算法为用户推荐电影

        :param userId:
        :param k: 推荐排名前k个电影
        :param simMatrix: optional
        :return: 推荐电影数组
        """
        scores: np.ndarray = self.scoreMovies(userId, simMatrix=simMatrix)
        ratedMovies: np.ndarray = np.argsort(scores)[::-1]
        ratedMovies = ratedMovies[self.ratingM[userId] != 0]
        return ratedMovies[:k]
