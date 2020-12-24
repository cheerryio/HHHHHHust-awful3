def challenge_6(answer):
    a, b = answer
    if max(a, b) != max(b, a):
        return True


if __name__ == "__main__":
    answer = {0}, {1}
    if challenge_6(answer) is True:
        print("answer ok")
