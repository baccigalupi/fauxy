#
#  Taken from Zed Shaw's awesome 'Learn to Code C the Hard Way': http://c.learncodethehardway.org/book
#  and modified a smidge
#

echo "\n\nRunning C Specs:"

for i in $(find ./c_spec -name "*_spec" -type f -maxdepth 4) $specs/*_spec
do
  if test -f $i
  then
    if $VALGRIND ./$i 2>> c_spec/spec.log
    then
      echo $i PASS
    else
      echo "ERROR in test $i: here's c_spec/spec.log"
      echo "------"
      tail c_spec/spec.log
      exit 1
    fi
  fi
done

echo ""
