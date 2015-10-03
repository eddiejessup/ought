import std.string;
import std.stdio;
import ought;

string getHTTPResponse(string data) {
    import std.datetime;

    auto nowTime = Clock.currTime(UTC());
    return format("
HTTP/1.1 200 OK
Date: %s
Content-Type: application/json
Content-Length: %s
Access-Control-Allow-Origin: *

%s", nowTime, data.length, data);
}

void serveData(Network network, string domain, ushort port) {
    import std.socket, std.stream, std.socketstream;
    import std.conv : to;

    Socket srv = new TcpSocket;
    srv.setOption(SocketOptionLevel.SOCKET, SocketOption.REUSEADDR, 1);
    srv.bind(new InternetAddress(domain, port));
    srv.listen(5);

    writefln("Listening for connections");
    int i = 0;
    for (Socket cli=srv.accept(); srv.isAlive(); cli=srv.accept()) {
        writefln("Connected by %s", cli);
        Stream ss = new SocketStream(cli);
        while (true) {
            writefln("waiting for data");
            string data = to!string(ss.readLine());
            if (data.length == 0) {
                break;
            }
            writefln("got data: %s", data);
            if (data.indexOf("GET") != -1) {
                writefln("got request for data");
                writefln("sending network data:");
                ss.writeString(getHTTPResponse(network.toJSON()));
                break;
            }
        }
        writefln("iterating network, %d", i);
        ++i;
        network.iterate();
        cli.close();
    }
}

void main() {
    int nX = 50;
    int nY = 10;
    Node[] nodes = getPeriodicTwoDimLattice(getRandomBitArray(nX, nY));
    Network network = Network(nodes);

    ushort port = 4444;
    string domain = "localhost";
    serveData(network, domain, port);

    int i = 0;
    while (true) {
        writeln("iter ", i);
        network.iterate();
        ++i;
    }
}
