def challenge_4(answer):
    a1, b1 = answer
    a2, b2 = answer
    if a1 * 2 != a1 and b1 * 2 != b1:
        a1 *= 2
        b1 *= 2
        if a1 == a2 and b1 != b2:
            return True


"""
理解数组mutable

"""
if __name__ == "__main__":
    answer = [1], 1
    if challenge_4(answer) is True:
        print("answer ok")
