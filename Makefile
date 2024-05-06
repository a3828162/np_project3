part1:
	g++ -w http_server.cpp -o http_server -std=c++14 -Wall -pedantic -pthread -lboost_system
	g++ -w console.cpp -o console.cgi -std=c++14 -Wall -pedantic -pthread -lboost_system
part2:
	g++ -w cgi_server.cpp -o cgi_server -lws2_32 -lwsock32 -std=c++14 -pthread
clean:
	rm -f http_server console.cgi