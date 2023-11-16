#include "User.h"
#include "UserChoice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlohmann/json.hpp"
#include "curl/curl.h"



size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
	size_t totalSize = size * nmemb;
	output->append((char*)contents, totalSize);
	return totalSize;
}

char* insertStringAtPosition(char* original, const char* toInsert, size_t position) {

	size_t originalLength = strlen(original);
	size_t toInsertLength = strlen(toInsert) - 1;

	char* newStr = new char[originalLength + toInsertLength] {};

	size_t indexOfNewStr = 0;
	size_t indexOfOriginalStr = 0;
	size_t indexOfToInsertStr = 0;

	for (; indexOfOriginalStr < position; indexOfOriginalStr++)
	{
		newStr[indexOfNewStr++] = original[indexOfOriginalStr];
	}

	for (size_t i = 0; i < toInsertLength; i++)
	{
		newStr[indexOfNewStr++] = toInsert[i];
	}

	for (size_t i = indexOfOriginalStr; i < originalLength - 1; i++)
	{
		newStr[indexOfNewStr++] = original[i];
	}

	return newStr;
}



void User::ChangePassword(std::list<User>& otherUsers)
{
	while (true)
	{
		std::cout << "Enter old password: ";
		std::string tmpPassword;
		std::cin >> tmpPassword;

		if (tmpPassword == password)
		{
			while (true)
			{
				std::cout << "Enter new password: ";
				std::cin >> tmpPassword;

				if (otherUsers.empty())
				{
					password = tmpPassword;
					return;
				}
				else
				{
					bool ifPasswordIsReserved = false;

					for (auto& other : otherUsers)
					{
						if (other.GetPassword() == tmpPassword)
						{
							std::cout << "Password is reserved, enter again\n";
							ifPasswordIsReserved = true;
							break;
						}
					}
					if (!ifPasswordIsReserved)
					{
						password = tmpPassword;
						return;
					}

				}
			}
		}
		else
		{
			std::cout << "Wrong password, enter again\n";
		}
	}
}

void User::ChangeName(std::list<User>& otherUsers)
{
	while (true)
	{
		std::cout << "Enter password: ";
		std::string tmpPassword;
		std::cin >> tmpPassword;

		if (tmpPassword == password)
		{
			std::cout << "New name: ";
			name.clear();
			std::cin >> name;
			return;
		}
		else
		{
			std::cout << "Wrong password, enter again\n";
		}
	}
}

void User::ShowHistory()
{
	if (responses.empty())
	{
		std::cout << "You habe not responses.\n";
	}
	else
	{
		int numberOfresponse = 1;
		for (auto& response : responses)
		{
			std::cout << numberOfresponse++ << ") ";
			std::cout << response << std::endl;
		}
	}
}

void User::MakeRequest()
{
	CURL* curl;
	CURLcode res;

	// Initialize libcurl
	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Create a curl handle
	curl = curl_easy_init();
	if (curl) {
		// Set the API endpoint URL
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");

		// Set the HTTP headers
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Authorization: Bearer api key"); //// remember about api key
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Set the request payload


		char payload[] = R"(
        {
            "model": "gpt-3.5-turbo",
            "messages": [
                {
                    "role": "user",
                    "content": " "
                }
            ]
        }
    )";

		char str[100];

		printf("Enter : ");
		fgets(str, sizeof(str), stdin);

		char* tmpStr = insertStringAtPosition(payload, str, 162);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, tmpStr);



		// Set callback for response handling
		std::string response;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// Perform the HTTP POST request
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			// Parse the response into JSON using nlohmann/json
			nlohmann::json jsonResponse = nlohmann::json::parse(response);

			// Access the parsed JSON data as needed
			//std::cout << "Response:\n" << jsonResponse.dump(4) << std::endl; // dump with indentation

			// Example: Accessing a specific field in the JSON
			std::string exampleField = jsonResponse["choices"][0]["message"]["content"];

			std::cout << exampleField << std::endl;
			responses.push_back(exampleField);

			// ... (perform additional processing as needed)
		}

		// Check for errors
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else
			//std::cout << "Response:\n" << response << std::endl;

		// Clean up
			curl_easy_cleanup(curl);
		curl_slist_free_all(headers);

		// Cleanup libcurl
		curl_global_cleanup();

		delete[] tmpStr;
	}
}
