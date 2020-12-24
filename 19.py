def challenge_19(answer):
    if answer.isdecimal():
        if len(answer) < 5:
            if sum(ord(c) - ord("0") for c in answer) == 23333:
                return True

"""
这些Unicode字符也被认为是decimal

"""
if __name__ == "__main__":
    answer = "᱙᱙᱃۰"
    if challenge_19(answer) is True:
        print("ok")
