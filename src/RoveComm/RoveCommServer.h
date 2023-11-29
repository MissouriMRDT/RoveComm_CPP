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

#include <map>
#include <thread>
#include <vector>

#include "RoveCommPacket.h"

/******************************************************************************
 * @brief Allow us to add more protocols. Currently only TCP/UDP.
 * Enum class prevents accidental casting to int
 *
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-11-14
 ******************************************************************************/
enum class RoveCommServerProtocol
{
    TCP,
    UDP
};

using RoveCommServerPort = unsigned int;
using RoveCommCallback   = void();

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
typedef struct
{
        char cOctants[4];
        unsigned int unPort;
} RoveCommAddress;

inline std::ostream& operator<<(std::ostream& out, const RoveCommAddress& address);

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
        virtual int WriteTo(RoveCommPacket& packet, RoveCommAddress address) const;
        /******************************************************************************
         * @brief Read incoming packets
         *
         * @return std::vector<RoveCommPacket>& - a list of RoveCommPackets. 0 if no packets available.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        virtual std::vector<RoveCommPacket>& Read() const;

        inline const RoveCommServerPort GetPort() const { return m_unPort; }

    protected:
        const RoveCommServerPort m_unPort;
        // std::map<RoveCommDataId, RoveCommCallback> m_fCallbacks;
        std::thread m_thNetworkThread;
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
        static void OpenServerOnPort(unsigned int port, RoveCommServerProtocol protocol = RoveCommServerProtocol::UDP);
        static void Shutdown();
        static void Write(RoveCommPacket& packet, RoveCommServerProtocol protocol);

        /******************************************************************************
         * @brief Sets the callback function for any incoming packets with the given data id.
         * This runs asynchronously on another thread!
         *
         * @param unId - data id to listen for
         * @param fCallback - function to call
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-11-29
         ******************************************************************************/
        static void SetCallback(RoveCommDataId unId, RoveCommCallback fCallback);
        // more here eventually

    private:
        static std::map<RoveCommServerProtocol, std::vector<RoveCommServer>> s_Servers;
        static std::map<RoveCommDataId, RoveCommCallback> s_fCallbacks;
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
