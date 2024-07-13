 CC = gcc
 CFLAGS = -ansi -Wall -pedantic
 DEBUG_FLAG = -g
 EXEC = assembler

 SOURCES = src/slist.c src/pre_assembler.c src/errors.c src/hash_table.c src/utils.c src/operations.c src/memory_image.c src/stage_one.c src/export_files.c  src/stage_two.c src/assembler_main.c
 OBJS = $(patsubst %.c,%.o, $(SOURCES))

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf *.o *.am *.ob *.ent *.ext
