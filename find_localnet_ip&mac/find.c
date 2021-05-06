#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>

static void set_socket(char *interface, int interface_flag, struct ifreq *ifr);
static void set_str_ip(char *interface, char *str);
static void set_str_mac(char *interface, char *str);

static void set_socket(char *interface, int interface_flag, struct ifreq *ifr)
{
    int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strcpy(ifr->ifr_name, interface);

    ioctl(s, interface_flag, ifr);

    close(s);
}

static void set_str_ip(char *interface, char *str)
{
    struct ifreq ifr;
    
    set_socket(interface, SIOCGIFADDR, &ifr);

    // printf("%s\n",inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
    sprintf(str, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}

static void set_str_mac(char *interface, char *str)
{
    struct ifreq ifr;
    int i;

    set_socket(interface, SIOCGIFHWADDR, &ifr);

    // for(i = 0; i < 5; i++)
    //     printf("%.2x:", ifr.ifr_hwaddr.sa_data[i] & 0xff);
    // printf("%.2x\n", ifr.ifr_hwaddr.sa_data[i] & 0xff);

    for(i = 0; i < 5; i++)
        sprintf(&str[i * 3], "%.2x:", ifr.ifr_hwaddr.sa_data[i] & 0xff);
    sprintf(&str[i * 3], "%.2x", ifr.ifr_hwaddr.sa_data[i] & 0xff);
}

int main(int argc, char *argv[])
{
    char interface[] = "ens33";
    // char interface[] = "enx9cebe8291e81";

    char mac[18];
    char ip[16];

    set_str_mac(interface, mac);
    set_str_ip(interface, ip);

    printf("%s\n", mac);
    printf("%s\n", ip);

    return 0;
}
