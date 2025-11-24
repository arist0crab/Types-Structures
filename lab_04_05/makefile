# Компилятор
CC := gcc 

# Создаем необходимые папки если их нет
$(shell mkdir -p out)

# Опции компиляции
CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wvla -c -I inc
UFLAGS := -lrt -lcheck -lpthread -lm
LDFLAGS := -lm

# Общие объектные файлы (исключаем main.o)
OBJS_WITH_MAIN := $(patsubst src/%.c, out/%.o, $(wildcard src/*.c))
OBJS := $(filter-out out/main.o, $(OBJS_WITH_MAIN))
# Тестовые объектные файлы
TOBJS := $(patsubst unit_tests/%.c, out/%.o, $(wildcard unit_tests/*.c))

# Отладочная сборка, максимум отладочной информации, минимум оптимизации
ifeq ($(mode), debug)
	CFLAGS += -g3 -O0
endif

# Релизная сборка, отключаем assert и отладочную информацию
ifeq ($(mode), release)
 	CFLAGS += -DNDEBUG -g0
endif

# $^ - список зависимостей
# $@ - имя цели
# $< - первая зависимость

# Сценарии исполняемых файлов
app.exe : $(OBJS) out/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

unit_tests.exe : $(TOBJS) $(OBJS) out/check_main.o
	$(CC) -o $@ $^ $(LDFLAGS) $(UFLAGS)

# Сценарии объектных файлов
# out/%.o : src/%.c - % это замена для *, т.е. любые символы по сути
# -с: только компиляция, без линковки (то есть до объектников)
# $< имя первой зависимости из правила, исходный файл для компиляции
# -o указывает имя выходного файла
# $@ имя цели
out/%.o : src/%.c inc/%.h
	$(CC) $(CFLAGS) $< -o $@

out/main.o : src/main.c
	$(CC) $(CFLAGS) $^ -o $@

# Сценарии объектных тестовых файлов
out/check_%.o : unit_tests/check_%.c inc/%.h
	$(CC) $(CFLAGS) $< $(UFLAGS) -o $@

out/check_main.o : unit_tests/check_main.c
	$(CC) $(CFLAGS) $^ -o $@
	
# Другое
.PHONY : clean unit 

clean : 
	rm out/*.o *.exe

unit: unit_tests.exe
