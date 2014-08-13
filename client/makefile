cflags = -g --std=c99 -lcurses -I /usr/local/myself/libxml2/include/libxml2/
libs=-L /usr/local/myself/libxml2/lib -lxml2
objects = send_xml.o connect_to_server.o main.o client_tls.o recv_xml.o xmlchar_to_doc.o xmlchar_construct.o client_sasl.o md5.o client_bind.o client_final.o
objects_file_server=file_trans_server.o recv_xml.o send_xml.o
objects_file_client=file_trans_client.o recv_xml.o send_xml.o
objects_file_server_tmp=file_trans_server.o
objects_file_client_tmp=file_trans_client.o
all : client file_server file_client
.PHONY : all 
client : $(objects)
	gcc -o client $(objects) $(libs)
file_server : $(objects_file_server)
	gcc -o file_server $(objects_file_server) $(libs)
file_client : $(objects_file_client)
	gcc -o file_client $(objects_file_client) $(libs)
$(objects):%o:%c
	gcc -c $(cflags) $< -o $@
$(objects_file_server_tmp):%o:%c
	gcc -c $(cflags) $< -o $@
$(objects_file_client_tmp):%o:%c
	gcc -c $(cflags) $< -o $@

.PHONY : clean
clean :
	rm client $(objects) file_server $(objects_file_server_tmp) file_client $(objects_file_client_tmp)

