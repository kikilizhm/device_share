device.cgi : cgi.o sqlite.o sqlite3_lib/sqlite3.o
	gcc -o device.cgi cgi.o sqlite.o sqlite3_lib/sqlite3.o -Lsqlite3_lib -lpthread -ldl -L./
	cp device.cgi ../ftpsearch/web/
	cp device.db3 ../ftpsearch/web/

clean :
	rm -f sqlite3_lib/*.o
	rm -f *.o  
