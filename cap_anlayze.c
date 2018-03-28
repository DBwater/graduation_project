#include <linux/ip.h>
#include <stdio.h>
#include <linux/tcp.h>
#include <string.h>
#include "cap_public.h"

enum {
	PARA_cap_path = 1,
	PARA_save_path,
	PARA_Max
};
int main(int argc, char* argv[])
{
	struct cap_header file_head = { 0, };
	struct cap_item item_head= { 0, };	
	struct EthernetPacket eth_head = { 0, };
	struct iphdr ip_head = { 0, };
	struct tcphdr tcp_head = { 0, };
	//unsigned self_mac[MAC_LEN],
	char pkt[2048];
	FILE *cap_file;
	if (argc != PARA_Max) {
		return printf("Userage ./cap_anlayze <cap_path> <save_path>\n");
	}
	if (NULL == (cap_file = fopen(argv[PARA_cap_path], "rb"))) {
		return printf("Error: open %s error.\n", argv[PARA_cap_path]);
	}

	fread(&file_head, 1, sizeof(file_head), cap_file);
	
	while (sizeof(item_head) == fread(&item_head, 1, sizeof(item_head), cap_file)) {
		memset(pkt, 0, sizeof(pkt));
		fread(pkt, 1, item_head.save_len, cap_file);
		char *info = pkt;
		//eth头部
		memcpy(&eth_head, info,sizeof(eth_head));
		info += sizeof(eth_head);
		//ip头部
		struct iphdr *ip_hdr = (void*)info;
		memcpy(&ip_head, info, ip_hdr->ihl*4);
		info += ip_hdr->ihl*4;
		//tcp头部
		struct tcphdr *tcp_hdr = (void*)info;
		memcpy(&tcp_head, info, (tcp_hdr->doff) * 4);
		info += tcp_hdr->doff * 4;
		//携带的信息
		printf("%s\n", info);
	}
}
