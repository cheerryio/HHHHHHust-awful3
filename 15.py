def challenge_15(answer):
    item, l = answer
    if item in l and not min(l) <= item <= max(l):
        return True

"""
answer1：空串在任意字符串中
answer2：令item是两个以上字符的字符串，而max(l) 和 min(l)得到的是单个字符

"""
if __name__ == "__main__":
    answer = "", "a"
    answer2 = "aa", "aaa"
    if challenge_15(answer) is True:
        print("answer ok")
    if challenge_15(answer2) is True:
        print("answer2 ok")
