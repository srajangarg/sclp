cd sclp
make clean > /dev/null
make > /dev/null

rm -rf Examples-216
mkdir Examples-216
rm -rf Examples-2/*.spim
cp Examples-2/* Examples-216/
> "Examples-2/our.debug"
> "Examples-2/sample.debug"

for i in `seq 1 5`;
do
	echo "$i : " >> "Examples-2/our.debug"
	echo "$i : " >> "Examples-2/sample.debug"
	./sclp "Examples-2/test$i.c" &>> "Examples-2/our.debug"
	./sclp16 "Examples-216/test$i.c" &>> "Examples-2/sample.debug"

	if [ ! -f "Examples-2/test$i.c.spim" ]; then
		if [ ! -f "Examples-216/test$i.c.spim" ]; 
		then
    		echo "$i : ok, no files formed"
    	else
			echo "$i : error, sample program created file"
		fi
    else
		if [ ! -f "Examples-216/test$i.c.spim" ]; 
		then
    		echo "$i : error, our program created file"
    	else
    		diff "Examples-2/test$i.c.spim" "Examples-216/test$i.c.spim" &> /dev/null
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