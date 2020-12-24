def challenge_13(answer):
    a, b = answer
    if b and not (a ^ b) - a:
        return True

"""
集合也支持 ^ - 运算
^：集合异或
-：集合去补集

"""
if __name__ == "__main__":
    answer = {1}, {1}
    if challenge_13(answer) is True:
        print("ok")
