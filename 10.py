def challenge_10(answer):
    a, b = answer
    if a and a.count(b) > len(a):
        return True


"""
str.count("") == len(str)+1

"""
if __name__ == "__main__":
    answer = "aaa", ""
    if challenge_10(answer) is True:
        print("ok")
