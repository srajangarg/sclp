cd sclp
make clean > /dev/null
make > /dev/null

rm -rf Examples16
mkdir Examples16
rm -rf Examples/*.spim
cp Examples/* Examples16/
> "Examples/our.debug"
> "Examples/sample.debug"

for i in `seq 1 8`;
do
	echo "$i : " >> "Examples/our.debug"
	echo "$i : " >> "Examples/sample.debug"
	./sclp "Examples/test$i.c" &>> "Examples/our.debug"
	./sclp16 "Examples16/test$i.c" &>> "Examples/sample.debug"

	if [ ! -f "Examples/test$i.c.spim" ]; then
		if [ ! -f "Examples16/test$i.c.spim" ]; 
		then
    		echo "$i : ok, no files formed"
    	else
			echo "$i : error, sample program created file"
		fi
    else
		if [ ! -f "Examples16/test$i.c.spim" ]; 
		then
    		echo "$i : error, our program created file"
    	else
    		diff "Examples/test$i.c.spim" "Examples16/test$i.c.spim" &> /dev/null
    		if [ $? = 0 ];
    		then
    			echo "$i : ok, spim matches"
    		else
    			echo "$i : error, spim does not match"
    		fi
		fi
	fi

done

cd ..