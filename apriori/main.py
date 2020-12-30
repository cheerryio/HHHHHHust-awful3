import csv

from apriori import apriori

minSupport = 0.005
minConfidence = 0.5

if __name__ == '__main__':
    with open("Groceries.csv") as f:
        f_csv = csv.reader(f)
        headers = next(f_csv)
        itemsSet = [tuple(data[1][1:-1].split(",")) for data in list(f_csv)]

        a = apriori(itemsSet, bucketsSize=2000, strategy="pcy")

        ruleTotal: int = 0
        C1 = a.getC1()
        L1 = a.getL1(C1)
        a.displayL(L1)

        C2 = a.getC2(L1)
        L2 = a.getL(C2)
        a.displayL(L2)
        rules = a.getRule(L1, L2)
        rules = a.parseRule(rules)
        for rule in rules:
            print(rule)
        ruleTotal += len(rules)

        C3 = a.getC(L2)
        L3 = a.getL(C3)
        a.displayL(L2)
        rules = a.getRule(L2, L3)
        rules = a.parseRule(rules)
        for rule in rules:
            print(rule)
        ruleTotal += len(rules)

        C4 = a.getC(L3)
        L4 = a.getL(C4)
        a.displayL(L4)
        rules = a.getRule(L3, L4)
        rules = a.parseRule(rules)
        for rule in rules:
            print(rule)
        print("1项频繁集:", len(L1))
        print("2项频繁集:", len(L2))
        print("3项频繁集:", len(L3))
        print("4项频繁集", len(L4))
        print("total for 1,2,3:", ruleTotal)
