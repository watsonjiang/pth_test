var handler = function(conn) {
   //var data = conn.read();
   //conn.send(data);
   //conn.close(); 
   print(conn);
};

var echoserver = new EchoServer();
echoserver.requestHandler = handler;
echoserver.requestHandler("hello world");
//var rt = echoserver.listen(8080, 127.0.0.1);

print("listen return " + rt);
