#include "socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>

using namespace std;

void get_URL( const string& host, const string& path )
{
  try {
      // 1. 创建TCP套接字
      TCPSocket socket;

      // 2. 解析服务器地址（HTTP默认端口80）
      Address server_address(host, "http");

      // 3. 建立连接
      socket.connect(server_address);

      // 4. 构造HTTP请求
      const string request = 
          "GET " + path + " HTTP/1.1\r\n" +
          "Host: " + host + "\r\n" +
          "Connection: close\r\n\r\n"; // 注意结尾有两个\r\n

      // 5. 发送请求
      socket.write(request);

      // 6. 读取响应
      string response;
      string buffer;

      do {
          buffer.clear();
          socket.read(buffer); // 正确调用read方法
          response += buffer;
      } while (!buffer.empty() && !socket.eof());

      // 7. 输出响应内容
      cout << response;

  } catch (const exception& e) {
      cerr << "Error: " << e.what() << endl;
  }
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
