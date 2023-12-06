/******************************************************************************
 * @brief Base class for network thread management.
 *
 * @file RoveCommPacket.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2023-10-08
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_SERVER_H
#define ROVECOMM_SERVER_H

#include <future>
#include <map>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <vector>

#include "RoveCommPacket.h"

/******************************************************************************
 * @brief Allow us to add more protocols. Currently only TCP/UDP.
 * In functions that take multiple protocols, write TCP | UDP
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
enum RoveCommNetworkProtocol
{
    TCP = 1 << 0,
    UDP = 2 << 1
};

inline RoveCommNetworkProtocol operator|(RoveCommNetworkProtocol protocol, RoveCommNetworkProtocol other)
{
    return static_cast<RoveCommNetworkProtocol>(static_cast<int>(protocol) | static_cast<int>(protocol));
}

using RoveCommPort     = unsigned int;
using RoveCommCallback = void();

/******************************************************************************
 * @brief Octant of an IP address where FIRST = 1, SECOND = 2, etc.
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-12-01
 ******************************************************************************/
enum RoveCommAddressOctant
{
    FIRST = 0x01,
    SECOND,
    THIRD,
    FOURTH
};

/******************************************************************************
 * @brief Contains the octants of an IP address and a port number
 *
 * @param cOctants[4] - a char array with octants from most to least significant,
 * i.e. "1.2.3.4"
 * @param unPort - the port number
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-29
 ******************************************************************************/
class RoveCommAddress    // should this be a struct?
{
    public:
        RoveCommAddress(char cFirstOctant, char cSecondOctant, char cThirdOctant, char cFourthOctant, RoveCommPort unPort) :
            m_cOctants({cFirstOctant, cSecondOctant, cThirdOctant, cFourthOctant})
        {}

        RoveCommAddress(char cOctants[4], RoveCommPort unPort) : RoveCommAddress(cOctants[0], cOctants[1], cOctants[2], cOctants[3], 0) {}

        RoveCommAddress() : RoveCommAddress(0, 0, 0, 0, 0) {}

        // maybe have one that takes ("1.2.3.4", 0) ?

        inline char GetOctant(RoveCommAddressOctant octant) { return m_cOctants[octant - 1]; }

        inline RoveCommPort GetPort() { return m_unPort; }

        friend inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);
        friend inline bool operator==(RoveCommAddress& address, RoveCommAddress& other);
        friend inline bool operator!=(RoveCommAddress& address, RoveCommAddress& other);

        // for std::map lookups
        inline bool operator<(RoveCommAddress& other) { return this->m_unPort < other.m_unPort; }

    private:
        char m_cOctants[4];
        RoveCommPort m_unPort;

    public:
        // pass to RoveCommServer::Fetch() signifying any address. This is not a valid address.
        const static RoveCommAddress ANY;
};

inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);
inline bool operator==(RoveCommAddress& address, RoveCommAddress& other);
inline bool operator!=(RoveCommAddress& address, RoveCommAddress& other);

/******************************************************************************
 * @brief Base class that can be extended to add more protocols.
 * Each server listens to ONE port on its own ONE network thread.
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-13
 ******************************************************************************/
class RoveCommServer
{
    public:
        RoveCommServer(RoveCommPort unPort) : m_unPort(unPort) {}

        virtual ~RoveCommServer();

        /******************************************************************************
         * @brief Initialize listening socket and start thread
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual void Init();
        /******************************************************************************
         * @brief Close all open sockets and shut down thread
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual void Shutdown();

        /******************************************************************************
         * @brief Write a packet to all subscribers/connections
         *
         * @param packet - the RoveCommPacket to write
         * @return int - the number of bytes written (0 if error)
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual int Write(RoveCommPacket& packet) const;
        /******************************************************************************
         * @brief Write a packet to a single address, but not subscribers/connections.
         * If you want to send a packet to subscribers AND the ip, call SubscribeTo()
         *
         * @param packet - the RoveCommPacket to write
         * @param address - the RoveCommAddress (IP, Port) to send to
         * @return int - the number of bytes written (0 if error)
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual int SendTo(RoveCommPacket& packet, RoveCommAddress address) const;
        /******************************************************************************
         * @brief Read incoming packets and clear queue
         *
         * @return std::vector<RoveCommPacket> - a list of RoveCommPackets. 0 if no packets available.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual std::vector<RoveCommPacket> Read() const;

        /******************************************************************************
         * @brief Synchronously await the next RoveCommPacket with the given data_id.
         * This packet is marked immediately as read and does not get queued for Read()
         *
         * @param unId - the desired id
         * @param address - the desired address
         * @return std::future<RoveCommPacket> - the desired RoveCommPacket
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-01
         ******************************************************************************/
        virtual std::future<RoveCommPacket> Fetch(RoveCommDataId unId = rovecomm::System::ANY, RoveCommAddress address = RoveCommAddress::ANY) const;

        inline const RoveCommPort GetPort() const { return m_unPort; }

    protected:
        const RoveCommPort m_unPort;
        // std::map<RoveCommDataId, RoveCommCallback> m_fCallbacks;
        // std::thread m_thNetworkThread;
        std::queue<RoveCommPacket> m_qPacketCopyQueue;

        std::shared_mutex m_muPoolScheduleMutex;
        std::mutex m_muPacketCopyMutex;
};

/******************************************************************************
 * @brief Handles lifetimes of RoveCommServer's and defer work to them
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
class RoveCommServerManager
{
    public:
        static void OpenServerOnPort(RoveCommPort port = rovecomm::General::ETHERNET_UDP_PORT, RoveCommNetworkProtocol protocol = RoveCommNetworkProtocol::UDP);
        static void Shutdown();
        static int Write(RoveCommPacket& packet, RoveCommNetworkProtocol protocol = RoveCommNetworkProtocol::UDP);
        static int SendTo(RoveCommPacket& packet, RoveCommAddress address, RoveCommNetworkProtocol protocol = RoveCommNetworkProtocol::UDP);
        static std::vector<RoveCommPacket> Read(RoveCommNetworkProtocol protocol = RoveCommNetworkProtocol::UDP);
        static std::future<RoveCommPacket> Fetch(RoveCommDataId unId = rovecomm::System::ANY, RoveCommAddress address = RoveCommAddress::ANY);
        // static void SetCallback(RoveCommDataId unId, RoveCommCallback fCallback);
        //  more here eventually

    private:
        // static std::map<RoveCommNetworkProtocol, std::vector<RoveCommServer>> s_Servers;
        // static std::map<RoveCommDataId, RoveCommCallback> s_fCallbacks;
};

// This is the main singleton that we call static functions on
// So we can get Java-style RoveComm.write(...);
extern RoveCommServerManager RoveComm;

#endif

// Old comments
// // FIXME: Change parameter variables names to meet our style guide.
// RoveComm(int udp_port, int tcp_addr);

// // FIXME: Change function names and paramerter variable names to meet our style guide.
// static void listen();
// static void set_callback(int data_id, std::string& func);    // Find way to pass function as input
// static void clear_callback(int data_id);
// static void set_default_callback(std::string& func);         // Find way to pass function as input
// static void clear_default_callback();
// static int write(RoveCommPacket& packet, bool reliable = false);
// static void close_thread();
