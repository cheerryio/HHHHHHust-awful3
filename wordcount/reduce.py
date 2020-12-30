import threading
import csv
from typing import Dict, List, Tuple


class reduceThread(threading.Thread):
    def __init__(self, q, id):
        threading.Thread.__init__(self)
        self.q = q
        self.id = id

    def run(self):
        print("reduce {} start".format(self.id))
        reduces: Dict[str, int] = self.reduce()
        reduces: List[Tuple[str, int]] = [(k, v) for k, v in reduces.items()]
        with open('result/reduce0{}'.format(self.id), 'w', newline='') as f:
            f_csv = csv.writer(f)
            f_csv.writerows(reduces)
        print("reduce {} finish".format(self.id))

    def reduce(self):
        q = self.q
        reduces: Dict[str, int] = {}
        while (not q.empty()):
            word, count = q.get()
            if word in reduces.keys():
                reduces[word] += count
            else:
                reduces[word] = count
        return reduces
