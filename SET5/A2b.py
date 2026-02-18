A = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

v = lambda c: ord(c) - 96

for p in range(1, 32):
    for y in A:
        need = -p * v(y)
        for x in A:
            if v(x) == need:
                print("p", p, x + y)
                y = None
                break
        if y is None:
            break
