git clone https://github.com/warmcat/libwebsockets.git
git checkout  v1.7.1
cmake ../ -DOPENSSL_ROOT_DIR=/usr/local/Cellar/openssl/1.0.2h_1/ -DLWS_WITH_STATIC=OFF -DLWS_WITHOUT_TESTAPPS=ON
