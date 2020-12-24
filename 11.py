def challenge_11(answer):
    if max(answer) != max(*answer):
        return True

"""
传参的时候的*号 max(*[[1,2,3,4]])
这样 max([[1,2,3,4]]) 和 max(*[[1,2,3,4]]) 都会匹配到 max(List)单个参数传List的形式

"""
if __name__ == "__main__":
    answer = [[1, 2, 3, 4]]
    if challenge_11(answer) is True:
        print("ok")
