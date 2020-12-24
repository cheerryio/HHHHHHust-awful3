from copy import deepcopy


def challenge_14(answer):
    backup = deepcopy(answer)
    try:
        answer[0] += answer[1]
    except:
        print("after +=", answer)
        if backup != answer:
            return True

"""
answer是一个tuple,里面的两个元素是list，即 ([1],[2])
list相加可以正常进行，但是进行之后（值已经改变了之后），tuple的元素发生了变化，
触发异常。

但是把tuple里面的两个List都拿出来再相加，元素可以改变，不会触发异常：
a,b=answer
a+=b

"""
if __name__ == "__main__":
    answer = [1], [1]
    if challenge_14(answer) is True:
        print("ok")
