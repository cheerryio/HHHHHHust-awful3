def challenge_17(answer):
    item, l = answer
    if l[0] == item and item not in l:
        return True

"""
key in Dict 等价于 key in Dict.keys()!!!!

"""
if __name__ == "__main__":
    answer = 1, {0: 1}
    if challenge_17(answer) is True:
        print("ok")
