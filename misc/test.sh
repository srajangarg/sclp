make clean
make
for i in `seq 1 8`;
do
	./sclp "Examples/test$i.c"
	./sclp16 "Examples16/test$i.c"
	diff "Examples/test$i.c.spim" "Examples16/test$i.c.spim" &> /dev/null
	if [ $? = 0 ];
	then
		echo "Test Case $i alright"
	else
		echo "OOPS! wrong for test case $i"
	fi
done