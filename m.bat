@echo off
@if not exist build mkdir build
pushd build
cl -EHsc -Zi -I..\src ..\src\echoclient.cpp ..\src\SocketAddress.cpp ..\src\TCPSocket.cpp Kernel32.lib Ws2_32.lib
cl -EHsc -Zi -I..\src ..\src\echoserver.cpp ..\src\SocketAddress.cpp ..\src\TCPSocket.cpp Kernel32.lib Ws2_32.Lib
popd
