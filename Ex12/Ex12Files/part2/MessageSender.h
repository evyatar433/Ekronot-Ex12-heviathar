#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <iostream>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

class MessageSender 
{
private:
    std::unordered_set<std::string> _connectedUsers;
    std::queue<std::string> _messagesQueue;
    std::mutex _usersMutex;
    std::mutex _queueMutex;
    std::condition_variable _queueCondVar;
    const std::string _adminFile = "C:/Users/Cyber_User/OneDrive/Magshimim/c++/Ekronot-Ex12-heviathar/Ex12/Ex12Files/part2/data.txt";
    const std::string _outputFile = "C:/Users/Cyber_User/OneDrive/Magshimim/c++/Ekronot-Ex12-heviathar/Ex12/Ex12Files/part2/output.txt";

public:
    //MessageSender();
    void showMenu();
    void signIn();
    void signOut();
    void showConnectedUsers();
    void readAdminFile();
    void writeMessagesToUsersFile();
    void run();
};

#endif // MESSAGESENDER_H
