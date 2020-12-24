def challenge_18(answer):
    a, b = answer
    if (a - b) != -(b - a):
        return True

"""
浮点数中的特殊值 inf nan
对极大的数，python表达为inf
而两个极大的数字相减，python表达为nan
nan具有性质 nan!=nan

"""
if __name__ == "__main__":
    answer = 1e100000, 1e100000
    if challenge_18(answer) is True:
        print("ok")
