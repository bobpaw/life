#!/usr/bin/python3
rulestring = input("Rule String (B/S): ")
bornstring = rulestring[rulestring.index('B')+1:rulestring.index('/')]
survivestring = rulestring[rulestring.index('S')+1:]
bornarr = [0]*9
survivearr = [0] * 9
ruleint = 0
for i in range(len(bornstring)):
    bornarr[int(bornstring[i])] = True
for i in range(len(survivestring)):
    survivearr[int(survivestring[i])] = True
for i in range(9):
    if (bornarr[i]):
        ruleint |= 2**i
    if (survivearr[i]):
        ruleint |= 2**(i+9)
print(ruleint)
