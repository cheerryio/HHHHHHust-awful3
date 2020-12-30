from typing import List, Optional, Sequence, NoReturn, Callable, Tuple
import numpy as np
from random import randrange

"""
推荐系统实现

user-content:基于用户相似度进行推荐
1. 构造ratingM矩阵。行表示每一个用户，列表示每一部电影，元素是用户对电影的评分。
2. 构造m矩阵。
    brute_force: m is ratingM矩阵
    minhash: 对ratingM矩阵进行01化处理。2.5评分以下为0， 2.5-5评分为1。使用minhash算法对01化后的ratingM矩阵对列进行降维处理
3. 计算用户相似矩阵，是一个对称矩阵（此处计算量过大，我没有计算）
4. 计算某用户对某电影的评分，给定用户与电影。
    如果有相似矩阵，根据相似度矩阵找到与用户最相似的且都对该电影进行过评分的k个用户。
    如果没有相似度矩阵，计算该用户与所有用户之间的相似向量，找到上述所描述的k个用户。
    用此k个用户计算用户对该电影评分。
5. 针对用户进行评分时，利用实现的基于用户的CF算法，计算该用户对所有电影的打分。得到评分高并且用户没有评分过的n个电影

minhash算法:
基于用户的collabrative filtering算法 计算的是用户之间的相似度，即行与行之间的相似度，所以可以将列降维
1. 生成多个哈希函数。哈希函数用来将一列映射到新的一列，并且不同列不能映射到相同的位置
    f(x)=(ax+b)%n 当a与n互质时，一定会将列映射到一个新的排列 。a,n可用随机数生成，直到a,n互质
2. 对每一轮（每一个哈希函数），将userRating矩阵的列代入哈希函数，映射到新的一列，这样又形成了一个
    与userRating矩阵形状一致的矩阵。对每一行，将1第一次出现的数组index记录，形成一个 1-dim 向量
    这个1-dim 向量就是此时M里面的一列。
3. 计算相似度时，对M里面的两行（对应两个user），计算对应位元素相等的概率（这是jarcard相似度？）

"""


class UserCF:
    def __init__(self, data: Sequence[Tuple[int, int, float]], K: int = 3, strategy="brute_force",
                 minhashDim: int = 10) -> NoReturn:
        self.strategies: List[str] = ["brute_force", "minhash"]
        if strategy not in self.strategies:
            raise Exception("receive unexpected strategy")
        self.strategy = strategy
        self.userN = max([item[0] for item in data])
        self.movieN = max([item[1] for item in data])
        self.data = data
        self.K = K
        self.minhashDim = minhashDim

    def makeRatingM(self) -> NoReturn:
        self.ratingM: np.ndarray = np.zeros((self.userN + 1, self.movieN + 1),
                                            dtype=np.float)  # matrix to record user movie ratings
        for item in self.data:
            userId, movieId, rating = item
            self.ratingM[userId][movieId] = rating

    def makeM(self) -> NoReturn:
        if self.strategy in ["brute_force"]:
            self.m = self.ratingM  # 当不做minhash降维，utilityMax直接是rating矩阵
        elif self.strategy in ["minhash"]:
            m: np.ndarray = self.ratingM.copy()
            m[m < 2.5] = 0
            m[m >= 2.5] = 1
            self.m = np.zeros((self.userN + 1, self.minhashDim), dtype=np.float)

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

            hashes = makeHashes(self.movieN + 1, self.minhashDim)
            for index, hash in enumerate(hashes):
                X: np.ndarray = np.arange(0, self.movieN + 1)
                hashedX = hash(X)
                assert np.unique(X).shape == np.unique(hashedX).shape

                hashedM: np.ndarray = m[:, hashedX]
                s = np.argmax(hashedM, axis=1)
                self.m[:, index] = s

    @staticmethod
    def jarcardSim(A: np.ndarray, B: np.ndarray) -> float:
        assert A.shape == B.shape
        n: int = A.shape[0]
        a: int = np.sum(A == B)

        return a / n

    @staticmethod
    def pearsonSim(A: np.ndarray, B: np.ndarray) -> float:
        """
        Pearson correlation coefficiency

        :param A: rating of size (self.userN,)
        :param B: rating of size (self.userN,)
        :return: similarity by Pearson correlation coefficient
        """
        mA = np.asarray(A, dtype=np.float)
        mB = np.asarray(B, dtype=np.float)
        Amean = mA[mA != 0].mean()
        Bmean = mB[mB != 0].mean()
        c = np.logical_and.reduce([A != 0, B != 0])
        if np.sum(c) == 0:
            return 0

        a = A[c] - Amean
        b = B[c] - Bmean

        from numpy.linalg import norm
        c: float = (norm(a, ord=2) * norm(b, ord=2))
        if c == 0:
            return 0
        r: float = np.dot(a, b) / c
        r = max(min(r, 1.0), -1.0)
        return r

    def makeSimVector(self, userId: int, simMatrix: Optional[np.ndarray] = None) -> np.ndarray:
        """
        计算该用户与所有用户之间的相似度向量

        :param userId:
        :param simMatrix:
        :return:
        """
        if simMatrix is not None:
            assert simMatrix.shape == (self.userN + 1, self.userN + 1)
            return simMatrix[userId]

        simVector: np.ndarray = np.zeros((self.userN + 1,), dtype=np.float)
        for i in range(1, self.userN + 1):
            if i == userId:
                simVector[i] = 0
                continue
            A: np.ndarray = self.m[userId]
            B: np.ndarray = self.m[i]
            if self.strategy in ["brute_force"]:
                A, B = A[1:], B[1:]
                sim: float = self.pearsonSim(A, B)
            elif self.strategy in ["minhash"]:
                sim: float = self.jarcardSim(A, B)

            simVector[i] = sim
        return simVector

    def makeSimMatrix(self) -> np.ndarray:
        """
        相似矩阵，对称矩阵。simMatrix[m][n]表达用户m与用户n之间的相似度

        :return: 相似矩阵
        """
        simMatrix = np.zeros((self.userN + 1, self.userN + 1), dtype=np.float)
        for m in range(1, self.userN):
            A: np.ndarray = np.asarray(self.m[m], dtype=np.float)
            for n in range(m + 1, self.userN + 1):
                B: np.ndarray = np.asarray(self.m[m], dtype=np.float)
                if self.strategy in ["brute_force"]:
                    A, B = A[1:], B[1:]
                    sim: float = self.pearsonSim(A, B)
                elif self.strategy in ["minhash"]:
                    sim: float = self.jarcardSim(A, B)
                simMatrix[m][n] = sim
                simMatrix[n][m] = sim
        self.simMatrix = simMatrix
        return simMatrix

    def scoreMovie(self, userId: int, movieId: int, simVector: Optional[np.ndarray] = None,
                   simRank: Optional[np.ndarray] = None, simMatrix: Optional[np.ndarray] = None) -> float:
        """
        用户对某一电影打分

        :param userId:
        :param movieId:
        :param simVector: optional
        :param simRank: optional
        :param simMatrix: optional
        :return: 用户对电影打分
        """
        if self.ratingM[userId][movieId] != 0:
            return self.ratingM[userId][movieId]

        if simVector is None:
            if simMatrix is None:
                simVector: np.ndarray = self.makeSimVector(userId)
            else:
                simVector: np.ndarray = simMatrix[userId]

        if simRank is None:
            simRank: np.ndarray = np.argsort(np.abs(simVector))[::-1]
        rank = simRank[self.ratingM[simRank][:, movieId] != 0]
        rank = rank[:self.K]  # rank的元素是按照sim从大到小排的userId
        a: float = np.sum(simVector[rank] * self.ratingM[rank][:, movieId])
        b: float = np.sum(simVector[rank])
        score: float = a / b
        if b == 0:
            print("return average rating of user")
            return self.ratingM[userId][self.ratingM[userId] != 0].mean()
        return score

    def scoreMovies(self, userId: int, simVector: Optional[np.ndarray] = None,
                    simMatrix: Optional[np.ndarray] = None) -> np.ndarray:
        """
        该用户对每一个movie调用scoreMovie进行评分

        :param userId:
        :param simVector: optional 相似向量
        :param simMatrix: optional 相似矩阵
        :return: 用户对所有movie的评分
        """
        if simVector is None:
            if simMatrix is None:
                simVector: np.ndarray = self.makeSimVector(userId)
            else:
                assert simMatrix.shape == (self.userN + 1, self.userN + 1)
                simVector: np.ndarray = simMatrix[userId]
        assert simVector.shape == (self.userN + 1,)

        simRank: np.ndarray = np.argsort(np.abs(simVector))[::-1]
        scores: np.ndarray = np.zeros((self.movieN + 1,), dtype=np.float)
        for movieId in range(1, self.movieN + 1):
            score: float = self.scoreMovie(userId, movieId, simVector=simVector, simRank=simRank, simMatrix=simMatrix)
            scores[movieId] = score
        return scores

    def recommendMovies(self, userId: int, n: int = 3, simVector: Optional[np.ndarray] = None,
                        simMatrix: Optional[np.ndarray] = None) -> np.ndarray:
        """
        利用基于用户的collabrative filtering算法推荐n部电影

        :param userId:
        :param n: 推荐n部电影
        :return: 推荐电影数组
        """
        scores: np.ndarray = self.scoreMovies(userId, simVector=simVector, simMatrix=simMatrix)
        ratedMovies: np.ndarray = np.argsort(scores)[::-1]
        ratedMovies = ratedMovies[self.ratingM[userId] != 0]
        return ratedMovies[:n]
