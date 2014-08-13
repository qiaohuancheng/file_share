cflags = -g --std=c99 -lcurses -I /usr/local/myself/libxml2/include/libxml2/ -I /usr/include/mysql
libs_server_on = -L /usr/local/myself/libxml2/lib -lxml2 -L /usr/lib/mysql -lmysqlclient -lz
libs=-L /usr/local/myself/libxml2/lib -lxml2
objects_server = server_init.o main.o
objects_server_on = recv_xml.o server_on.o server_tls.o send_xml.o xmlchar_to_doc.o server_sasl.o md5.o server_bind.o xmlchar_construct.o client_final.o client_ll.o remote_login.o message_parsing.o read_xml.o write_xml.o
all : server server_on
.PHONY : all 
server : $(objects_server)
	cc -o server $(objects_server) $(libs)
server_on : $(objects_server_on)
	cc -o server_on $(objects_server_on) $(libs_server_on)
$(objects_server):%o:%c
	cc -c $(cflags) $< -o $@
$(objects_server_on):%o:%c
	cc -c $(cflags) $< -o $@
.PHONY : clean
clean :
	rm server $(objects_server) server_on $(objects_server_on)

