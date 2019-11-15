简单动态字符串
================
Redis 没有直接使用c语言传统的字符串表示，而是自己构建了一种名为简单动态字符串（simple dynamic string,SDS）的抽象类型，并将SDS用作Redis的默认字符串表示.

## SDS的定义