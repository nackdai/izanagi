var obj = require('./build/Release/Render2D');

var express = require('express');
var http = require('http');

var app = express();
app.use(express.static(__dirname + '/public'));

var server = http.createServer(app);

port = process.env.PORT || 5000;

server.listen(port, function() {
  console.log('server connected')
});

var socketIO = require('socket.io');

// クライアントの接続を待つ(IPアドレスとポート番号を結びつけます)
var io = socketIO.listen(server);

//obj.SetRootPath('D:\\work\\tmp\\izanagi\\github\\Samples\\Render2D\\public');
obj.SetRootPath('C:\\work\\izanagi\\github\\Samples\\NodeJsBinding\\public');
obj.Render2D();

var count = 0;

// クライアントが接続してきたときの処理
io.sockets.on('connection', function (socket) {
    console.log("connected");

    setInterval(function () {
        obj.Loop();
        socket.emit('updated', { cnt: count });
        count++;
    }, 100);
});
