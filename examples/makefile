compressgnw: compress_gnw.c ../compressor.c ../jsmn.c ../picocoap.c ../bit_operations.c ../schc.c
	gcc -g $(CFLAGS) -o compressgnw compress_gnw.c ../compressor.c ../jsmn.c ../picocoap.c ../bit_operations.c ../schc.c -lm

compress: compress.c ../compressor.c ../jsmn.c ../picocoap.c ../bit_operations.c ../schc.c
	gcc -g $(CFLAGS) -o compress compress.c ../compressor.c ../jsmn.c ../picocoap.c ../bit_operations.c ../schc.c -lm

icmpv6: icmpv6.c ../compressor.c ../jsmn.c ../picocoap.c ../bit_operations.c ../schc.c
	gcc -g $(CFLAGS) -o icmpv6 icmpv6.c ../compressor.c ../jsmn.c ../picocoap.c ../bit_operations.c ../schc.c -lm
	
lwm2m: lwm2m.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c
	gcc -g $(CFLAGS) -o lwm2m lwm2m.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c -lm

gateway: gateway.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c ../rcs.c timer.c socket/socket_server.c
	gcc -g $(CFLAGS) -o gateway gateway.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c ../rcs.c timer.c socket/socket_server.c -lm -lpthread

client: client.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c ../rcs.c timer.c socket/socket_client.c
	gcc -g $(CFLAGS) -o client client.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c ../rcs.c timer.c socket/socket_client.c -lm -lpthread	

interop: interop.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c
	gcc -g $(CFLAGS) -o interop interop.c ../compressor.c ../jsmn.c ../fragmenter.c ../picocoap.c ../bit_operations.c ../schc.c timer.c -lm -lpthread
	
clean:
	rm compress gateway client lwm2m interop icmpv6

all: gateway client compress lwm2m interop icmpv6