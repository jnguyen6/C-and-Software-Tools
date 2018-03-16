#!/bin/bash
FAIL=0

# Function to run the program against a test case and check
# its output and exit status for correct behavior
testAttractions() {
  TESTNO=$1

  rm -f output.txt

  echo "Test $TESTNO: ./attractions < input-$TESTNO.txt > output.txt"
  ./attractions < input-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited successfully
  if [ $STATUS -ne 0 ]; then
      echo "**** Test $TESTNO FAILED - incorrect exit status"
      FAIL=1
      return 1
  fi
  
  # Make sure any output to standard out looks right.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

if [ -x attractions ] ; then
    testAttractions 01
    testAttractions 02
    testAttractions 03
    testAttractions 04
    testAttractions 05
    testAttractions 06
    testAttractions 07
    testAttractions 08
    testAttractions 09
    testAttractions 10
    testAttractions 11
    testAttractions 12
    testAttractions 13
    testAttractions 14
    testAttractions 15
    testAttractions 16
    testAttractions 17
    testAttractions 18
    testAttractions 19
    testAttractions 20
else
    echo "**** Your program didn't compile successfully, so we couldn't test it."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi
