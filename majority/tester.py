from subprocess import call
import shlex
from random import randint
import sys

fileName = sys.argv[1]

dcj = "/Users/rituraj/coding/distributed_code/dcj_mac_os/dcj.sh "

def buildSolutions():
	# Build my solution
	print("Building my solution")
	call(shlex.split( dcj + 'build --source=my_solution.cpp'))

	# Build correct solution
	print("Building correct solution")
	call(shlex.split( dcj + ' build --source=correct_solution.cpp'))


for testCount in range(10):
	print("\n\nTest: {0}".format(testCount))
	inputCount = randint(1, (1<<15))
	print("Input Count " + str(inputCount))
	# Generate test file
	call(shlex.split('python generateTest.py {0} {1}'.format(inputCount, fileName)))
	print("Generated file " + str(fileName))

	buildSolutions()

	for nodeCount in [10, 100]:
		print("\nTesting on nodes {0}".format(nodeCount))
		# Run Admin
		print("Running correct solution")
		call(shlex.split( dcj + ' run --executable=./correct_solution --nodes={0} --output=all'.format(nodeCount)))

		# Running my solution
		print("Running MY solution")
		call(shlex.split( dcj + ' run --executable=./my_solution --nodes={0} --output=all'.format(nodeCount)))

