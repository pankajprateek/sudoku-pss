for i in {0..9999}
do

	cat $i | tr '.' '0' | tr '|' ' ' | tr '-' ' ' | tr -s '[:space]' > ./sudoku2/$i
	rm -f $i
done
