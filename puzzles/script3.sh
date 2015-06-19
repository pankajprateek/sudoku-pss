for i in {0..199}
do

	cat $i | tr '.' '0' | tr '|' ' ' | tr '-' ' ' | tr -s '[:space]' > ./sudoku3/$i
	rm -f $i
done
