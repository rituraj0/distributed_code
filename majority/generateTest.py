from random import randint
import sys

inputCount = int(sys.argv[1])
fileName = str(sys.argv[2])

fileObject = open(fileName, "w+")
# Write to file
toPrint = "#include <cassert>\n  long long GetN() {\n return " +  str(inputCount) + "LL;\n}"
fileObject.write(toPrint)

toPrint = "\nlong long GetVote(long long index) {\n switch ((int)index) {"
fileObject.write(toPrint)
maxNumber = (1<<30)

inputNums = []

for x in range(inputCount):
	inputNums.append(randint(-maxNumber, maxNumber))

if(randint(0, 5) < 5):
	for x in range(1, (inputCount*2)/3):
		inputNums[x] = inputNums[1]
	import random
	random.shuffle(inputNums)

for x in range(inputCount):
	toPrint = "case {0}: return {1}LL;\n".format(x, inputNums[x])
	fileObject.write(toPrint)

fileObject.write("default: assert(0);\n")
fileObject.write("} \n }")

fileObject.close()
