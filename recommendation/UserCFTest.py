import unittest
import csv
from CF import UserCF


class MyTestCase(unittest.TestCase):
    def test_something(self):
        with open("data/train_set.csv") as trainF, open("data/test_set.csv") as testF:
            f_csv = csv.reader(trainF)
            headers = next(f_csv)
            train = [tuple([int(row[0]), int(row[1]), float(row[2])]) for row in
                     list(f_csv)]

            f_csv = csv.reader(testF)
            headers = next(f_csv)
            test = [tuple([int(row[0]), int(row[1]), float(row[2])]) for row in
                    list(f_csv)]

            # a = UserCF(data=train, K=50, strategy="brute_force")
            a = UserCF(data=train, K=50, strategy="minhash", minhashDim=10)
            a.makeRatingM()
            a.makeM()

            # recommendMovies = a.recommendMovies(1)
            # print(f"recommend movies:{recommendMovies}")

            err = 0
            lastUserId = 0
            for index, item in enumerate(test):
                userId, movieId, rating = item
                if lastUserId != userId:
                    simVector = a.makeSimVector(userId)
                score = a.scoreMovie(userId, movieId, simVector=simVector)
                err += (rating - score) ** 2
                lastUserId = userId
                print(f"number:{index}, test:{rating}, predict{round(score, 1)}, err:{err},")


if __name__ == '__main__':
    unittest.main()
