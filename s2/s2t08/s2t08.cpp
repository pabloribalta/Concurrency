#include <mutex>


struct Packet{};
struct ConnectionInfo{};

struct ConnectionHandle
{
  void sendData(Packet p)
  {
  }

  Packet receiveData()
  {
    return{};
  }
};

struct ConnectionManager
{
  ConnectionHandle open(ConnectionInfo &info)
  {
    return{};
  }
};

class SocketWrapper
{
public:
  explicit SocketWrapper(ConnectionInfo const& connectionDetails) :
    m_connectionDetails(connectionDetails)
  {
  }

  void sendData(Packet const& data) /* (!) This call will try to initialize m_connection */
  {
    std::call_once(connection_init_flag, &SocketWrapper::open_connection, this); /* (!) Similar syntax like std::bind and std::thread */
    m_connection.sendData(data);
  }

  Packet receiveData() /* (!) This call will try to initialize m_connection */
  {
    std::call_once(connection_init_flag, &SocketWrapper::open_connection, this); /* (!) Similar syntax like std::bind and std::thread */
    return m_connection.receiveData();
  }

private:
  ConnectionInfo m_connectionDetails;
  ConnectionHandle m_connection;
  ConnectionManager m_connectionManager;

  std::once_flag connection_init_flag;

  void open_connection()
  {
    m_connection = m_connectionManager.open(m_connectionDetails);
  }
};

int main()
{
  return 0;
}