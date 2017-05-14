/*
 * 网络设备信息获取
 */

#include <common.h>
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char g_eth_name[16];
char g_mac_addr[6];

int g_ip_addr;
int g_subnet_mask;
int g_broadcast_ip_addr;

// 初始化网络，获取当前网络设备信息
static void init_net(const char *name)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        my_err("socket", __LINE__);
    }

    struct ifreq ifr;
    strcpy(g_eth_name, name);
    strcpy(ifr.ifr_name, g_eth_name);
    printf("eth name：\t%s\n", g_eth_name);

    // 获取并打印网卡地址
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
    {
        my_err("ioctl mac", __LINE__);
    }
    memcpy(g_mac_addr, ifr.ifr_hwaddr.sa_data, 6);
    printf("local mac：\t");
    for (int i = 0; i < 6; ++i)
    {
        printf("%02hhx%c", g_mac_addr[i], "\n:"[i < 5]);
    }

    // 获取并打印IP地址
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        my_err("ioctl ip", __LINE__);
    }
    struct sockaddr_in sin;
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    g_ip_addr = sin.sin_addr.s_addr;
    printf("local ip：\t%s\n", inet_ntoa(sin.sin_addr));

    // 获取并打印广播地址
    if (ioctl(sock, SIOCGIFBRDADDR, &ifr) < 0)
    {
        my_err("ioctl broadcast", __LINE__);
    }
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    g_broadcast_ip_addr = sin.sin_addr.s_addr;
    printf("broadcast：\t%s\n", inet_ntoa(sin.sin_addr));

    // 获取并打印子网掩码
    if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0)
    {
        my_err("ioctl mask", __LINE__);
    }
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    g_subnet_mask = sin.sin_addr.s_addr;
    printf("eth mask：\t%s\n", inet_ntoa(sin.sin_addr));

    close(sock);
}

int main(int argc, char *argv[])
{
    // initialize
    init_net("lo");
//    init_net("enp5s0");
    init_net("wlp3s0");

    // do sth.

    return 0;
}