def challenge_2(answer):
    a, b, c, d = answer
    if a == b and a is b and c == d and c is not d:
        return True


"""
a == b and a is b , 可知a b 是immutable类型，比如字符串，数字（str or int）,
c==d and c is not d , 可知c d 是mutable类型，比如数组（List）

元组是immutable
"""
if __name__ == "__main__":
    answer = (1, 1, [1], [1])
    answer2 = ("Hello", "Hello", [1], [1])
    answer3 = (1, 1, (1, 1), (1, 1))
    if challenge_2(answer) is True:
        print("answer1 ok")
    if challenge_2(answer2) is True:
        print("answer2 ok")
    if challenge_2(answer3) is True:
        print("answer3 ok")
    else:
        print("answer3 wrong")
