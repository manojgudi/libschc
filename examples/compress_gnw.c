/*
 * (c) 2018 - 2022  idlab - UGent - imec
 *
 * Bart Moons
 *
 * This file is part of the SCHC stack implementation
 *
 * This is a basic example on how to compress 
 * and decompress a packet
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "../compressor.h"

#define MAX_PACKET_LENGTH		128

/* the direction is defined by the RFC as follows:
 * UP 	from device to network gateway
 * DOWN	from network gateway to device
 */
#define DIRECTION 				1 /* 0 = UP, 1 = DOWN */

/* the IPv6/UDP/CoAP packet */
uint8_t msg[] = {
    0x11, 0x00, 0x28, 0x01, 0x20, 0x50, 0x02, 0x80,
    0x00, 0x2e, 0x01, 0x00, 0x00, 0x00, 0x76, 0x6f,
    0xf7, 0x22, 0x12, 0x34, 0x26, 0xc6, 0x72, 0x74,
    0x1c, 0xaf, 0xa8, 0xa8, 0xff, 0x07, 0x9e, 0xfa,
    0x00, 0x8c, 0x00, 0xf9, 0x00, 0x00, 0x00, 0x00,

	/* Data */
	0x01, 0x02, 0x03, 0x04 };

int main() {
	/* COMPRESSION */
	/* initialize the client compressor */
	if(!schc_compressor_init()) {
		return 1;
	}

	uint8_t compressed_buf[MAX_PACKET_LENGTH] = { 0 };
	uint32_t device_id = 0x15;

	/* compress packet */
	struct schc_compression_rule_t *schc_rule;
	schc_bitarray_t c_bit_arr = SCHC_DEFAULT_BIT_ARRAY(MAX_PACKET_LENGTH, compressed_buf);

	schc_rule = schc_compress(msg, sizeof(msg), &c_bit_arr, device_id, DIRECTION);
#if USE_IP6_UDP == 1
	uint8_t compressed_header[8] = {
			0x01, /* rule id */
#if DIRECTION == 0
			/* direction UP */
			0x62, /* next header mapping index = 1 (2b), dev prefix mapping index = 2 (2b), dev iid LSB = 2 (4b) */
			0x11, /* app iid LSB = 1 (4b), dev port mapping index = 0 (1b), app port mapping index = 0 (1b), type mapping index = 1 (2b) */
			0xFB, /* token LSB = fb (8b) */
#else
			/* direction DOWN */
			0x62, /* next header mapping index = 1 (2b), dev prefix mapping index = 2 (2b), dev iid LSB = 2 (4b), */
			0x11, /* app iid LSB = 1 (4b), dev port mapping index = 0 (1b), app port mapping index = 0 (1b), type mapping index = 1 (2b) */
			0xFB, /* token LSB = fb (8b) */
#endif
			0x01, 0x02, 0x03, 0x04 /* payload */
	};
#elif USE_COAP == 1
	uint8_t compressed_header[7] = {
			0x01, /* rule id */
			0x7E, /* type = 1 (2b), token = fb (6 bits) */
			0xC0, /* token = fb (2 remaining bits) + 6 bits payload (0x01) */
			0x40, /* 2 bits payload (0x01) + 6 bits (0x02) */
			0x80, /* 2 bits payload (0x02) + 6 bits (0x03) */
			0xC1, /* 2 bits payload (0x03) + 6 bits (0x04) */
			0x00  /* 2 bits payload (0x04) + 6 bits padding */
	};
#else
	uint8_t compressed_header[5] = {
	0x01, /* rule id */
	0x01, 0x02, 0x03, 0x04 /* only payload */
};
#endif

	/* test the compressed bit array */
	int err = 0;
	printf("\n");
	if(schc_rule != NULL) {
		for (int i = 0; i < c_bit_arr.len; i++) {
			if (compressed_header[i] != c_bit_arr.ptr[i]) {
				printf(
						"main(): an error occured while compressing, byte=%02d, original=0x%02x, compressed=0x%02x\n",
						i, compressed_header[i], c_bit_arr.ptr[i]);
				err = 1;
			}
		}
	} else {
		err = 1;
	}

	if (!err) {
		printf("main(): compression succeeded\n");
	}

	/* DECOMPRESSION */
	uint8_t new_packet_len = 0;

	/* NOTE: DIRECTION is set to the flow the packet is following as this packet
	 * and thus equals the direction of the compressor
	 */
	unsigned char decomp_buf[MAX_PACKET_LENGTH] = { 0 };
	new_packet_len = schc_decompress(&c_bit_arr, decomp_buf, device_id,
			c_bit_arr.len, DIRECTION); /* use the compressed bit array to decompress */
	if (new_packet_len == 0) { /* some error occurred */
		return 1;
	}

	/* test the result */
	for (int i = 0; i < sizeof(msg); i++) {
		if (msg[i] != decomp_buf[i]) {
			printf(
					"main(): an error occured while decompressing, byte=%02d, original=0x%02x, decompressed=0x%02x\n",
					i, msg[i], decomp_buf[i]);
			err = 1;
		}
	}

	if (sizeof(msg) != new_packet_len) {
		printf(
				"main(); an error occured while decompressing, original length=%ld, decompressed length=%d\n",
				sizeof(msg), new_packet_len);
		err = 1;
	}

	if (!err) {
		printf("main(): decompression succeeded\n");
	}

	return err;
}
