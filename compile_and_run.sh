#!/bin/bash
# Ariel Kazule 24.05.22
###################################
COMPILE_COMMAND="make all"
RUN_COMMAND="./final.out"
TEST_FOLDER="test_good"
###################################

RED='\033[1;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo "Compiling:"
rm ./test_good/*.our.out
$COMPILE_COMMAND > /dev/null
if [ $? -ne 0 ]
then
  echo "${RED}BUILD FAILED${NC}"
  return
fi

rm -rd $TEST_FOLDER/*/*.our.out 2> /dev/null
rm -rd $TEST_FOLDER/*/*/*.our.out 2> /dev/null
rm -rd $TEST_FOLDER/*/*.fail 2> /dev/null
rm -rd $TEST_FOLDER/*/*/*.fail 2> /dev/null

echo "Running tests:"

FILES="$(find "./$TEST_FOLDER" -name "*.in")"

fail="0"
NumPass=0
NumFail=0
NumTot=0
FailedTestsNames=""
start_time=$(date +%s.%3N)
for f in $FILES
do
  NumTot=$((NumTot+1))
  echo "Running Test Number $NumTot - $f"
  basename="${f##*/}"
  basename="${basename%.*}"
  folder="$(dirname $(readlink -f $f))"
  
  EXPECTED="$(find "$folder" -name "$basename.*out")"
  if [ ${#EXPECTED[@]} == 0 ] || [ -z ${EXPECTED[0]} ]
  then
    EXPECTED="$(find "$folder" -name "$basename.*res")"
  fi
  if [ ${#EXPECTED[@]} == 0 ] || [ -z ${EXPECTED[0]} ]
  then
    tput cuu1 # move cursor up by one line
    tput el # clear the line
    echo -e "${RED}MISSING EXPECTED OUTPUT - ${f} ${NC}"
    fail=1
    NumFail=$((NumFail+1))
    FailedTestsNames="${FailedTestsNames}\n${f}"
    $RUN_COMMAND <  "$f" > "$folder/$basename.our.out"
    continue 
  fi
  EXPECTED=${EXPECTED[0]}
  $RUN_COMMAND <  "$f" > "$folder/$basename.our.out"
  out="$(diff --minimal "$folder/$basename.our.out" "$EXPECTED")"
  tput cuu1 # move cursor up by one line
  tput el # clear the line
  if [ -z "$out" ]
  then
      NumPass=$((NumPass+1))
      rm "$folder/$basename.our.out"
  else
      fail=1
      NumFail=$((NumFail+1))
      FailedTestsNames="${FailedTestsNames}\n${f}"
  fi
done

end_time=$(date +%s.%3N)
if [ "$fail" -eq "0" ]
then
  echo -e "${GREEN}ALL TEST PASSED ($NumPass/$NumTot)${NC}"
else
  echo -e "${RED}TEST FAILED:${NC}${FailedTestsNames}"
  echo -e "${GREEN}PASSED ($NumPass/$NumTot)${NC}"
  echo -e "${RED}FAILED ($NumFail/$NumTot)${NC}"
  echo -e "you can see fail output at testname.our.out"
fi
elapsed=$(echo "scale=3; $end_time - $start_time" | bc)
echo "Run Time: $elapsed"
