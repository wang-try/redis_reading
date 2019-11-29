简单动态字符串
================
Redis 没有直接使用c语言传统的字符串表示，而是自己构建了一种名为简单动态字符串（simple dynamic string,SDS）的抽象类型，并将SDS用作Redis的默认字符串表示.

## SDS的结构
```c
truct sdshdr {
    
    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[];
};
```
(结构体所占用的内存空间就是每个成员对齐后存放时所占用的字节数之和)
- example<br>

![demo](https://raw.githubusercontent.com/wang-try/redis_reading/master/picture/SDS_demo.png)
###优点
1)常数复杂度获取字符串长度<br>
2)杜绝缓冲区溢出<br>
3)减少修改字符串时带来的内存分配次数<br>
4)二进制安全<br>
5)兼容部分c字符串函数<br>
###内存分配策略
1)空间预分配<br>
```c
sds sdsMakeRoomFor(sds s, size_t addlen) {

    struct sdshdr *sh, *newsh;

    // 获取 s 目前的空余空间长度
    size_t free = sdsavail(s);

    size_t len, newlen;

    // s 目前的空余空间已经足够，无须再进行扩展，直接返回
    if (free >= addlen) return s;

    // 获取 s 目前已占用空间的长度
    len = sdslen(s);
    sh = (void*) (s-(sizeof(struct sdshdr)));

    // s 最少需要的长度
    newlen = (len+addlen);

    // 根据新长度，为 s 分配新空间所需的大小
    if (newlen < SDS_MAX_PREALLOC)
        // 如果新长度小于 SDS_MAX_PREALLOC 
        // 那么为它分配两倍于所需长度的空间
        newlen *= 2;
    else
        // 否则，分配长度为目前长度加上 SDS_MAX_PREALLOC
        newlen += SDS_MAX_PREALLOC;
    // T = O(N)
    newsh = zrealloc(sh, sizeof(struct sdshdr)+newlen+1);//释放旧的sh，重新分配新的内存，大小为sizeof(struct sdshdr)+newlen+1

    // 内存不足，分配失败，返回
    if (newsh == NULL) return NULL;

    // 更新 sds 的空余长度
    newsh->free = newlen - len;

    // 返回 sds
    return newsh->buf;
}
```
2)惰性空间释放<br>
```c
  void sdsclear(sds s) {
  
      // 取出 sdshdr
      struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
  
      // 重新计算属性
      sh->free += sh->len;
      sh->len = 0;
  
      // 将结束符放到最前面（相当于惰性地删除 buf 中的内容）
      sh->buf[0] = '\0';
  }

```
