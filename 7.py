def challenge_7(answer):
    a, b, c = answer
    for x in a, b, c:
        if isinstance(x, float) or isinstance(x, complex):
            return False
    if a * (b + c) != a * b + a * c:
        return True

"""
python中对str,List,Tuple的乘法

"""

if __name__ == "__main__":
    answer = 2, "a", "b"
    answer2 = 2, [1], [2]
    answer3 = 2, tuple([1]), tuple([2])
    if challenge_7(answer) is True:
        print("answer ok")
    if challenge_7(answer2) is True:
        print("answer2 ok")
    if challenge_7(answer3) is True:
        print("answer3 ok")
