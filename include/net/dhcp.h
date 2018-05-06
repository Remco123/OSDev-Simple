#ifndef MYOS__NET__DHCP_H
#define MYOS__NET__DHCP_H

#include <common/types.h>
#include <net/ipv4.h>
#include <net/udp.h>
#include <memorymanagement.h>
#include <common/memfunc.h>

namespace myos
{
    namespace net
    {
        struct DhcpHeader
        {
            common::uint8_t opcode;
            common::uint8_t htype;
            common::uint8_t hlen;
            common::uint8_t hopCount;
            common::uint32_t xid;
            common::uint16_t secCount;
            common::uint16_t flags;
            Ipv4Addr clientIpAddr;
            Ipv4Addr yourIpAddr;
            Ipv4Addr serverIpAddr;
            Ipv4Addr gatewayIpAddr;
            common::uint32_t clientEthAddr;
            common::uint8_t reserved[10];
            char serverName[64];
            char bootFilename[128];
        } __attribute__((packed));

        #define OP_REQUEST                      1
        #define OP_REPLY                        2

        #define HTYPE_ETH                       1

        #define MAGIC_COOKIE                    0x63825363

        #define OPT_PAD                         0
        #define OPT_SUBNET_MASK                 1
        #define OPT_ROUTER                      3
        #define OPT_DNS                         6
        #define OPT_REQUESTED_IP_ADDR           50
        #define OPT_LEASE_TIME                  51
        #define OPT_DHCP_MESSAGE_TYPE           53
        #define OPT_SERVER_ID                   54
        #define OPT_PARAMETER_REQUEST           55
        #define OPT_END                         255

        #define PORT_BOOTP_SERVER               67
        #define PORT_BOOTP_CLIENT 68

        struct DhcpOptions
        {
            const Ipv4Addr *subnetMask;
            const Ipv4Addr *routerList;
            const Ipv4Addr *routerEnd;
            const Ipv4Addr *dnsList;
            const Ipv4Addr *dnsEnd;
            const Ipv4Addr *requestedIpAddr;
            common::uint32_t leaseTime;
            common::uint32_t messageType;
            const Ipv4Addr *serverId;
            const common::uint8_t *parameterList;
            const common::uint8_t *parameterEnd;
        } __attribute__((packed));

        // ------------------------------------------------------------------------------------------------
        // Message Types

        #define DHCP_DISCOVER                   1
        #define DHCP_OFFER                      2
        #define DHCP_REQUEST                    3
        #define DHCP_DECLINE                    4
        #define DHCP_ACK                        5
        #define DHCP_NAK                        6
        #define DHCP_RELEASE                    7
        #define DHCP_INFORM 8

        class DHCP : public UserDatagramProtocolHandler
        {
        public:
            DHCP();
            ~DHCP();
            void HandleUserDatagramProtocolMessage(common::uint8_t* data, common::uint16_t size);
            void DhcpDiscover(common::uint32_t clientAddr, UserDatagramProtocolSocket* socket);
        };
    }
}

#endif