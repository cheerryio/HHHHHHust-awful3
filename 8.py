def challenge_8(answer):
    a, b, c = answer
    for x in a, b, c:
        if isinstance(x, float) or isinstance(x, complex):
            return False
    if a * (b * c) != (a * b) * c:
        return True

"""
利用python中 List * -1 = []
例如：[1,2,3,4] * -1 = []

"""
if __name__ == "__main__":
    answer = [0], -1, -1
    if challenge_8(answer) is True:
        print("ok")
