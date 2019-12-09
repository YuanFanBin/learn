#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 5.1.3 列表推导式
print list(map(lambda x: x**2, range(10)))
# 等价于
print [x**2 for x in range(10)]

print [(x, y) for x in [1,2,3] for y in [3,1,4] if x != y]

from math import pi
print [str(round(pi, i)) for i in range(1, 6)]

# 5.1.4 嵌套的列表推导式

matrix = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12],
]
print [[row[i] for row in matrix ] for i in range(4)]
# 等价于
transposed = []
for i in range(4):
    transposed.append([row[i] for row in matrix])
print transposed
# zip() 函数
print list(zip(*matrix))

# 5.2 del语句
