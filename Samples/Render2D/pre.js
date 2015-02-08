Module['preRun'] = function () {
    FS.createFolder(
        '/', // 親フォルダの指定
        'data', // フォルダ名
        true, // 読み込み許可
        true // 書き込み許可(今回の例はfalseでもよさげ)
    );
    FS.createPreloadedFile(
        '/data',
        'earth.img',
        '/data/earth.img',  // httpでアクセスする際のURLを指定
        true,
        false
    );
};