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
```$xslt
if(len(SDS)>1024*1024){
    
}else{
}
```
2)惰性空间释放<br>

