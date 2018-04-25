#!/bin/bash
FAIL=0

# Make a fresh copy of the target program
make clean
make
if [ $? -ne 0 ] || [ ! -f regular ]; then
  echo "**** Make (compilation) FAILED"
  exit 13
fi

# Check the results of program execution.
# Splitting this out from the test function makes it easier
# to handle some error tests.
checkResults() {
  TESTNO=$1
  ESTATUS=$2

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test failed - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output.
  if [ -f expected-$TESTNO.txt ]; then
      diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - program output doesn't match expected"
	  FAIL=1
	  return 1
      fi
  elif [ -s output.txt ]; then
      # if there' no expected output for this test, the program shouldn't
      # have printed anything.
      echo "**** Test FAILED - shouldn't have printed output to stdout"
      FAIL=1
      return 1
  fi
      
  # Check different things, depending on whether it's successful.
  if [ $ESTATUS -eq 0 ]; then
      if [ -s stderr.txt ]; then
	  echo "**** Test FAILED - shouldn't have printed any error output"
	  FAIL=1
	  return 1
      fi
  else
      # Make sure the output matches the expected output.
      diff -q stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - didn't print the right error message"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Test the pattern matching program.
runTest() {
  rm -f output.txt stderr.txt

  echo "Test $1: ./regular '$2' input-$1.txt > output.txt 2> stderr.txt"
  ./regular "$2" input-$1.txt > output.txt 2> stderr.txt
  STATUS=$?

  checkResults "$1" "$3"
}
echo
echo "Tests for extended character class syntax."

runTest 30 '^[a-z]+$' 0
runTest 31 '^[A-Z]+$' 0
runTest 32 '^[a-zA-Z]+$' 0
runTest 33 '^[-m]+$' 0
runTest 34 '^[^a-zA-Z]+$' 0
runTest 35 '^[^a-]+$' 0
runTest 36 '^[^^]+$' 0
runTest 37 '^[^]+$' 0
runTest 38 '^[n-m]+$' 1

echo
echo "Tests for extended repetition syntax."
runTest 40 '^a{5,10}$' 0
runTest 41 '^[0123456789]{,5}$' 0
runTest 42 '^[0123456789]{3,}$' 0
runTest 43 '^a{7,6}$' 1
runTest 44 '^a{abc,xyz}$' 1

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi

exit 0
