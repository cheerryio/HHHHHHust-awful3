import csv
import numpy as np

from pagerank import pageRank

d = 0.85
N = 514

if __name__ == '__main__':
    P: np.ndarray = np.zeros(shape=(N, N), dtype=np.float)

    with open('sent_receive.csv') as f:
        f_csv = csv.reader(f)
        headers = next(f_csv)
        for row in f_csv:
            P[int(row[2])][int(row[1])] = 1

        R: np.ndarray = pageRank(P, d=0.85, maxerr=1e-8)

        rankId: np.ndarray = np.argsort(R, axis=0).reshape((N,))[::-1]
        rankId = rankId[rankId != 0]
        result = [tuple([id, R[id][0]]) for id in rankId]
        for each in result:
            print(each)
        # with open("Persons.csv") as f:
        #     f_csv = csv.reader(f)
        #     headers = next(f_csv)
        #     personMap = {int(item[0]): item[1] for item in list(f_csv)}
        #     rankPerson = [personMap[id] for id in rankId]
        #     print(rankPerson)
