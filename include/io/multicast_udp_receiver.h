#ifndef PACKAGE_MULTICAST_UDP_SOCKET_LIBRARY_H
#define PACKAGE_MULTICAST_UDP_SOCKET_LIBRARY_H
//=========== Linux ==============//
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/if.h> //struct ifreq
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <linux/net_tstamp.h> //timestamp interface
#include<netdb.h>
//============ C++ ================//
#include <system_error>
#include <string>
#include <cstring>
#include <iostream>
#include <thread>
#include <functional>

namespace io::network {
    struct udp_hdr {
        std::uint16_t source;
        std::uint16_t dest;
        std::uint16_t len;
        std::uint16_t check;
    };
    struct ip_hdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
        std::uint8_t    ihl:4,
                version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
        std::uint8_t    version:4,
  		                ihl:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
        std::uint8_t tos;
        std::uint16_t tot_len;
        std::uint16_t id;
        std::uint16_t frag_off;
        std::uint8_t ttl;
        std::uint8_t protocol;
        std::uint16_t check;
        std::uint32_t saddr;
        std::uint32_t daddr;
    };
    struct eth_hdr {
        static constexpr std::uint16_t eth_len = 6;	/* Octets in one ethernet addr	 */
        unsigned char	h_dest[eth_len];	/* destination eth addr	*/
        unsigned char	h_source[eth_len];	/* source ether addr	*/
        __be16		h_proto;		/* packet type ID field	*/
    };
    struct block_desc {
        uint32_t version;
        uint32_t offset_to_priv;
        tpacket_hdr_v1 h1;
    };
    struct ring {
        iovec *rd; //readerss descriptos
        uint8_t *map;
        tpacket_req3 req;
    };
    struct _socket {
        int packet_version = TPACKET_V3; //packet version
        int fd; //socket file descriptor
        struct sockaddr_ll addr; //struct com para o bind na interface e  protocolo
    };
    struct udp_packet {
        sockaddr_ll *sock_address;
        eth_hdr* eth;
        ip_hdr* ip;
        udp_hdr* udp;
        char *data;
        uint64_t timestamp_ns;
    };
    enum class ErrorCode
    {
        bind_interface_error = 0x01,
    };

    static const char* BIND_ERROR_DESC = "Error to bind to interface: %s\n";

    class multicast_udp_receiver {
    public:
        multicast_udp_receiver(std::string __interface, std::string __address, short __port) {
            init_socket();
            v3_fill();
            setup_ring_buffer();
            create_mmap();
            config_poll();
            _M_interface = __interface;
            _M_address = __address;
            _M_port = __port;
            if (!bind_to_interface(_M_interface))
            {
                char msg[500];
                sprintf(msg, BIND_ERROR_DESC, _M_interface.c_str());
                throw std::system_error(static_cast<int>(ErrorCode::bind_interface_error), std::generic_category(),
                                        msg);
            }
        }

        void set_output(std::function<void(const udp_packet &)> __output) {
            _M_output = __output;
        }

        bool join_to_group()
        {
            return join_to_group(_M_interface, _M_address, _M_port);
        }

        bool leave_group()
        {
            return leave_group(_M_interface, _M_address);
        }

        void start() {
            _M_is_running = true;
            _M_thread.reset(new std::thread(
                    [&]() {
                        read_async();
                    }
            ));
        }

        void stop() {
            _M_is_running = false;
            teardown_socket();
            if (_M_thread->joinable()) {
                _M_thread->join();
            }
        }

        ~multicast_udp_receiver() {
        }

    private:

        bool leave_group(const std::string __interface, const std::string __ipv4) {

            auto index = get_inteface_index(__interface);
            auto bip = get_binary_ip(__ipv4);
            if(bip == -1 || index == -1)
            {
                return false;
            }
            return leave_group(index, ntohl(bip));
        }

        bool bind_to_interface(const std::string &__interface) {
            struct ifreq s_ifr;
            strcpy(s_ifr.ifr_ifrn.ifrn_name, __interface.c_str());
            ioctl(_M_sock.fd, SIOCGIFINDEX, &s_ifr);

            _M_sock.addr.sll_family = AF_PACKET;
            _M_sock.addr.sll_protocol = htons(ETH_P_IP);
            _M_sock.addr.sll_ifindex = s_ifr.ifr_ifru.ifru_ivalue; //index of interface
            _M_sock.addr.sll_pkttype = PACKET_MULTICAST;

            if (bind(_M_sock.fd, (struct sockaddr *) &(_M_sock.addr), sizeof(struct sockaddr_ll))) {
                return false;
            }
            return true;
        }

        bool join_to_group(const std::string __interface, const std::string __ipv4, std::uint16_t __port) {
            _M_interface = __interface;
            _M_port = __port;
            auto index = get_inteface_index(__interface);
            auto bip = get_binary_ip(__ipv4);
            _M_group = bip;
            if(bip == -1 || index == -1)
            {
                return false;
            }
            return join_to_group(index, ntohl(bip));
        }

        void ipv4MulticastToMac(uint32_t ipv4Multicast, unsigned char macAddress[6]) {
            // O prefixo para endereços MAC multicast é 01:00:5E
            macAddress[0] = 0x01;
            macAddress[1] = 0x00;
            macAddress[2] = 0x5E;
            // Os bits 23 a 0 do endereço IPv4 são usados para formar os bits 39 a 16 do endereço MAC
            uint32_t macSuffix = (ipv4Multicast & 0x007FFFFF) | 0x00800000;
            // Os bits 7 a 0 do endereço IPv4 são usados para formar os bits 47 a 40 do endereço MAC
            macAddress[3] = (macSuffix >> 16) & 0xFF;
            // Os bits 31 a 24 do endereço IPv4 são usados para formar os bits 55 a 48 do endereço MAC
            macAddress[4] = (macSuffix >> 8) & 0xFF;
            // Os bits 39 a 32 do endereço IPv4 são usados para formar os bits 63 a 56 do endereço MAC
            macAddress[5] = macSuffix & 0xFF;
        }

        int get_binary_ip(const std::string& __ipv4)
        {
            int ip;
            if (inet_pton(AF_INET, __ipv4.c_str(), &ip) <= 0) {
                return -1;
            }
            return ip;
        }

        int get_inteface_index(const std::string& __interface)
        {
            struct ifreq s_ifr;
            strcpy(s_ifr.ifr_ifrn.ifrn_name, __interface.c_str());
            if (ioctl(_M_sock.fd, SIOCGIFINDEX, &s_ifr) < 0) {
                return -1;
            }
            return  s_ifr.ifr_ifru.ifru_ivalue;
        }

        bool join_to_group(uint32_t __interface_index, uint32_t __ipv4) {
            packet_mreq req;
            req.mr_ifindex = __interface_index;
            req.mr_type = PACKET_MR_MULTICAST;
            req.mr_alen = 0x06;
            ipv4MulticastToMac((__ipv4), req.mr_address);
            printf("Join to: %02x:%02x:%02x:%02x:%02x:%02x\n",
                   req.mr_address[0], req.mr_address[1], req.mr_address[2],
                   req.mr_address[3], req.mr_address[4], req.mr_address[5]);
            if (setsockopt(_M_sock.fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &req, sizeof(packet_mreq))) {
                return false;
            }
            return true;
        }

        bool leave_group(uint32_t __interface_index, short __ipv4) {
            packet_mreq req;
            req.mr_ifindex = __interface_index;
            req.mr_type = PACKET_MR_MULTICAST;
            req.mr_alen = 0x06;
            ipv4MulticastToMac(__ipv4, req.mr_address);
            if (setsockopt(_M_sock.fd, SOL_PACKET, PACKET_DROP_MEMBERSHIP, &req, sizeof(packet_mreq))) {
                return false;
            }
            return true;
        }

        std::error_code init_socket() {
            _M_sock.fd = ::socket(PF_PACKET, SOCK_RAW, htons(0x00));
            if (_M_sock.fd == -1) {
                return std::make_error_code(std::errc::bad_file_descriptor);
            }
            if (setsockopt(_M_sock.fd, SOL_PACKET, PACKET_VERSION, &(_M_sock.packet_version),
                           sizeof(_M_sock.packet_version))) {
                return std::make_error_code(std::errc::invalid_argument);
            }
            int req = 1;
            if (setsockopt(_M_sock.fd, SOL_PACKET, PACKET_TIMESTAMP, (void *) &req, sizeof(req))) {
                return std::make_error_code(std::errc::invalid_argument);
            }
            return std::error_code();
        }

        bool setup_ring_buffer() {
            if (setsockopt(_M_sock.fd, SOL_PACKET, PACKET_RX_RING, (void *) &(_M_ring.req), sizeof(_M_ring.req))) {
                return false;
            }
            return true;
        }

        void v3_fill() {
            memset(&_M_ring.req, 0, sizeof(_M_ring.req));
            _M_ring.req.tp_retire_blk_tov = 64;
            _M_ring.req.tp_sizeof_priv = 0;
            _M_ring.req.tp_feature_req_word = TP_FT_REQ_FILL_RXHASH;
            _M_ring.req.tp_block_size = getpagesize() << 2;
            _M_ring.req.tp_frame_size = TPACKET_ALIGNMENT << 7;
            _M_ring.req.tp_block_nr = 1 << 10;
            _M_ring.req.tp_frame_nr = _M_ring.req.tp_block_size /
                                      _M_ring.req.tp_frame_size *
                                      _M_ring.req.tp_block_nr;
        }

        void create_mmap() {
            _M_ring.map = static_cast<uint8_t *>(mmap(NULL, _M_ring.req.tp_block_size * _M_ring.req.tp_block_nr,
                                                      PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, _M_sock.fd, 0));
            if (_M_ring.map == MAP_FAILED) {
                return;
            }
            _M_ring.rd = static_cast<iovec *>(malloc(_M_ring.req.tp_block_nr * sizeof(*_M_ring.rd)));
            for (unsigned int i = 0; i < _M_ring.req.tp_block_nr; ++i) {
                _M_ring.rd[i].iov_base = _M_ring.map + (i * _M_ring.req.tp_block_size);
                _M_ring.rd[i].iov_len = _M_ring.req.tp_block_size;
            }
        }

        void config_poll() {
            memset(&_M_pfd, 0, sizeof(_M_pfd));
            _M_pfd.fd = _M_sock.fd;
            _M_pfd.events = POLLIN | POLLERR;
            _M_pfd.revents = 0;
        }

        void update_statistics() {
            tpacket_stats_v3 stats;
            socklen_t len = sizeof(stats);
            getsockopt(_M_sock.fd, SOL_PACKET, PACKET_STATISTICS, &stats, &len);
            printf("\nReceived %u packets,  %u dropped, freeze_q_cnt: %u\n",
                   stats.tp_packets, stats.tp_drops,
                   stats.tp_freeze_q_cnt);
        }

        void read_async() {
            block_desc *pbd;
            unsigned int block_num = 0;

            while (_M_is_running) {
                pbd = (struct block_desc *) _M_ring.rd[block_num].iov_base;
                if ((pbd->h1.block_status & TP_STATUS_USER) == 0x00) {
                    poll(&_M_pfd, 1, -1);
                    continue;
                }
                walk_block(pbd);
                flush_block(pbd);
                block_num = (block_num + 1) % _M_ring.req.tp_block_nr;
            }
        }

        void walk_block(block_desc *pbd) {
            auto num_pkts = pbd->h1.num_pkts;
            auto ppd = (tpacket3_hdr *) ((uint8_t *) pbd + pbd->h1.offset_to_first_pkt);


            for (unsigned int i = 0; i < num_pkts; ++i) {
                filter_and_dispatch_packet(ppd);
                ppd = (tpacket3_hdr *) ((uint8_t *) ppd + ppd->tp_next_offset);
            }
        }

        void filter_and_dispatch_packet(tpacket3_hdr *ppd) {
            constexpr int UDP_PROTOCOL = 17;
            clock_gettime(CLOCK_MONOTONIC, &_timer);
            sockaddr_ll *src_addr = (sockaddr_ll *) ((uint8_t *) ppd + sizeof(tpacket3_hdr));
            auto eth = (eth_hdr *) ((uint8_t *) ppd + ppd->tp_mac);
            auto ip = (ip_hdr *) ((uint8_t *) eth + ETH_HLEN);
            auto udp = (udp_hdr *) ((uint8_t *) ip + sizeof(ip_hdr));
            char *payload = (char *) ((uint8_t *) udp + sizeof(udp_hdr));

            if (((src_addr->sll_pkttype == PACKET_OUTGOING  && !_M_loopback) &&
                    src_addr->sll_pkttype != PACKET_MULTICAST) ||
                    !(ntohs(eth->h_proto) == ETH_P_IP &&
                      ip->protocol == UDP_PROTOCOL &&
                      ip->daddr == _M_group &&
                      ntohs(udp->dest) == _M_port)
                    ) {
                return;
            }


            udp_packet packet{.sock_address = src_addr,
                    .eth = eth,
                    .ip = ip,
                    .udp = udp,
                    .data = payload};
            packet.timestamp_ns = (uint64_t)_timer.tv_nsec;

            _M_output(packet);
        }

        void flush_block(block_desc *pbd) {
            pbd->h1.block_status = TP_STATUS_KERNEL;
        }

        void teardown_socket() {
            munmap(_M_ring.map, _M_ring.req.tp_block_size * _M_ring.req.tp_block_nr);
            free(_M_ring.rd);
            close(_M_sock.fd);
        }

        bool _M_init = false;
        bool _M_loopback = false;
        bool _M_is_running = false;
        _socket _M_sock;
        ring _M_ring;
        pollfd _M_pfd;
        std::unique_ptr<std::thread> _M_thread;
        std::function<void(const udp_packet &)> _M_output;
        std::uint32_t _M_group;
        std::uint16_t _M_port;
        std::string _M_interface;
        std::string _M_address;
        struct timespec _timer;
    };
}
#endif //PACKAGE_MULTICAST_UDP_SOCKET_LIBRARY_H

