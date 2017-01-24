/*
 * Copyright (c) 2015 Apple Inc. All rights reserved.
 *
 * @APPLE_APACHE_LICENSE_HEADER_START@
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @APPLE_APACHE_LICENSE_HEADER_END@
 */

#ifndef __FIREHOSE_BUFFER_PRIVATE__
#define __FIREHOSE_BUFFER_PRIVATE__

#include <stdint.h>

#define OS_FIREHOSE_SPI_VERSION 20160318

/*!
 * @group Firehose SPI
 * SPI intended for logd only
 * Layout of structs is subject to change without notice
 */

#define FIREHOSE_BUFFER_CHUNK_SIZE				4096ul
#define FIREHOSE_BUFFER_LIBTRACE_HEADER_SIZE	2048ul
#define FIREHOSE_BUFFER_KERNEL_CHUNK_COUNT		16

typedef union {
	uint64_t fbc_atomic_pos;
#define FIREHOSE_BUFFER_POS_ENTRY_OFFS_INC		(1ULL <<  0)
#define FIREHOSE_BUFFER_POS_PRIVATE_OFFS_INC	(1ULL << 16)
#define FIREHOSE_BUFFER_POS_REFCNT_INC			(1ULL << 32)
#define FIREHOSE_BUFFER_POS_FULL_BIT			(1ULL << 56)
#define FIREHOSE_BUFFER_POS_USABLE_FOR_STREAM(pos, stream) \
		((((pos).fbc_atomic_pos >> 48) & 0x1ff) == (uint16_t)stream)
	struct {
		uint16_t fbc_next_entry_offs;
		uint16_t fbc_private_offs;
		uint8_t  fbc_refcnt;
		uint8_t  fbc_qos_bits;
		uint8_t  fbc_stream;
		uint8_t  fbc_flag_full : 1;
		uint8_t  fbc_flag_io : 1;
		uint8_t  _fbc_flag_unused : 6;
	};
} firehose_buffer_pos_u;

typedef struct firehose_buffer_chunk_s {
	uint8_t  fbc_start[0];
	firehose_buffer_pos_u volatile fbc_pos;
	uint64_t fbc_timestamp;
	uint8_t  fbc_data[FIREHOSE_BUFFER_CHUNK_SIZE
			- sizeof(firehose_buffer_pos_u)
			- sizeof(uint64_t)];
} __attribute__((aligned(8))) *firehose_buffer_chunk_t;

typedef struct firehose_buffer_range_s {
	uint16_t fbr_offset; // offset from the start of the buffer
	uint16_t fbr_length;
} *firehose_buffer_range_t;


// implemented by the kernel
extern void __firehose_buffer_push_to_logd(firehose_buffer_t fb, bool for_io);
extern void __firehose_critical_region_enter(void);
extern void __firehose_critical_region_leave(void);
extern void __firehose_allocate(vm_offset_t *addr, vm_size_t size);

// exported for the kernel
firehose_tracepoint_t
__firehose_buffer_tracepoint_reserve(uint64_t stamp, firehose_stream_t stream,
		uint16_t pubsize, uint16_t privsize, uint8_t **privptr);

firehose_tracepoint_t
__firehose_buffer_tracepoint_reserve_with_chunk(firehose_buffer_chunk_t fbc,
		uint64_t stamp, firehose_stream_t stream,
		uint16_t pubsize, uint16_t privsize, uint8_t **privptr);

void
__firehose_buffer_tracepoint_flush(firehose_tracepoint_t vat,
		firehose_tracepoint_id_u vatid);

void
__firehose_buffer_tracepoint_flush_chunk(firehose_buffer_chunk_t fbc,
		firehose_tracepoint_t vat, firehose_tracepoint_id_u vatid);

firehose_buffer_t
__firehose_buffer_create(size_t *size);

void
__firehose_merge_updates(firehose_push_reply_t update);



#endif // __FIREHOSE_BUFFER_PRIVATE__
