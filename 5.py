def challenge_5(answer):
    r = reversed([1, 2, 3])
    if list(r) == list(r) + answer:
        return True

"""
reversed函数返回iterator
所以 在 list(r) == list(r) + answer 语句中， == 左边的那个list(r)
就会把迭代器里面的所有数据拿出来，所以右边的list(r)得到[]，是空

"""
if __name__ == "__main__":
    answer = [3, 2, 1]
    if challenge_5(answer) is True:
        print("answer ok")
