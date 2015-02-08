var obj = require('./build/Release/Render2D');

var express = require('express');
var http = require('http');

var app = express();
app.use(express.static(__dirname + '/public'));

console.log("1");

var server = http.createServer(app);

port = process.env.PORT || 5000;

console.log("2");

server.listen(port, function() {
  console.log('server connected')
});

console.log("3");

var socketIO = require('socket.io');

// �N���C�A���g�̐ڑ���҂�(IP�A�h���X�ƃ|�[�g�ԍ������т��܂�)
var io = socketIO.listen(server);

//obj.SetRootPath('D:\\work\\tmp\\izanagi\\github\\Samples\\Render2D\\public');
obj.SetRootPath('C:\\work\\izanagi\\github\\Samples\\NodeJsBinding\\public');
obj.Render2D();

console.log("4");

var count = 0;

// �N���C�A���g���ڑ����Ă����Ƃ��̏���
io.sockets.on('connection', function (socket) {
    console.log("connected");

    setInterval(function () {
        obj.Loop();
        socket.emit('updated', { cnt: count });
        count++;
    }, 100);
});
