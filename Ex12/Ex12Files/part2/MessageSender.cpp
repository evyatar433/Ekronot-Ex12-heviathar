#include "MessageSender.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>`1a
#include <string>
#include <queue>
#include <unordered_set>
#include <limits>
#include <mutex>
#include <condition_variable>

#define PATH "C:/Users/Cyber_User/OneDrive/Magshimim/c++/Ekronot-Ex12-heviathar/Ex12/Ex12Files/part2/data.txt"
#define OUTPUT_PATH "C:/Users/Cyber_User/OneDrive/Magshimim/c++/Ekronot-Ex12-heviathar/Ex12/Ex12Files/part2/output.txt"

bool running = true;

std::condition_variable cv;

std::mutex mtx1;
std::mutex mtx2;

// show the menu for the user
void MessageSender::showMenu()
{
	int choice;
	while (true)
	{
		std::unordered_set<std::string> users;
		std::cout << "\n1. Sign In" << std::endl;
		std::cout << "2. Sign Out" << std::endl;
		std::cout << "3. Connected Users" << std::endl;
		std::cout << "4. Exit" << std::endl;

		std::cout << "Enter your choice: ";

		// check if the input is an integer
		std::cin >> choice;

		if (std::cin.fail()) 
		{

			// ignore invalid input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw std::invalid_argument("invalid input please enter an integer");
			continue;
		}

		// options
		switch (choice)
		{
		case 1:
			signIn();
			break;

		case 2:
			signOut();
			break;

		case 3:
			showConnectedUsers();
			break;

		case 4:
			return;

		default:
			std::cout << "invalid choice try again\n";
		}
	}
}

// sign in user
void MessageSender::signIn()
{
	std::string username;
	std::cout << "enter your username: ";
	std::cin >> username;
	std::unique_lock<std::mutex> lock2(mtx2);

	auto result = _connectedUsers.insert(username);

	lock2.unlock();

	if (!result.second) 
	{  
		std::cout << "error: username already exists" << std::endl;
	}
	else 
	{
		std::cout << "welcome, " << username << " You have been added to the connected users\n";
	}
}


// sign out user
void MessageSender::signOut()
{
	std::string username;
	std::cout << "enter your username: ";
	std::cin >> username;

	std::unique_lock<std::mutex> lock2(mtx2);

	auto it =_connectedUsers.find(username);
	lock2.unlock();

	if (it != _connectedUsers.end()) 
	{ 
		_connectedUsers.erase(it); 
		std::cout << "goodbye " << username << "you have been signed out\n";
	}
	else 
	{
		std::cout << "error: username not found in the connected users\n";
	}
}

// show all connecteed users
void MessageSender::showConnectedUsers()
{
	std::cout << "connected users:\n";
	for (const auto& user : _connectedUsers) 
	{
		std::cout << user << std::endl;
	}
}

// read from file and put in the queue
void MessageSender::readAdminFile() 
{
	while (running) 
	{
		std::ifstream file(_adminFile);
		if (!file) 
		{
			std::cerr << "error opening data file" << std::endl;
			//std::this_thread::sleep_for(std::chrono::seconds(60));
			continue;
		}

		std::string line;
		std::queue<std::string> tempQueue;
		while (std::getline(file, line)) 
		{
			tempQueue.push(line);
		}
		file.close();

		{
			std::lock_guard<std::mutex> lock(_queueMutex);
			while (!tempQueue.empty()) 
			{
				_messagesQueue.push(tempQueue.front());
				tempQueue.pop();
			}
		}
		cv.notify_one();

		// dont uncomment
		//std::ofstream clearFile("data.txt", std::ofstream::trunc);
		//clearFile.close();

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}


// function write messages from queue to the output file
void MessageSender::writeMessagesToUsersFile() 
{
	while (running) 
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		cv.wait(lock, [this] { return !_messagesQueue.empty() || !running; });

		if (!running) break;

		std::ofstream outputFile(_outputFile, std::ios::app);
		if (!outputFile) 
		{
			std::cerr << "error opening output file" << std::endl;
			continue;
		}


		while (!_messagesQueue.empty()) 
		{
			std::string message = _messagesQueue.front();
			_messagesQueue.pop();
			lock.unlock();

			std::lock_guard<std::mutex> userLock(_usersMutex);
			for (const auto& user : _connectedUsers) 
			{
				outputFile << user << ": " << message << std::endl;
			}
			lock.lock();
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
}


void MessageSender::run() 
{
	std::thread readerThread(&MessageSender::readAdminFile, this);
	std::thread writerThread(&MessageSender::writeMessagesToUsersFile, this);
	showMenu();
	readerThread.join();
	writerThread.join();
}

int main()
{
	MessageSender ms;

	std::thread readerThread(&MessageSender::readAdminFile, &ms);
	std::thread writerThread(&MessageSender::writeMessagesToUsersFile, &ms);

	ms.showMenu();

	running = false;
	cv.notify_all();

	readerThread.detach();
	writerThread.detach();

	return 0;
}
