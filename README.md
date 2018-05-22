# 毕业设计
```c
struct iphdr {
#if defined(__LITTLE_ENDIAN_BITFIELD) //小端模式下
__u8 ihl:4,//首部长度(4位)
version:4;//ip协议版本IPv4
#elif defined (__BIG_ENDIAN_BITFIELD) //大端模式下
__u8 version:4,
ihl:4;
#else
#error "Please fix <asm/byteorder.h>"
#endif
__u8 tos;//服务类型字段(8位)
__be16 -tot_len;//16位IP数据报总长度
__be16 -id;//16位标识字段（唯一表示主机发送的每一分数据报）
__be16 -frag_off;//(3位分段标志+13位分段偏移数)
__u8 ttl;//8位数据报生存时间
__u8 protocol;//协议字段（8位）
__be16 check;//16位首部校验
__be32 saddr; //源IP地址
__be32 daddr; //目的IP地址
};
```
