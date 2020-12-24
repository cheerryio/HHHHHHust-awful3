def challenge_16(answer):
    item, l = answer
    if item == 233 and item in l and l in l:
        return True

"""
str bytes可以调用in方法
int in str   ❌
int in bytes ✔

"""
if __name__ == "__main__":
    answer = 233, b'\xe9'
    if challenge_16(answer) is True:
        print("ok")
