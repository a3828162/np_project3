#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;
using namespace std;

struct clientInfo {
    string hostName;
    string port;
    string testFile;
};

string header = "Content-type: text/html\r\n\r\n";
string get_panel_page() {
    string panel_part1 = R"(
	<!DOCTYPE html>
	<html lang="en">
	  <head>
	    <title>NP Project 3 Panel</title>
	    <link
	      rel="stylesheet"
	      href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css"
	      integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2"
	      crossorigin="anonymous"
	    />
	    <link
	      href="https://fonts.googleapis.com/css?family=Source+Code+Pro"
	      rel="stylesheet"
	    />
	    <link
	      rel="icon"
	      type="image/png"
	      href="https://cdn4.iconfinder.com/data/icons/iconsimple-setting-time/512/dashboard-512.png"
	    />
	    <style>
	      * {
	        font-family: 'Source Code Pro', monospace;
	      }
	    </style>
	  </head>
	  <body class="bg-secondary pt-5">
	  	<form action="console.cgi" method="GET">
	      <table class="table mx-auto bg-light" style="width: inherit">
	        <thead class="thead-dark">
	          <tr>
	            <th scope="col">#</th>
	            <th scope="col">Host</th>
	            <th scope="col">Port</th>
	            <th scope="col">Input File</th>
	          </tr>
	        </thead>
	        <tbody>
	)";

    string host_menu;
    for (int i = 0; i < 12; i++) {
        host_menu += "<option value=\"nplinux" + to_string(i + 1) +
                     ".cs.nctu.edu.tw\">nplinux" + to_string(i + 1) +
                     "</option>";
    }

    string panel_part2;
    int N_SERVERS = 5;
    for (int i = 0; i < N_SERVERS; i++) {
        panel_part2 += (boost::format(R"(
			<tr>
	          <th scope="row" class="align-middle">Session %1%</th>
	          <td>
	            <div class="input-group">
	              <select name="h%2%" class="custom-select">
	                <option></option>"%3%"
	              </select>
	              <div class="input-group-append">
	                <span class="input-group-text">.cs.nctu.edu.tw</span>
	              </div>
	            </div>
	          </td>
	          <td>
	            <input name="p%2%" type="text" class="form-control" size="5" />
	          </td>
	          <td>
	            <select name="f%2%" class="custom-select">
	              <option></option>
	              <option value="t1.txt">t1.txt</option>
	              <option value="t2.txt">t2.txt</option>
	              <option value="t3.txt">t3.txt</option>
	              <option value="t4.txt">t4.txt</option>
	              <option value="t5.txt">t5.txt</option>
	            </select>
	          </td>
	        </tr>
			)") % to_string(i + 1) %
                        to_string(i) % host_menu)
                           .str();
    }
    string panel_part3 = R"(
			<tr>
	          <td colspan="3"></td>
	          <td>
	            <button type="submit" class="btn btn-info btn-block">Run</button>
	          </td>
	        </tr>
	      </tbody>
	    </table>
	  </form>
	  </body>
	  </html>
	)";

    return header + panel_part1 + panel_part2 + panel_part3;
}

string get_console_page(vector<clientInfo> cnt) {
    string console_head = R"(
		<!DOCTYPE html>
		<html lang="en">
		  <head>
		    <meta charset="UTF-8" />
		    <title>NP Project 3 Sample Console</title>
		    <link
		      rel="stylesheet"
		      href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css"
		      integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2"
		      crossorigin="anonymous"
		    />
		    <link
		      href="https://fonts.googleapis.com/css?family=Source+Code+Pro"
		      rel="stylesheet"
		    />
		    <link
		      rel="icon"
		      type="image/png"
		      href="https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png"
		    />
		    <style>
		      * {
		        font-family: 'Source Code Pro', monospace;
		        font-size: 1rem !important;
		      }
		      body {
		        background-color: #212529;
		      }
		      pre {
		        color: #cccccc;
		      }
		      b {
		        color: #01b468;
		      }
		    </style>
		  </head>
		  <body>
		    <table class="table table-dark table-bordered">
		      <thead>
		        <tr>
	)";
    string console_part1;
    for (int i = 0; i < cnt.size(); i++) {
        if (cnt[i].hostName != "" && cnt[i].port != "" &&
            cnt[i].testFile != "") {
            console_part1 += "<th scope=\"col\">" + cnt[i].testFile + ":" +
                             cnt[i].port + "</th>\r\n";
        }
    }

    string console_part2 = R"(
				</tr>
		      </thead>
		      <tbody>
		        <tr>
	)";

    string console_part3;
    for (int i = 0; i < cnt.size(); i++) {
        if (cnt[i].hostName != "" && cnt[i].port != "" &&
            cnt[i].testFile != "") {
            console_part3 += "<td><pre id=\"s" + to_string(i) +
                             "\" class=\"mb-0\"></pre></td>\r\n";
        }
    }
    string console_part4 = R"(
		        </tr>
		      </tbody>
		    </table>
		  </body>
		</html>
	)";

    return header + console_head + console_part1 + console_part2 +
           console_part3 + console_part4;
}

const string env_Variables[9] = {
    "REQUEST_METHOD",  "REQUEST_URI", "QUERY_STRING",
    "SERVER_PROTOCOL", "HTTP_HOST",   "SERVER_ADDR",
    "SERVER_PORT",     "REMOTE_ADDR", "REMOTE_PORT"};

vector<clientInfo> clients(5);

class shellClient : public std::enable_shared_from_this<shellClient> {
  public:
    shellClient(boost::asio::io_context &io_context, tcp::socket remoteServerSocket ,shared_ptr<tcp::socket> shared_client, int index)
        : resolver(io_context), socket_(move(remoteServerSocket)), shared_client_(shared_client) ,index(index){
          clientPtr = &clients[index];
        }

  void start() { do_resolve(); }

  private:

  void do_resolve() {
    auto self(shared_from_this());
    tcp::resolver::query query(clientPtr->hostName, clientPtr->port);
    resolver.async_resolve(query, [this, self](boost::system::error_code ec, tcp::resolver::results_type results) {
      if (!ec) {
        do_connect(results);
      }
    });
  }

  void do_connect(tcp::resolver::results_type results) {
    auto self(shared_from_this());
    boost::asio::async_connect(socket_, results, [this, self](boost::system::error_code ec, tcp::endpoint) {
      if (!ec) {
        do_read();
      }
    });
  }

  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length), [this, self](boost::system::error_code ec, size_t length) {
      if (!ec) {
        data_[length] = '\0';
        cout << "Read from " << clientPtr->hostName << ":" << clientPtr->port << " : " << data_ << endl;
        do_write(length);
      }
    });
  }

  void do_write(size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length), [this, self](boost::system::error_code ec, size_t) {
      if (!ec) {
        cout << "Write to " << clientPtr->hostName << ":" << clientPtr->port << " : " << data_ << endl;
        do_read();
      }
    });
  }

  tcp::resolver resolver; // resolve host name to ip address
  tcp::socket socket_; // socket to remote np server
  shared_ptr<tcp::socket> shared_client_; // share ptr to browser socket
  clientInfo* clientPtr; // pointer to clientInfo
  int index; // index of client in clients vector
  enum { max_length = 4096 }; // max length of data
  char data_[max_length]; // data buffer
  //boost::asio::io_context &io_context_; 
};

class session : public std::enable_shared_from_this<session> {
  public:
    session(tcp::socket socket, boost::asio::io_context &io_context) : socket_(std::move(socket)), io_context_(io_context) {}

    void start() { do_read(); }

  private:
    void do_read() {
        auto self(shared_from_this());
        
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {

                    data_[length] = '\0';
                    http_request_parser();
                    memset(data_, '\0', length);
                    cgi_handler();
                }
            });
    }

    void cgi_handler() {

        auto self(shared_from_this());
        strcpy(data_, "HTTP/1.1 200 OK\r\n");
        boost::asio::async_write(
            socket_, boost::asio::buffer(data_, strlen(data_)),
            [this, self](boost::system::error_code ec, size_t s) {
                string cgiFileName = env[env_Variables[1]].substr(
                    0, env[env_Variables[1]].find("?"));
                cout << cgiFileName << endl;
                if (cgiFileName == "/panel.cgi") {
                    panel_handler();
                } else if (cgiFileName == "/console.cgi") {
                    clients.clear();
                    clients.assign(5, clientInfo{});
                    setClientInfo();
                    console_handler();
                    shared_ptr<tcp::socket> shared_client_ =
                        make_shared<tcp::socket>(move(socket_));
                    for (int i = 0; i < clients.size(); i++) {
                        if (clients[i].hostName != "" && clients[i].port != "" &&
                            clients[i].testFile != "") {
                            tcp::socket remoteClient_(io_context_);
                            make_shared<shellClient>(io_context_, remoteClient_, shared_client_ ,i)->start();
                        }
                    }
                } else {
                    socket_.close();
                }
            });
    }

    void panel_handler() {
        auto self(shared_from_this());
        string panel_page = get_panel_page();

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(panel_page.c_str(), panel_page.length()),
            [this, self](boost::system::error_code ec, size_t) {
                if (!ec) {
                    cout << "Write Panel.cgi OK" << endl;
                }
            });
    }

    void console_handler() {
        auto self(shared_from_this());
        string console_page = get_console_page(clients);

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(console_page.c_str(), console_page.length()),
            [this, self](boost::system::error_code ec, size_t) {
                if (!ec) {
                    cout << "Write Console.cgi OK" << endl;
                }
            });
    }

    void setClientInfo() {
        string query = env["QUERY_STRING"];
        stringstream ss(query);
        string token;
        int i = 0;
        while (getline(ss, token, '&')) {
            clients[i].hostName =
                token.substr(token.find("=") + 1, token.size() - 1);
            getline(ss, token, '&');
            clients[i].port =
                token.substr(token.find("=") + 1, token.size() - 1);
            getline(ss, token, '&');
            clients[i].testFile =
                token.substr(token.find("=") + 1, token.size() - 1);
            ++i;
        }
    }

    void http_request_parser() {
        stringstream ss;
        ss << string(data_);
        string token;
        vector<string> tmp;
        while (ss >> token)
            tmp.push_back(token);

        env[env_Variables[0]] = tmp[0];
        env[env_Variables[1]] = tmp[1];
        env[env_Variables[3]] = tmp[2];
        env[env_Variables[4]] = tmp[4];
        env[env_Variables[5]] = socket_.local_endpoint().address().to_string();
        env[env_Variables[6]] = to_string(socket_.local_endpoint().port());
        env[env_Variables[7]] = socket_.remote_endpoint().address().to_string();
        env[env_Variables[8]] = to_string(socket_.remote_endpoint().port());

        if (env[env_Variables[1]].find("?") != string::npos) {
            env[env_Variables[2]] = env[env_Variables[1]].substr(
                env[env_Variables[1]].find("?") + 1);
        } else {
            env[env_Variables[2]] = "";
        }

        for (auto &[key, value] : env) {
            // cout << key << " : " << value << endl;
        }
    }

    boost::asio::io_context &io_context_;
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    map<string, string> env;
};

class server {
  public:
    server(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), io_context_(io_context){
        do_accept();
    }

  private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "Server: Accept...\n";
                    std::make_shared<session>(std::move(socket), io_context_)->start();
                }

                do_accept();
            });
    }
    
    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }
        boost::asio::io_context io_context;
        server s(io_context, std::atoi(argv[1]));

        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}