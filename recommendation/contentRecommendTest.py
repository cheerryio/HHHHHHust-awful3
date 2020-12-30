import unittest
import csv
from recommend import contentRecommend


class MyTestCase(unittest.TestCase):
    def test_something(self):
        with open("data/movies.csv") as moviesF, open("data/train_set.csv") as trainF, open(
                "data/test_set.csv") as testF:
            f_csv = csv.reader(moviesF)
            headers = next(f_csv)
            moviesData = [tuple([int(row[0]), row[2].split("|") if "no genres listed" not in row[2] else []]) for row in
                          list(f_csv)]

            f_csv = csv.reader(trainF)
            next(f_csv)
            train = [tuple([int(row[0]), int(row[1]), float(row[2])]) for row in
                     list(f_csv)]

            f_csv = csv.reader(testF)
            next(f_csv)
            test = [tuple([int(row[0]), int(row[1]), float(row[2])]) for row in
                    list(f_csv)]
            a = contentRecommend(data=train, moviesData=moviesData, strategy="minhash", minhashDim=10)

            a.makeRatingM()
            a.makeIdfMap()
            a.makeTfIdfMatrix()
            a.makeM()

            # a.makeSimMatrix()

            # movies=a.recommendMovies(1)
            # print(movies)

            err = 0
            for item in test:
                userId, movieId, rating = item
                score = a.scoreMovie(userId, movieId)
                # score = 3.22
                err += (rating - score) ** 2
                print(f"test:{rating}, predict{round(score, 1)}, err:{err},")


if __name__ == '__main__':
    unittest.main()
