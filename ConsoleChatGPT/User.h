#pragma once
#include <iostream>
#include <list>

class User
{
	int id;
	std::string name;
	std::string password;
	std::list<std::string> responses;

public:
	User() : name("No name"), password("no password"), id(-1) { }
	User(std::string name, std::string password, int id) : name(name), password(password), id(id) { }

	std::string GetName() { return name; }
	std::string GetPassword() { return password; }
	int GetId() { return id; }

	void SetName(std::string name) { this->name = name; }
	void SetPassword(std::string password) { this->password = password; }

	void ChangePassword(std::list<User>& otherUsers);
	void ChangeName(std::list<User>& otherUsers);

	void ShowHistory();
	void MakeRequest();

	void ShowInfo();
};

