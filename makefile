build_all:http_client server  debug release

clean_all:
	rm http_client server ./dbg/client_debug ./dbg/server_debug ./rel/server_release ./rel/client_release

rebuild_all: clean_all build_all 

http_client:
	gcc -Wall -o http_client dummy_http_client.c

server:
	gcc -Wall -o server dummy_http_server.c
	

debug_client:
		gcc -g3 -O0 -Wall dummy_http_client.c -o ./dbg/client_debug

debug_server:
		gcc -g3 -O0 -Wall dummy_http_server.c -o ./dbg/server_debug

release_server:
		gcc -g0 -o2 -Wall dummy_http_server.c -o ./rel/server_release

release_client:
		gcc -g0 -o2 -Wall dummy_http_client.c -o ./rel/client_release

debug: debug_client debug_server

release: release_client release_server

 
