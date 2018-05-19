#ifndef CAP_PUBLIC_H
#define CAP_PUBLIC_H

/************************************************************************************
定义业界通用的公共数据结构
************************************************************************************/

/* CAP文件头格式定义 (24字节)*/
struct cap_header {							/* CAP文件头格式定义 */
    unsigned int magic;						/* ==0xa1b2c3d4 */
    short version_major;					/* NDS只支持:2 */
    short version_minor;					/* NDS只支持:4 */
    int zone;                                           /* gmt to local correct */
    int timestamps;                             /* accuracy of time tamps */
    int snaplen;                                    /* max length saved portion of pkt */
    int linktype;                                   /* LINKTYPE_* */
};

/* 报文头格式定义 (16字节) */
struct cap_item {                               	/* 报文头格式定义 */
    unsigned int sec;                           /* time stamp */
    unsigned int usec;                         /* time stamp */
    unsigned int save_len;    				/* present length */
    unsigned int wire_len;    				/* wire length */
};

/*帧头(14byte)结构为*/
struct EthernetPacket
{
    char mac_dst[6];                            /* 目的网卡物理地址 */
    char mac_src[6];                            /* 源网卡物理地址   */
    unsigned short PacketType;      /* 包类型， ip或ARP等*/
};

#endif // CAP_PUBLIC_H
