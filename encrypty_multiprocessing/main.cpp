#include <iostream>
#include <filesystem>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"
#include <ctime>
#include <iomanip>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::string directory;
    std::string action;

    std::cout << "Enter the directory path: ";
    std::getline(std::cin, directory);

    std::cout << "Enter the action (encrypt/decrypt): ";
    std::getline(std::cin, action);

    try {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            ProcessManagement processManagement;

            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
                    auto task = std::make_unique<Task>(taskAction, filePath);

                    std::time_t t = std::time(nullptr);
                    std::tm* now = std::localtime(&t);
                    std::cout << "Starting the encryption/decryption at: " << std::put_time(now, "%Y-%m-%d %H:%M:%S") << std::endl;
                    processManagement.submitToQueue(std::move(task));
                }
            }
        } else {
            std::cout << "Invalid directory path!" << std::endl;
        }
    } catch (const fs::filesystem_error& ex) {
        std::cout << "Filesystem error: " << ex.what() << std::endl;
    }

    return 0;
}