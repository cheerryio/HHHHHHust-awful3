def challenge_3(answer):
    if answer in answer == answer:
        return True

"""
python中比较运算符相连
a in b == c 等价于 a in b and  b == c
a < b < c 等价于 a < b and b < c
"""
if __name__ == "__main__":
    answer = "Hello"
    if challenge_3(answer) is True:
        print("answer ok")
