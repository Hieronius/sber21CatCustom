FLAGS=-Wall -Werror -Wextra
YELLOW=\x1b[33m
WHITE=\x1b[0m

all: s21_cat

s21_cat: cat.o
	gcc $(FLAGS) cat.c -o s21_cat

cat.o: cat.c
	gcc $(FLAGS) -c cat.c

test1:
	@echo "Start tests"
	@echo "$(YELLOW)1.Test only file without flags$(WHITE)"
	echo "Hello, World!" > test1.txt
	cat test1.txt > original_output
	./s21_cat test1.txt > custom_output
	diff original_output custom_output
	rm test1.txt
	rm original_output
	rm custom_output
	@echo "Done"

test2:
	@echo "$(YELLOW)2. Test flag -n$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test2.txt
	cat -n test2.txt > original_output
	./s21_cat -n test2.txt > custom_output
	diff custom_output original_output
	rm test2.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
test3:
	@echo "$(YELLOW)3. Test flag -s$(WHITE)"
	echo "This is a long line that should be paginated.\n\n\n\nThis is another line." > test3.txt
	cat -s test3.txt > original_output
	./s21_cat -s test3.txt > custom_output
	diff custom_output original_output
	rm test3.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
test4:
	@echo "$(YELLOW)4. Test flag -e$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test4.txt
	cat -e test4.txt > original_output
	./s21_cat -e test4.txt > custom_output
	diff custom_output original_output
	rm test4.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
test5:
	@echo "$(YELLOW)5. Test flag -b$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test5.txt
	cat -b test5.txt > original_output
	./s21_cat -b test5.txt > custom_output
	diff custom_output original_output
	rm test5.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
test6:
	@echo "$(YELLOW)6. Test flag -t$(WHITE)"
	echo "Line 1\nLine 2\nLine 3 Русские буквы для проверки t" > test6.txt
	cat -t test6.txt > original_output
	./s21_cat -t test6.txt > custom_output
	diff custom_output original_output
	rm test6.txt
	rm original_output
	rm custom_output
	@echo "Done"

clean:
	rm -rf *.o
	rm -rf s21_cat
	rm -rf test1.txt
	rm -rf test2.txt
	rm -rf test3.txt
	rm -rf test4.txt
	rm -rf test5.txt
	rm -rf test6.txt
	rm -rf original_output
	rm -rf custom_output
