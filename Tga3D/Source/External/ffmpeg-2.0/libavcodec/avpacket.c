/*
 * AVPacket functions for libavcodec
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <string.h>

#include "libavutil/avassert.h"
#include "libavutil/common.h"
#include "libavutil/mem.h"
#include "avcodec.h"
#include "bytestream.h"
#include "internal.h"

void ff_packet_free_side_data(AVPacket *pkt)
{
    int i;
    for (i = 0; i < pkt->side_data_elems; i++)
        av_free(pkt->side_data[i].data);
    av_freep(&pkt->side_data);
    pkt->side_data_elems = 0;
}

#if FF_API_DESTRUCT_PACKET
void av_destruct_packet(AVPacket *pkt)
{
    av_free(pkt->data);
    pkt->data = NULL;
    pkt->size = 0;
}

/* a dummy destruct callback for the callers that assume AVPacket.destruct ==
 * NULL => static data */
static void dummy_destruct_packet(AVPacket *pkt)
{
    av_assert0(0);
}
#endif

void av_init_packet(AVPacket *pkt)
{
    pkt->pts                  = AV_NOPTS_VALUE;
    pkt->dts                  = AV_NOPTS_VALUE;
    pkt->pos                  = -1;
    pkt->duration             = 0;
    pkt->convergence_duration = 0;
    pkt->flags                = 0;
    pkt->stream_index         = 0;
#if FF_API_DESTRUCT_PACKET
    pkt->destruct             = NULL;
#endif
    pkt->buf                  = NULL;
    pkt->side_data            = NULL;
    pkt->side_data_elems      = 0;
}

int av_new_packet(AVPacket *pkt, int size)
{
    AVBufferRef *buf = NULL;

    if ((unsigned)size >= (unsigned)size + FF_INPUT_BUFFER_PADDING_SIZE)
        return AVERROR(EINVAL);

    av_buffer_realloc(&buf, size + FF_INPUT_BUFFER_PADDING_SIZE);
    if (!buf)
        return AVERROR(ENOMEM);

    memset(buf->data + size, 0, FF_INPUT_BUFFER_PADDING_SIZE);

    av_init_packet(pkt);
    pkt->buf      = buf;
    pkt->data     = buf->data;
    pkt->size     = size;
#if FF_API_DESTRUCT_PACKET
    pkt->destruct = dummy_destruct_packet;
#endif

    return 0;
}

void av_shrink_packet(AVPacket *pkt, int size)
{
    if (pkt->size <= size)
        return;
    pkt->size = size;
    memset(pkt->data + size, 0, FF_INPUT_BUFFER_PADDING_SIZE);
}

int av_grow_packet(AVPacket *pkt, int grow_by)
{
    int new_size;
    av_assert0((unsigned)pkt->size <= INT_MAX - FF_INPUT_BUFFER_PADDING_SIZE);
    if (!pkt->size)
        return av_new_packet(pkt, grow_by);
    if ((unsigned)grow_by >
        INT_MAX - (pkt->size + FF_INPUT_BUFFER_PADDING_SIZE))
        return -1;

    new_size = pkt->size + grow_by + FF_INPUT_BUFFER_PADDING_SIZE;
    if (pkt->buf) {
        int ret = av_buffer_realloc(&pkt->buf, new_size);
        if (ret < 0)
            return ret;
    } else {
        pkt->buf = av_buffer_alloc(new_size);
        if (!pkt->buf)
            return AVERROR(ENOMEM);
        memcpy(pkt->buf->data, pkt->data, FFMIN(pkt->size, pkt->size + grow_by));
#if FF_API_DESTRUCT_PACKET
        pkt->destruct = dummy_destruct_packet;
#endif
    }
    pkt->data  = pkt->buf->data;
    pkt->size += grow_by;
    memset(pkt->data + pkt->size, 0, FF_INPUT_BUFFER_PADDING_SIZE);

    return 0;
}

int av_packet_from_data(AVPacket *pkt, uint8_t *data, int size)
{
    if (size >= INT_MAX - FF_INPUT_BUFFER_PADDING_SIZE)
        return AVERROR(EINVAL);

    pkt->buf = av_buffer_create(data, size + FF_INPUT_BUFFER_PADDING_SIZE,
                                av_buffer_default_free, NULL, 0);
    if (!pkt->buf)
        return AVERROR(ENOMEM);

    pkt->data = data;
    pkt->size = size;
#if FF_API_DESTRUCT_PACKET
    pkt->destruct = dummy_destruct_packet;
#endif

    return 0;
}

#define ALLOC_MALLOC(data, size) data = av_malloc(size)
#define ALLOC_BUF(data, size)                \
do {                                         \
    av_buffer_realloc(&pkt->buf, size);      \
    data = pkt->buf ? pkt->buf->data : NULL; \
} while (0)

#define DUP_DATA(dst, src, size, padding, ALLOC)                        \
    do {                                                                \
        void *data;                                                     \
        if (padding) {                                                  \
            if ((unsigned)(size) >                                      \
                (unsigned)(size) + FF_INPUT_BUFFER_PADDING_SIZE)        \
                goto failed_alloc;                                      \
            ALLOC(data, size + FF_INPUT_BUFFER_PADDING_SIZE);           \
        } else {                                                        \
            ALLOC(data, size);                                          \
        }                                                               \
        if (!data)                                                      \
            goto failed_alloc;                                          \
        memcpy(data, src, size);                                        \
        if (padding)                                                    \
            memset((uint8_t *)data + size, 0,                           \
                   FF_INPUT_BUFFER_PADDING_SIZE);                       \
        dst = data;                                                     \
    } while (0)

/* Makes duplicates of data, side_data, but does not copy any other fields */
static int copy_packet_data(AVPacket *pkt, AVPacket *src, int dup)
{
    pkt->data      = NULL;
    pkt->side_data = NULL;
    if (pkt->buf) {
        AVBufferRef *ref = av_buffer_ref(src->buf);
        if (!ref)
            return AVERROR(ENOMEM);
        pkt->buf  = ref;
        pkt->data = ref->data;
    } else {
        DUP_DATA(pkt->data, src->data, pkt->size, 1, ALLOC_BUF);
    }
#if FF_API_DESTRUCT_PACKET
    pkt->destruct = dummy_destruct_packet;
#endif
    if (pkt->side_data_elems && dup)
        pkt->side_data = src->side_data;
    if (pkt->side_data_elems && !dup) {
        return av_copy_packet_side_data(pkt, src);
    }
    return 0;

failed_alloc:
    av_destruct_packet(pkt);
    return AVERROR(ENOMEM);
}

int av_copy_packet_side_data(AVPacket *pkt, AVPacket *src)
{
    if (src->side_data_elems) {
        int i;
        DUP_DATA(pkt->side_data, src->side_data,
                src->side_data_elems * sizeof(*src->side_data), 0, ALLOC_MALLOC);
        memset(pkt->side_data, 0,
                src->side_data_elems * sizeof(*src->side_data));
        for (i = 0; i < src->side_data_elems; i++) {
            DUP_DATA(pkt->side_data[i].data, src->side_data[i].data,
                    src->side_data[i].size, 1, ALLOC_MALLOC);
            pkt->side_data[i].size = src->side_data[i].size;
            pkt->side_data[i].type = src->side_data[i].type;
        }
    }
    return 0;

failed_alloc:
    av_destruct_packet(pkt);
    return AVERROR(ENOMEM);
}

int av_dup_packet(AVPacket *pkt)
{
    AVPacket tmp_pkt;

    if (!pkt->buf && pkt->data
#if FF_API_DESTRUCT_PACKET
        && !pkt->destruct
#endif
        ) {
        tmp_pkt = *pkt;
        return copy_packet_data(pkt, &tmp_pkt, 1);
    }
    return 0;
}

int av_copy_packet(AVPacket *dst, AVPacket *src)
{
    *dst = *src;
    return copy_packet_data(dst, src, 0);
}

void av_free_packet(AVPacket *pkt)
{
    if (pkt) {
        int i;

        if (pkt->buf)
            av_buffer_unref(&pkt->buf);
#if FF_API_DESTRUCT_PACKET
        else if (pkt->destruct)
            pkt->destruct(pkt);
        pkt->destruct = NULL;
#endif
        pkt->data            = NULL;
        pkt->size            = 0;

        for (i = 0; i < pkt->side_data_elems; i++)
            av_free(pkt->side_data[i].data);
        av_freep(&pkt->side_data);
        pkt->side_data_elems = 0;
    }
}

uint8_t *av_packet_new_side_data(AVPacket *pkt, enum AVPacketSideDataType type,
                                 int size)
{
    int elems = pkt->side_data_elems;

    if ((unsigned)elems + 1 > INT_MAX / sizeof(*pkt->side_data))
        return NULL;
    if ((unsigned)size > INT_MAX - FF_INPUT_BUFFER_PADDING_SIZE)
        return NULL;

    pkt->side_data = av_realloc(pkt->side_data,
                                (elems + 1) * sizeof(*pkt->side_data));
    if (!pkt->side_data)
        return NULL;

    pkt->side_data[elems].data = av_malloc(size + FF_INPUT_BUFFER_PADDING_SIZE);
    if (!pkt->side_data[elems].data)
        return NULL;
    pkt->side_data[elems].size = size;
    pkt->side_data[elems].type = type;
    pkt->side_data_elems++;

    return pkt->side_data[elems].data;
}

uint8_t *av_packet_get_side_data(AVPacket *pkt, enum AVPacketSideDataType type,
                                 int *size)
{
    int i;

    for (i = 0; i < pkt->side_data_elems; i++) {
        if (pkt->side_data[i].type == type) {
            if (size)
                *size = pkt->side_data[i].size;
            return pkt->side_data[i].data;
        }
    }
    return NULL;
}

#define FF_MERGE_MARKER 0x8c4d9d108e25e9feULL

int av_packet_merge_side_data(AVPacket *pkt){
    if(pkt->side_data_elems){
        AVBufferRef *buf;
        int i;
        uint8_t *p;
        uint64_t size= pkt->size + 8LL + FF_INPUT_BUFFER_PADDING_SIZE;
        AVPacket old= *pkt;
        for (i=0; i<old.side_data_elems; i++) {
            size += old.side_data[i].size + 5LL;
        }
        if (size > INT_MAX)
            return AVERROR(EINVAL);
        buf = av_buffer_alloc(size);
        if (!buf)
            return AVERROR(ENOMEM);
        pkt->buf = buf;
        pkt->data = p = buf->data;
#if FF_API_DESTRUCT_PACKET
        pkt->destruct = dummy_destruct_packet;
#endif
        pkt->size = size - FF_INPUT_BUFFER_PADDING_SIZE;
        bytestream_put_buffer(&p, old.data, old.size);
        for (i=old.side_data_elems-1; i>=0; i--) {
            bytestream_put_buffer(&p, old.side_data[i].data, old.side_data[i].size);
            bytestream_put_be32(&p, old.side_data[i].size);
            *p++ = old.side_data[i].type | ((i==old.side_data_elems-1)*128);
        }
        bytestream_put_be64(&p, FF_MERGE_MARKER);
        av_assert0(p-pkt->data == pkt->size);
        memset(p, 0, FF_INPUT_BUFFER_PADDING_SIZE);
        av_free_packet(&old);
        pkt->side_data_elems = 0;
        pkt->side_data = NULL;
        return 1;
    }
    return 0;
}

int av_packet_split_side_data(AVPacket *pkt){
    if (!pkt->side_data_elems && pkt->size >12 && AV_RB64(pkt->data + pkt->size - 8) == FF_MERGE_MARKER){
        int i;
        unsigned int size, orig_pktsize = pkt->size;
        uint8_t *p;

        p = pkt->data + pkt->size - 8 - 5;
        for (i=1; ; i++){
            size = AV_RB32(p);
            if (size>INT_MAX || p - pkt->data < size)
                return 0;
            if (p[4]&128)
                break;
            p-= size+5;
        }

        pkt->side_data = av_malloc(i * sizeof(*pkt->side_data));
        if (!pkt->side_data)
            return AVERROR(ENOMEM);

        p= pkt->data + pkt->size - 8 - 5;
        for (i=0; ; i++){
            size= AV_RB32(p);
            av_assert0(size<=INT_MAX && p - pkt->data >= size);
            pkt->side_data[i].data = av_malloc(size + FF_INPUT_BUFFER_PADDING_SIZE);
            pkt->side_data[i].size = size;
            pkt->side_data[i].type = p[4]&127;
            if (!pkt->side_data[i].data)
                return AVERROR(ENOMEM);
            memcpy(pkt->side_data[i].data, p-size, size);
            pkt->size -= size + 5;
            if(p[4]&128)
                break;
            p-= size+5;
        }
        pkt->size -= 8;
        /* FFMIN() prevents overflow in case the packet wasn't allocated with
         * proper padding.
         * If the side data is smaller than the buffer padding size, the
         * remaining bytes should have already been filled with zeros by the
         * original packet allocation anyway. */
        memset(pkt->data + pkt->size, 0,
               FFMIN(orig_pktsize - pkt->size, FF_INPUT_BUFFER_PADDING_SIZE));
        pkt->side_data_elems = i+1;
        return 1;
    }
    return 0;
}

int av_packet_shrink_side_data(AVPacket *pkt, enum AVPacketSideDataType type,
                               int size)
{
    int i;

    for (i = 0; i < pkt->side_data_elems; i++) {
        if (pkt->side_data[i].type == type) {
            if (size > pkt->side_data[i].size)
                return AVERROR(ENOMEM);
            pkt->side_data[i].size = size;
            return 0;
        }
    }
    return AVERROR(ENOENT);
}
