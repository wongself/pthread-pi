from decimal import *

getcontext().prec = 200

s = Decimal(1)
# Signal for the next operation
pi = Decimal(3)
n = 10000000

print("Approximation of the number PI through the Nilakantha's series\n")
print("Please wait. Running...\n")

for i in range(2, n * 2, 2):
    pi = pi + s * (Decimal(4) / (Decimal(i) * (Decimal(i) + Decimal(1)) *
                                 (Decimal(i) + Decimal(2))))
    s = -1 * s

print("Approximated value of PI :")
print(pi)
