#ifndef __PROCESS_PCAP__
#define __PROCESS_PCAP__

#include <cstring>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include <cstring>
#include <iostream>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <pcap/pcap.h>
#include <unistd.h>

#define NANOSECONDS_PER_SECOND 1000000000L

struct PcapResults {
  std::function<void(const u_char *, std::size_t)> output;
  std::function<void(std::string)> error;
};

struct PcapReader {
  PcapReader(const std::filesystem::path &p, PcapResults output)
      : _output(output), _path(p.c_str()) {}

  void start() {
    int ifindex = 0;
    int loopback = 0;
    double speed = 1;
    int interval = -1;
    int repeat = 1;
    int ttl = -1;
    int broadcast = 0;

    timespec deadline = {};
    if (clock_gettime(CLOCK_MONOTONIC, &deadline) == -1) {
      std::cerr << "clock_gettime: " << strerror(errno) << std::endl;
      return;
    }
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline_with_tstamp_precision(
        _path.c_str(), PCAP_TSTAMP_PRECISION_NANO, errbuf);

    if (handle == nullptr) {
      std::cerr << "pcap_open: " << errbuf << std::endl;
      //    return 1;
    }

    timespec start = {-1, -1};
    timespec pcap_start = {-1, -1};

    pcap_pkthdr header;
    const u_char *p;
    while ((p = pcap_next(handle, &header))) {
      if (start.tv_nsec == -1) {
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
          std::cerr << "clock_gettime: " << strerror(errno) << std::endl;
          return;
        }
        pcap_start.tv_sec = header.ts.tv_sec;
        pcap_start.tv_nsec =
            header.ts.tv_usec; // Note PCAP_TSTAMP_PRECISION_NANO
      }
      if (header.len != header.caplen) {
        continue;
      }
      auto eth = reinterpret_cast<const ether_header *>(p);

      // jump over and ignore vlan tags
      while (ntohs(eth->ether_type) == ETHERTYPE_VLAN) {
        p += 4;
        eth = reinterpret_cast<const ether_header *>(p);
      }
      if (ntohs(eth->ether_type) != ETHERTYPE_IP) {
        continue;
      }
      auto ip = reinterpret_cast<const struct ip *>(p + sizeof(ether_header));
      if (ip->ip_v != 4) {
        continue;
      }
      if (ip->ip_p != IPPROTO_UDP) {
        continue;
      }
      auto udp = reinterpret_cast<const udphdr *>(p + sizeof(ether_header) +
                                                  ip->ip_hl * 4);
      if (interval != -1) {
        // Use constant packet rate
        deadline.tv_sec += interval / 1000L;
        deadline.tv_nsec += (interval * 1000000L) % NANOSECONDS_PER_SECOND;
      } else {
        // Next packet deadline = start + (packet ts - first packet ts) * speed
        int64_t delta =
            (header.ts.tv_sec - pcap_start.tv_sec) * NANOSECONDS_PER_SECOND +
            (header.ts.tv_usec -
             pcap_start.tv_nsec); // Note PCAP_TSTAMP_PRECISION_NANO
        if (speed != 1.0) {
          delta *= speed;
        }
        deadline = start;
        deadline.tv_sec += delta / NANOSECONDS_PER_SECOND;
        deadline.tv_nsec += delta % NANOSECONDS_PER_SECOND;
      }

      // Normalize timespec
      if (deadline.tv_nsec > NANOSECONDS_PER_SECOND) {
        deadline.tv_sec++;
        deadline.tv_nsec -= NANOSECONDS_PER_SECOND;
      }

      timespec now = {};
      if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
        std::cerr << "clock_gettime: " << strerror(errno) << std::endl;
        return;
      }

      if (deadline.tv_sec > now.tv_sec ||
          (deadline.tv_sec == now.tv_sec && deadline.tv_nsec > now.tv_nsec)) {
        if (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline,
                            nullptr) == -1) {
          std::cerr << "clock_nanosleep: " << strerror(errno) << std::endl;
          return;
        }
      }

#ifdef __GLIBC__
      ssize_t len = ntohs(udp->len) - 8;
#else
      ssize_t len = ntohs(udp->uh_ulen) - 8;
#endif
      const u_char *d =
          &p[sizeof(ether_header) + ip->ip_hl * 4 + sizeof(udphdr)];

      _output.output(d, len);
    }

    pcap_close(handle);
  }

private:
  PcapResults _output;
  std::string _path;
};
#endif