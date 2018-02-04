#!/bin/bash
FAIL=0

# Function to run the program and check its output against the expected
# text output
testText() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.ppm

  echo "Text test $TESTNO: ./triangle < input-$TESTNO.txt > output.ppm"
  ./triangle < input-$TESTNO.txt > output.ppm
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Text Test $TESTNO FAILED - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # We don't care about the output if it has to exit unsuccessfully
  if [ $ESTATUS -eq 0 ] ; then
      # Make sure the output matches the expected output.
      if ! diff -q expected-t$TESTNO.ppm output.ppm >/dev/null 2>&1 ; then
	  echo "**** Text Test $TESTNO FAILED - output didn't match the expected output"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Text Test $TESTNO PASS"
  return 0
}

# Function to run the program and check its output against the expected
# binary output
testBinary() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.ppm

  echo "Binary test $TESTNO: ./btriangle < input-$TESTNO.txt > output.ppm"
  ./btriangle < input-$TESTNO.txt > output.ppm
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED with binary encoding - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # We don't care about the output if it has to exit unsuccessfully
  if [ $ESTATUS -eq 0 ] ; then
      # Make sure the output matches the expected output.
      if ! diff -q expected-b$TESTNO.ppm output.ppm >/dev/null 2>&1 ; then
	  echo "**** Test $TESTNO FAILED with binary encoding - output didn't match the expected output"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Binary Test $TESTNO PASS"
  return 0
}

# Build the triangle program with text encoding
rm -f triangle
gcc -Wall -std=c99 -g triangle.c geometry.c text.c -o triangle -lm

# Run individual tests.
if [ -x triangle ] ; then
    testText 1 0
    testText 2 0
    testText 3 0
    testText 4 0
    testText 5 0
    testText 6 0
    testText 7 0
    testText 8 1
    testText 9 1
else
    echo "**** Program didn't compile successfully for text encoding"
    FAIL=1
fi

# Build the triangle program with binary encoding
rm -f btriangle
gcc -Wall -std=c99 -g triangle.c geometry.c binary.c -o btriangle -lm

# Run individual tests.
if [ -x btriangle ] ; then
    testBinary 1 0
    testBinary 2 0
    testBinary 3 0
    testBinary 4 0
    testBinary 5 0
    testBinary 6 0
    testBinary 7 0
    testBinary 8 1
    testBinary 9 1
else
    echo "**** Program didn't compile successfully for binary encoding"
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
