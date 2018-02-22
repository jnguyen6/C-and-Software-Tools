#!/bin/bash
FAIL=0

# Function to check the output of the program after it's run, including checking
# standard output, standard error and exit status.
checkHangman() {
  TESTNO=$1
  ESTATUS=$2
  STATUS=$3

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - Expected an exit status of $ESTATUS, bug got: $STATUS"
      FAIL=1
      return 1
  fi

  # For this program, we should get output either to standard error or to
  # standard output.
  if [ $ESTATUS -eq 0 ] ; then
      # Make sure the output matches the expected output.
      if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
	  echo "**** Test $TESTNO FAILED - output didn't match the expected output"
	  FAIL=1
	  return 1
      fi
  else
      # Make sure the error message matches the expected error message.
      if ! diff -q stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1 ; then
	  echo "**** Test $TESTNO FAILED - standard error didn't match the expected error message"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Run a test of the hangman program, then check to make sure we got the right
# behavior
testHangman() {
  TESTNO=$1
  WORDLIST=$2
  SEED=$3
  ESTATUS=$4
  
  rm -f output.txt stderr.txt

  echo "Test $TESTNO: ./hangman $WORDLIST $SEED < input-$TESTNO.txt > output.txt 2> stdout.txt"
  ./hangman $WORDLIST $SEED < input-$TESTNO.txt > output.txt 2> stderr.txt
  STATUS=$?

  checkHangman $TESTNO $ESTATUS $STATUS
}

# Try to get a fresh compile of the project.
make clean
make
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your program."
    FAIL=1
fi

# Run individual tests.
if [ -x hangman ] ; then
    testHangman 1 animals.txt 2 0
    testHangman 2 animals.txt 2 0
    testHangman 3 animals.txt 2 0
    testHangman 4 food.txt 3 0
    testHangman 5 food.txt 3 0
    testHangman 6 animals.txt 2 0
    testHangman 7 animals.txt 2 0
    testHangman 8 animals.txt 2 0
    testHangman 9 animals.txt 2 0
    testHangman 10 animals.txt 2 0

    # A test we can't run via testHangman
    echo "Test 11: ./hangman < input-11.txt > output.txt 2> stdout.txt"
    ./hangman < input-11.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkHangman 11 1 $STATUS
    
    testHangman 12 notafile.txt 1 1
    testHangman 13 bad-words-13.txt 2 1
    testHangman 14 bad-words-14.txt 3 1
    testHangman 15 bad-words-15.txt 4 1
else
    echo "**** Program didn't compile successfully"
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
