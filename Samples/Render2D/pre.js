Module['preRun'] = function () {
    FS.createFolder(
        '/', // �e�t�H���_�̎w��
        'data', // �t�H���_��
        true, // �ǂݍ��݋���
        true // �������݋���(����̗��false�ł��悳��)
    );
    FS.createPreloadedFile(
        '/data',
        'earth.img',
        '/data/earth.img',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );
};