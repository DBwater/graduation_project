#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "cap_public.h"
#include <iostream>
#include <linux/ip.h>
#include <stdio.h>
#include <linux/tcp.h>
#include <string.h>
#include<arpa/inet.h>
#include<math.h>
#include <fstream>
#include <sstream>
using namespace std;

void get_ipstr(unsigned long ip,char *ipstr){
   struct in_addr addr;
   addr.s_addr=ip;
   char *str=inet_ntoa(addr);
   strcpy(ipstr,str);
}
//计算信息熵
double calculate_byte(char *str,int len){
    int count[257];
    memset(count,0,sizeof(count));
    for(int i=0;i<len;i++){
        int idx=(int)str[i];
        count[idx+128]++;
    }
    double ans=0;
    for(int i=0;i<257;i++){
        if(count[i]!=0){
            double cnt=((double)count[i]/(double)len)*(log((double)len/(double)count[i])/log(2));
            ans+=cnt;
        }
    }
    return ans;
}
//分析cap包
void analayze_cap(const char *file_addr,const char *save_addr){
    struct cap_header file_head = { 0, };
    struct cap_item item_head= { 0, };
    struct EthernetPacket eth_head = { 0, };
    struct iphdr ip_head = { 0, };
    struct tcphdr tcp_head = { 0, };
    char pkt[20480],dst_ip[20],src_ip[20];
    FILE *cap_file;
    FILE *save_file;
    if (NULL == (cap_file = fopen(file_addr, "rb"))) {
        printf("Error: open %s error.\n", file_addr);
        return;
    }
    fread(&file_head, 1, sizeof(file_head), cap_file);
    if(NULL==(save_file=fopen(save_addr,"w+"))){
        printf("Error: open %s error.\n",save_addr);
        return;
    }
    while (sizeof(item_head) == fread(&item_head, 1, sizeof(item_head), cap_file)) {
        memset(pkt, 0, sizeof(pkt));
        fread(pkt, 1, item_head.save_len, cap_file);
        //pkt[item_head.save_len]='\0';//结束符号
        int data_len=item_head.save_len;
        char *info = pkt;
        //eth头部
        memcpy(&eth_head, info,sizeof(eth_head));
        info += sizeof(eth_head);
        data_len-=sizeof(eth_head);
        //ip头部
        struct iphdr *ip_hdr = (struct iphdr *)info;
        memcpy(&ip_head, info, ip_hdr->ihl*4);
        info += ip_hdr->ihl*4;
        data_len-=ip_hdr->ihl*4;

        get_ipstr(ip_hdr->daddr,dst_ip);
        get_ipstr(ip_hdr->saddr,src_ip);
        double ans=calculate_byte(info,data_len);
        //printf("%s -> %s,   信息熵:%f,  %s\n",src_ip,dst_ip,ans,ans>=5?"加密":"未加密");
        fprintf(save_file,"%s -> %s,   信息熵:%f,  %s\n",src_ip,dst_ip,ans,ans>=5?"加密":"未加密");
        /*for(int i=0;i<data_len;i++){
            printf("%d     ",(char)info[i]);
        }*/

        //printf("\n");
        //tcp头部
        struct tcphdr *tcp_hdr = (struct tcphdr*)info;
        memcpy(&tcp_head, info, (tcp_hdr->doff) * 4);
        info += tcp_hdr->doff * 4;
        //携带的信息
        //printf("%s\n", info);
    }
    fclose(save_file);
    fclose(cap_file);
}

//Qstring和string转换
QString strtoqsconst (string &s)
{
   return QString(QString::fromLocal8Bit(s.c_str()));
}
string qstostr(const QString &s)
{
  return string((const char*)s.toLocal8Bit());
}
//读取文件内容到string
string readFileIntoString(const char * filename)
{
ifstream ifile(filename);
//将文件读入到ostringstream对象buf中
ostringstream buf;
char ch;
while(buf&&ifile.get(ch))
buf.put(ch);
//返回与流对象buf关联的字符串
return buf.str();
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectButton_clicked()
{
    QFileDialog *file= new QFileDialog(this);
    file->setWindowTitle("select file");
    file->setDirectory(".");
    if(file->exec()==QFileDialog::Accepted){
        QString filepath=file->selectedFiles()[0];
       ui->fileAddr->setText(filepath);
    }
}

void MainWindow::on_startButton_clicked()
{
    //ui->fileAddr->text();
  QString text1=ui->fileAddr->text();
  string str=qstostr(text1);
  const char *file_path=str.c_str();
  const char *save_addr="/home/dbwater/data.txt";
 analayze_cap(file_path,save_addr);
 str=readFileIntoString(save_addr);
 QString data = strtoqsconst(str);
 ui->ansEdit->insertPlainText(data);
 printf("ok\n");
}
