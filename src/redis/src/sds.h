/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __SDS_H
#define __SDS_H

/*
 * 最大预分配长度
 */
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

/*
 * 类型别名，用于指向 sdshdr 的 buf 属性
 */
typedef char *sds;

/*
 * 保存字符串对象的结构
 */
struct sdshdr {

    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[];//char buf[] 或写成 char buf[0] 柔性数组，使得每个sdshdr对象可以拥有可变大小的数组。buf不占用sdshdr的空间，只是作为一个符号地址存在
};

/*
 * 返回 sds 实际保存的字符串的长度
 *
 * T = O(1)
 */

/*
 * static inline 作用：内联函数的代码会被直接嵌入在它被调用的地方，调用几次就嵌入几次，没有使用call指令。
 * 这样省去了函数调用时的一些额外开销，比如保存和恢复函数返回地址等，可以加快速度。不过调用次数多的话，会使可执行文件变大，这样会降低速度。
 * 相比起宏来说，内核开发者一般更喜欢使用内联函数。因为内联函数没有长度限制，格式限制。编译器还可以检查函数调用方式，以防止其被误用。
 * 不加static，每调用一次函数，都会开辟一块内存空间，使用static就和static修饰变量类似，在此文件内有效，且程序运行期都不会释放，只会开辟一块内存空间。
*/

static inline size_t sdslen(const sds s) {
    //sdsnewlen 返回的是buf成员，s-(sizeof(struct sdshdr))得到的是sdshdr的起始地址
    struct sdshdr *sh = (void *) (s - (sizeof(struct sdshdr)));
    return sh->len;
}

/*
 * 返回 sds 可用空间的长度
 *
 * T = O(1)
 */
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void *) (s - (sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);

sds sdsnew(const char *init);

sds sdsempty(void);

size_t sdslen(const sds s);

sds sdsdup(const sds s);

void sdsfree(sds s);

size_t sdsavail(const sds s);

sds sdsgrowzero(sds s, size_t len);

sds sdscatlen(sds s, const void *t, size_t len);

sds sdscat(sds s, const char *t);

sds sdscatsds(sds s, const sds t);

sds sdscpylen(sds s, const char *t, size_t len);

sds sdscpy(sds s, const char *t);

sds sdscatvprintf(sds s, const char *fmt, va_list ap);

#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else

sds sdscatprintf(sds s, const char *fmt, ...);

#endif

sds sdscatfmt(sds s, char const *fmt, ...);

sds sdstrim(sds s, const char *cset);

void sdsrange(sds s, int start, int end);

void sdsupdatelen(sds s);

void sdsclear(sds s);

int sdscmp(const sds s1, const sds s2);

sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);

void sdsfreesplitres(sds *tokens, int count);

void sdstolower(sds s);

void sdstoupper(sds s);

sds sdsfromlonglong(long long value);

sds sdscatrepr(sds s, const char *p, size_t len);

sds *sdssplitargs(const char *line, int *argc);

sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);

sds sdsjoin(char **argv, int argc, char *sep);

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);

void sdsIncrLen(sds s, int incr);

sds sdsRemoveFreeSpace(sds s);

size_t sdsAllocSize(sds s);

#endif
