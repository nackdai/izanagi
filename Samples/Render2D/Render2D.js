var obj = require('./build/Release/Render2D');

obj.Render2D();

setInterval(function () {
    obj.Loop();
}, 100);
