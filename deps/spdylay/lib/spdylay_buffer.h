/*
 * Spdylay - SPDY Library
 *
 * Copyright (c) 2012 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef SPDYLAY_BUFFER_H
#define SPDYLAY_BUFFER_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <spdylay/spdylay.h>

typedef struct spdylay_buffer_chunk {
  uint8_t *data;
  struct spdylay_buffer_chunk *next;
} spdylay_buffer_chunk;

/*
 * List of fixed sized chunks
 */
typedef struct {
  /* Capacity of each chunk buffer */
  size_t capacity;
  /* Root of list of chunk buffers. The root is dummy and its data
     member is always NULL. */
  spdylay_buffer_chunk root;
  /* Points to the current chunk to write */
  spdylay_buffer_chunk *current;
  /* Total length of this buffer */
  size_t len;
  /* Offset of last chunk buffer */
  size_t last_offset;
} spdylay_buffer;

/*
 * Initializes buffer with fixed chunk size chunk_capacity.
 */
void spdylay_buffer_init(spdylay_buffer *buffer, size_t chunk_capacity);
/* Releases allocated memory for buffer */
void spdylay_buffer_free(spdylay_buffer *buffer);
/* Returns buffer pointer */
uint8_t* spdylay_buffer_get(spdylay_buffer *buffer);
/* Returns available buffer length */
size_t spdylay_buffer_avail(spdylay_buffer *buffer);
/* Advances buffer pointer by amount. This reduces available buffer
   length. */
void spdylay_buffer_advance(spdylay_buffer *buffer, size_t amount);
/*
 * Allocate new chunk buffer. This will increase total length of
 * buffer (returned by spdylay_buffer_length) by capacity-last_offset.
 * It means untouched buffer is assumued to be written.
 *
 * This function returns 0 if it succeeds, or one of the following
 * negative eror codes:
 *
 * SPDYLAY_ERR_NOMEM
 *     Out of memory.
 */
int spdylay_buffer_alloc(spdylay_buffer *buffer);

/* Returns total length of buffer */
size_t spdylay_buffer_length(spdylay_buffer *buffer);

/* Returns capacity of each fixed chunk buffer */
size_t spdylay_buffer_capacity(spdylay_buffer *buffer);

/* Stores the contents of buffer into |buf|. |buf| must be at least
   spdylay_buffer_length(buffer) bytes long. */
void spdylay_buffer_serialize(spdylay_buffer *buffer, uint8_t *buf);

/* Reset |buffer| for reuse.  Set the total length of buffer to 0.
   Next spdylay_buffer_avail() returns 0. This function does not free
   allocated memory space; they are reused. */
void spdylay_buffer_reset(spdylay_buffer *buffer);

#endif /* SPDYLAY_BUFFER_H */
