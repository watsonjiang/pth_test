var handler = function(conn) {
   var data = conn.read();
   conn.send(data);
   conn.close(); 
};

var echoserver = new EchoServer();
echoserver.setRequestHandler(handler);
var rt = echoserver.listen(8080, 127.0.0.1);

print("listen return " + rt);
