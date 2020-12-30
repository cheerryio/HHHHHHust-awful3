import csv
from typing import List, Tuple
from CF import recSysU

# items of train set
# [userid, movieid, rating, timestamp]

if __name__ == '__main__':
    with open("data/train_set.csv") as trainF, open("data/test_set.csv") as testF:
        f_csv = csv.reader(trainF)
        headers = next(f_csv)
        train: List[Tuple[int, int, float]] = [tuple([int(row[0]), int(row[1]), float(row[2])]) for row in list(f_csv)]

        f_csv = csv.reader(testF)
        headers = next(f_csv)
        test: List[Tuple[int, int, float]] = [tuple([int(row[0]), int(row[1]), float(row[2])]) for row in list(f_csv)]

        a = recSysU(data=train, K=100)
        a.makeUtilityMatrix()
        err: float = 0
        lastUserId = 0
        for item in test:
            userId, movieId, rating = item
            if lastUserId != userId:
                simMatrix = a.makeSimMatrix(userId)
            score: float = a.scoreMovie(userId, movieId, simMatrix)
            err += (rating - score) ** 2
            print(err)
            lastUserId = userId
