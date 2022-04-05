# Lucas Toole 2022

SOURCE_FILES := main.c lmalloc.c

default:
	clang -Wall -Wextra -Werror -g -fsanitize=address -o driver $(SOURCE_FILES)
