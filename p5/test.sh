#!/bin/bash
FAIL=0

# make a fresh copy of the target programs
make clean
make
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

# Check the results of an execution of encode.
# Splitting this out from the test function makes it easier
# to handle some error tests.
checkEncode() {
  TESTNO=$1
  ESTATUS=$2

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test failed - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Check different things, depending on whether it's successful.
  if [ -s stdout.txt ]; then
      echo "**** Test failed - program shouldn't print anything to standard output"
      FAIL=1
      return 1
  fi
      
  # Check different things, depending on whether it's successful.
  if [ $ESTATUS -eq 0 ]; then
      # Make sure the output matches the expected output.
      diff -q encoded-$TESTNO.bin encoded.bin >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - encoded output doesn't match expected"
	  FAIL=1
	  return 1
      fi
      
      if [ -s stderr.txt ]; then
	  echo "**** Test FAILED - shouldn't have printed any error output"
	  FAIL=1
	  return 1
      fi
  else
      # Make sure the output matches the expected output.
      diff -q stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - printed the wrong error message"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Test the encode program.
testEncode() {
  rm -f encoded.bin stdout.txt stderr.txt

  echo "Test $1: ./encode $3 input-$1.txt encoded.bin > stdout.txt 2> stderr.txt"
  ./encode $3 input-$1.txt encoded.bin > stdout.txt 2> stderr.txt
  STATUS=$?

  checkEncode "$1" "$2" "$3"
}

# Check the results of an execution of decode.  Same idea as with
# checkEncode
checkDecode() {
  TESTNO=$1
  ESTATUS=$2

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test failed - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Check different things, depending on whether it's successful.
  if [ -s stdout.txt ]; then
      echo "**** Test failed - program shouldn't print anything to standard output"
      FAIL=1
      return 1
  fi
      
  # Check different things, depending on whether it's successful.
  if [ $ESTATUS -eq 0 ]; then
      # The file we get back should match the corresponding input.
      diff -q input-$TESTNO.txt output.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - decoded output doesn't match expected"
	  FAIL=1
	  return 1
      fi
      
      if [ -s stderr.txt ]; then
	  echo "**** Test FAILED - shouldn't have printed any error output"
	  FAIL=1
	  return 1
      fi
  else
      # Make sure the output matches the expected output.
      diff -q stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - printed the wrong error message"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

 # Test the decode program.
testDecode() {
  rm -f output.txt stdout.txt stderr.txt

  echo "Test $1: ./decode $3 encoded-$1.bin output.txt > stdout.txt 2> stderr.txt"
  ./decode $3 encoded-$1.bin output.txt > stdout.txt 2> stderr.txt
  STATUS=$?

  checkDecode "$1" "$2" "$3"
}

# Test the encode program
echo
echo "Testing encode"
if [ -x encode ]; then
  testEncode 1 0 codes-1.txt
  testEncode 2 0 codes-1.txt
  testEncode 3 0 codes-1.txt
  testEncode 4 0 codes-1.txt
  testEncode 5 0 codes-2.txt
  testEncode 6 0 codes-1.txt
  testEncode 7 0 codes-2.txt
  testEncode 8 0 codes-1.txt

  testEncode 10 1 codes-1.txt

  # invalid command-line arguments.
  rm -f output.bin stdout.txt
  echo "Test 11: ./encode codes-1.txt > stdout.txt 2> stderr.txt"
  ./encode codes-1.txt > stdout.txt 2> stderr.txt
  STATUS=$?
  checkEncode 11 1
  
  testEncode 13 1 bad-codes.txt
else
  echo "Since your encode program didn't compile, we couldn't test it"
fi


# Test the decode program
echo
echo "Testing decode"
if [ -x decode ]; then
  testDecode 1 0 codes-1.txt
  testDecode 2 0 codes-1.txt
  testDecode 3 0 codes-1.txt
  testDecode 4 0 codes-1.txt
  testDecode 5 0 codes-2.txt
  testDecode 6 0 codes-1.txt
  testDecode 7 0 codes-2.txt
  testDecode 8 0 codes-1.txt

  testDecode 9 1 codes-1.txt
  testDecode 12 1 codes-1.txt
else
  echo "Since your decode program didn't compile, we couldn't test it"
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi
