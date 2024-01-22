FLAGS=-Wall -Werror -Wextra

all: s21_cat

s21_cat: cat.o
	gcc $(FLAGS) cat.c -o s21_cat

cat.o: cat.c
	gcc $(FLAGS) -c cat.c

clean:
	rm -rf *.o
	rm -rf s21_cat
