import csv
from typing import Tuple, List, NoReturn, Sequence
import threading


class mapThread(threading.Thread):
    def __init__(self, filename: str, id: int):
        threading.Thread.__init__(self)
        self.filename = filename
        self.id = id

    def run(self) -> NoReturn:
        print("map {} start".format(self.id))
        wordMap: List[Tuple[str, int]] = self.map()
        wordMap: List[Tuple[str, int]] = self.combine(wordMap)

        shuffle1, shuffle2, shuffle3 = self.shuffle(wordMap)

        with open('result/map{}{}'.format(1, self.id), 'w', newline='') as f:
            f_csv = csv.writer(f)
            f_csv.writerows(shuffle1)
        with open('result/map{}{}'.format(2, self.id), 'w', newline='') as f:
            f_csv = csv.writer(f)
            f_csv.writerows(shuffle2)
        with open('result/map{}{}'.format(3, self.id), 'w', newline='') as f:
            f_csv = csv.writer(f)
            f_csv.writerows(shuffle3)
        print("map {} finish".format(self.id))

    def map(self) -> List[Tuple[str, int]]:
        wordMap: List[Tuple[str, int]] = []
        with open(self.filename) as f:
            f_csv = csv.reader(f)  # this dataset doesnot to read the header first....
            for row in f_csv:
                for word in row:
                    word = word.strip()
                    wordMap.append(tuple([word, 1]))
            wordMap.sort()
            return wordMap

    def combine(self, wordMap: Sequence[Tuple[str, int]]) -> List[Tuple[str, int]]:
        if len(wordMap) == 0:
            return wordMap
        r: List[Tuple[str, int]] = []
        word, count = wordMap[0]
        for item in wordMap[1:]:
            if word == item[0]:
                count += int(item[1])
            else:
                r.append(tuple([word, count]))
                word, count = item
        # add last unappended item
        r.append(tuple([word, count]))
        return r

    def shuffle(self, wordMap: Sequence[Tuple[str, int]]) -> Tuple[
        List[Tuple[str, int]], List[Tuple[str, int]], List[Tuple[str, int]]]:
        def classify(word: str) -> int:
            if word[0] < 'Z':
                return 1
            elif word[0] < 'i':
                return 2
            else:
                return 3

        shuffle1: List[Tuple[str, int]] = []
        shuffle2: List[Tuple[str, int]] = []
        shuffle3: List[Tuple[str, int]] = []
        for item in wordMap:
            class_number: int = classify(item[0])
            if class_number == 1:
                shuffle1.append(item)
            elif class_number == 2:
                shuffle2.append(item)
            else:
                shuffle3.append(item)

        return tuple([shuffle1, shuffle2, shuffle3])
