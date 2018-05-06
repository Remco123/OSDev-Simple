#include <net/dhcp.h>

using namespace myos;
using namespace myos::net;
using namespace myos::common;

void printf(char*);

DHCP::DHCP()
{

}
DHCP::~DHCP()
{

}
void DHCP::HandleUserDatagramProtocolMessage(common::uint8_t* data, common::uint16_t size)
{
    printf("Got DHCP Response");
}
void DHCP::DhcpDiscover(uint32_t clientAddr, UserDatagramProtocolSocket* socket)
{
    printf("Starting DHCP Discover");
    uint8_t* packet = (uint8_t*)MemoryManager::activeMemoryManager->malloc(512);;

    DhcpHeader* header = (DhcpHeader*)packet;
    //Build Header
    header->opcode = OP_REQUEST;
    header->htype = HTYPE_ETH;
    header->hlen = sizeof(uint32_t);
    header->hopCount = 0;
    header->xid = 0;
    header->secCount = 0;
    header->flags = 0;
    header->clientEthAddr = clientAddr;

    uint8_t* msg = packet + sizeof(DhcpHeader);
    *(uint32_t*)msg = bigEndian32(MAGIC_COOKIE);
    msg += 4;

    *msg++ = OPT_DHCP_MESSAGE_TYPE;
    *msg++ = 1;
    *msg++ = DHCP_DISCOVER;

    // Parameter Request list
    *msg++ = OPT_PARAMETER_REQUEST;
    *msg++ = 3;
    *msg++ = OPT_SUBNET_MASK;
    *msg++ = OPT_ROUTER;
    *msg++ = OPT_DNS;

    // Option End
    *msg++ = OPT_END;

    socket->Send(msg, 512);
    printf("Packet Send");
}