from map import mapThread
from reduce import reduceThread
import queue
import csv

mapThreads = []
reduceThreads = []
queue1 = queue.Queue()
queue2 = queue.Queue()
queue3 = queue.Queue()


def classify(word: str) -> int:
    if word[0] < 'Z':
        return 1
    elif word[0] < 'i':
        return 2
    else:
        return 3


if __name__ == "__main__":
    for i in range(1, 10):
        thread = mapThread("data/source0{}".format(i), i)
        thread.start()
        mapThreads.append(thread)

    for t in mapThreads:
        t.join()

    print("load map data, adding data to queue")
    for m in range(1, 4):
        for n in range(1, 10):
            with open("result/map{}{}".format(m, n)) as f:
                f_csv = csv.reader(f)
                for item in f_csv:
                    item[0] = item[0].strip()
                    item[1] = int(item[1])
                    item = tuple(item)
                    if m == 1:
                        queue1.put(item)
                    elif m == 2:
                        queue2.put(item)
                    else:
                        queue3.put(item)
    print("finish loading map data")

    thread = reduceThread(q=queue1, id=1)
    thread.start()
    reduceThreads.append(thread)
    thread = reduceThread(q=queue2, id=2)
    thread.start()
    reduceThreads.append(thread)
    thread = reduceThread(q=queue3, id=3)
    thread.start()
    reduceThreads.append(thread)

    for t in reduceThreads:
        t.join()

    # combine three reduce files
    with open("result/result", "w") as result_f:
        data: str = ""
        for i in range(1, 4):
            with open("result/reduce0{}".format(i), "r") as f:
                data += f.read()
        result_f.write(data)
