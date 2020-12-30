from typing import List, Tuple, Dict,Sequence,NoReturn,Mapping
import numpy as np
from bitmap import BitMap

"""
class是apriori算法的主要实现。产生备选集，由备选集得到频繁项集，
频繁项集再组合成为备选集，直到最终频繁项集为空，迭代终止
备选集：备选集C1需要自己计算，以items的tuple作为键，以在各个items出现次数作为值
频繁项集：以items的tuple作为键，以支持度作为值

@param basketSet: source dataset
@param minSupport: 最小支持度
@param minConfidence: 最小支持度
@param strategy: four strategies supported. 
                 1. brute_force: for the plain apriori algrithm
                 2. pcy: 在getL1中需要从原始的basketSet创建pair hash到buckets，getC2拿到的pair (i,j) i，j都是频繁项集，
                    && i，j被hash到一个frequent bucket
                 3. multi_stage: 在getL1中从原始的backetSet创建pair hash到buckets1,得到一阶频繁项集后，
                    将一阶频繁项集组合的pair hash到buckets2，在getC2拿到的pair (i,j) i，j都是频繁项集 &&
                    i，j map到buckets1的frequent bucket && i，j map到buckets2的frequent bucket
                 4. multi_hash: 在getL1中从原始的backetSet创建pair后，利用不同的hash函数hash到多个buckets，
                    生成多个bitmap，getC2得到的pair (i,j) i，j 都是频繁项集 && i，j在所有map中都被map到了frequent bucket
"""


class apriori():
    def __init__(self, basketSet: Sequence[Tuple],
                 minSupport: float = 0.005,
                 minConfidence: float = 0.5,
                 bucketsSize: int = 50,
                 strategy: str = "brute_force"):
        self.strategies: List[str] = ["brute_force", "pcy", "multi_stage", "multi_hash"]
        if strategy not in self.strategies:
            raise Exception("strategy not supported")
        self.strategy = strategy
        if self.strategy != "brute_force":
            self.bitmaps = []
        self.minSupport: float = minSupport
        self.minConfidence: float = minConfidence
        self.bucketsSize: int = bucketsSize
        self.total: int = len(basketSet)
        self.idMapName: List[str] = []
        for basket in basketSet:
            for item in basket:
                if item not in self.idMapName:
                    self.idMapName.append(item)
        self.nameMapId: Dict[str, int] = {self.idMapName[i]: i for i in range(len(self.idMapName))}
        self.basketSet: List[Tuple] = []
        for basket in basketSet:
            mappedBasket = [self.nameMapId[item] for item in basket]
            mappedBasket.sort()
            self.basketSet.append(tuple(mappedBasket))

    def hash1(self, i: int, j: int) -> int:
        return hash(i * 2333 + j * 8461) % self.bucketsSize

    def hash2(self, i: int, j: int) -> int:
        return hash(i * 2339 + j * 5011 + 1) % self.bucketsSize

    def hash3(self, i: int, j: int) -> int:
        return hash(i * 6113 + j * 2801 + 2) % self.bucketsSize

    def getC1(self) -> Dict[Tuple, int]:
        C1: Dict[Tuple, int] = {tuple([i]): 0 for i in range(len(self.idMapName))}
        return C1

    def getL1(self, C: Mapping[Tuple, int]) -> Dict[Tuple, int]:
        minSupportNumber = self.minSupport * self.total

        if self.strategy in ["pcy", "multi_stage"]:
            buckets: List[int] = [0] * self.bucketsSize
            bitmap = BitMap(self.bucketsSize)

            for basket in self.basketSet:
                for i in range(len(basket) - 1):
                    for j in range(i + 1, len(basket)):
                        buckets[self.hash1(basket[i], basket[j])] += 1
            for i in range(self.bucketsSize):
                if buckets[i] >= minSupportNumber:
                    bitmap.set(i)
            self.bitmaps.append(bitmap)

        elif self.strategy in ["multi_hash"]:
            buckets1, buckets2, buckets3 = [0] * self.bucketsSize, [0] * self.bucketsSize, [0] * self.bucketsSize
            bitmap1, bitmap2, bitmap3 = BitMap(self.bucketsSize), BitMap(self.bucketsSize), BitMap(self.bucketsSize)
            for basket in self.basketSet:
                for i in range(len(basket) - 1):
                    for j in range(i + 1, len(basket)):
                        buckets1[self.hash1(basket[i], basket[j])] += 1
                        buckets2[self.hash2(basket[i], basket[j])] += 1
                        buckets3[self.hash3(basket[i], basket[j])] += 1
            for i in range(self.bucketsSize):
                if buckets1[i] >= minSupportNumber:
                    bitmap1.set(i)
                if buckets2[i] >= minSupportNumber:
                    bitmap2.set(i)
                if buckets3[i] >= minSupportNumber:
                    bitmap3.set(i)
            self.bitmaps.append(bitmap1)
            self.bitmaps.append(bitmap2)
            self.bitmaps.append(bitmap3)

        return self.getL(C)

    def getC2(self, L: Mapping[Tuple, int]) -> Dict[Tuple, int]:
        C: Dict[Tuple, int] = self.getC(L)

        minSupportNumber = self.minSupport * self.total
        keys = list(C.keys())
        pairs = keys

        if self.strategy in ["multi_stage"]:
            buckets: List[int] = [0] * self.bucketsSize
            bitmap = BitMap(self.bucketsSize)

            for basket in self.basketSet:
                for i in range(len(basket) - 1):
                    for j in range(i + 1, len(basket)):
                        if tuple([basket[i], basket[j]]) in pairs:
                            buckets[self.hash2(basket[i], basket[j])] += 1

            for i in range(self.bucketsSize):
                if buckets[i] >= minSupportNumber:
                    bitmap.set(i)
            self.bitmaps.append(bitmap)

        if self.strategy in ["pcy", "multi_stage", "multi_hash"]:
            for key in pairs:
                pair = key
                valid = True
                for m in range(len(self.bitmaps)):
                    if m == 0 and not self.bitmaps[m].test(self.hash1(pair[0], pair[1])):
                        valid = False
                    if m == 1 and not self.bitmaps[m].test(self.hash2(pair[0], pair[1])):
                        valid = False
                    if m == 2 and not self.bitmaps[m].test(self.hash3(pair[0], pair[1])):
                        valid = False
                    if not valid:
                        break
                if not valid:
                    del C[key]
        return C

    def getL(self, C: Mapping[Tuple, int]) -> Dict[Tuple, int]:
        for basket in self.basketSet:
            for key in C.keys():
                if set(key).issubset(set(basket)):
                    C[key] += 1

        minSupportNumber = self.minSupport * self.total
        L: Dict[Tuple, int] = {}
        for key, value in C.items():
            if value >= minSupportNumber:
                L[key] = value
        return L

    def getC(self, L: Mapping[Tuple, int]) -> Dict[Tuple, int]:
        C: Dict[Tuple, int] = {}
        baskets = list(L.keys())
        length = len(baskets[0]) + 1
        for m in range(len(baskets) - 1):
            for n in range(m + 1, len(baskets)):
                combined = set(list(baskets[m]) + list(baskets[n]))
                combined = list(combined)
                combined.sort()
                if len(combined) == length and tuple(combined) not in C.keys():
                    combinedArray: np.ndarray = np.array(combined)
                    valid = True
                    for element in combinedArray:
                        if tuple(combinedArray[combinedArray != element]) not in baskets:
                            valid = False
                            break
                    if valid:
                        C[tuple(combined)] = 0
        return C

    def getRule(self, L1: Mapping[Tuple, int], L2: Mapping[Tuple, int]) -> List[Tuple[Tuple, Tuple, float]]:
        rules: List[Tuple[Tuple, Tuple, float]] = []
        for basket in L2.keys():
            basketArray = np.array(list(basket))
            for item in basket:
                target: Tuple = tuple([item])
                source: Tuple = tuple(list(basketArray[basketArray != item]))
                confidence: float = L2[basket] / L1[source]
                if confidence >= self.minConfidence:
                    rules.append(tuple([source, target, confidence]))
        return rules

    def displayL(self, L: Mapping[Tuple, int]) -> NoReturn:
        for k, v in L.items():
            print(k, v / self.total)

    def parseRule(self, rules: Sequence[Tuple[Tuple, Tuple, float]]) -> List[Tuple[Tuple, Tuple, float]]:
        parsedRule: List[Tuple[Tuple, Tuple, float]] \
            = [tuple([
            tuple([self.idMapName[i] for i in rule[0]]),
            tuple([self.idMapName[i] for i in rule[1]]),
            rule[2]
        ]) for rule in rules]
        return parsedRule
