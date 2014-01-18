var echoserver = new EchoServer();
echoserver.handlerScript = "echo_server.js";
var port = 9090;
var addr = "127.0.0.1";
print("start echo server on " + addr + ":" + port);
var rt = echoserver.listen(port, addr);
print("listen return " + rt);
