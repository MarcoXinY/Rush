#include <iostream>
#include "Chat.hpp"

namespace Service
{

Chat::Chat()
{
    std::cout << "creat a new chat Client" << std::endl;
}

bool Chat::getConnectedStatus() const
{
    return IsConnected;
}


void Chat::parseHandshake(unsigned char* input_frame, int input_len)
{
    webSocket.parseHandshake(input_frame, input_len);
    localName = webSocket.params["localName"];
    peerName = webSocket.params["peerName"];
}

std::string Chat::answerHandshake()
{
    std::string reponse = webSocket.answerHandshake();
    IsConnected = true;
    return reponse;
}

void Chat::processRequest(unsigned char* input_frame, int input_len)
{
    if(IsConnected == false)
    {
        //没有握手 执行握手
        parseHandshake(input_frame, input_len);
    }
    else
    {
        //握手完成 解析requestMessage
        int PayloadDataLenth = 0;
        unsigned char outBuffer[1024] = {0};
        webSocketLib::WebSocketFrameType frameType = webSocket.getFrame(input_frame, input_len, outBuffer, 1024, &PayloadDataLenth);
        std::string resqustMessage((char*)outBuffer, PayloadDataLenth);//PayloadDataLenth - 1 ?
        std::cout << resqustMessage <<std::endl;
    }
}

std::string Chat::responseWithProcessResult()
{
    if(IsConnected == false)
    {
        //没有握手 握手的response
        return answerHandshake();
    }
    else
    {
        //握手完成 回Response
        std::string sendmsg = "abc";
        unsigned char sendbuffer[1024] ={0};

        int result = webSocket.makeFrame(webSocketLib::WebSocketFrameType{webSocketLib::WebSocketFrameType::TEXT_FRAME},
                                        (unsigned char*)sendmsg.c_str(),
                                        sendmsg.size(),
                                        sendbuffer,
                                        sizeof(sendbuffer));

        std::string responseFrame((char *)sendbuffer, result);

        std::cout << "result :" << result << std::endl;
        return responseFrame;
        // std::cout << "content :" << sendmsg << std::endl;
        //printf("Recv: %s, %d Bytes\n", sendbuffer, strlen((const char *)sendbuffer));

    }
}

Chat::~Chat()
{
    std::cout << "release a chat client " << std::endl;
}

}

