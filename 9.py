def challenge_9(answer):
    a, b = answer
    for x in a, b:
        if isinstance(x, float) or isinstance(x, complex):
            return False
    if type(a ** b) != type(b ** a):
        return True


"""
幂运算可以得到int,float,complex类型

"""
if __name__ == "__main__":
    answer = -1, 2
    if challenge_9(answer) is True:
        print("answer ok")
