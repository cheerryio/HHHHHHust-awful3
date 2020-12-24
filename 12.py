def challenge_12(answer):
    a, b = answer
    if a < b and all(x > y for x, y in zip(a, b)):
        return True

"""
List之间大于小于符号的比较：从前往后依次比较每一个元素的大小。
    如果有一个List的元素比较完，那他是小的那一个

all([]) is True

知道python中 all([]) 和 any([])的使用哦

"""
if __name__ == "__main__":
    answer = [], [0]
    if challenge_12(answer) is True:
        print("ok")
