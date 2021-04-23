obj = Controller.cpp CTable.cpp main.cpp Sql.cpp
target = main
CC = g++

do:
	$(CC) -g $(obj) -o $(target)
