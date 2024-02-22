from UsageTimePredict import UsageTimePredict
import time

with open('data.txt', 'r') as file:
        data = file.read().split(';')
usageTimePredict = UsageTimePredict()
print(usageTimePredict.update(True))
time.sleep(5)
print("--------------------------")
print(usageTimePredict.update(True))
time.sleep(5)
print("--------------------------")
print(usageTimePredict.update(False))
time.sleep(5)
print("--------------------------")
print(usageTimePredict.update(False))
time.sleep(5)
print("--------------------------")
print(usageTimePredict.update(True))
time.sleep(5)
print("--------------------------")
print(usageTimePredict.update(False))