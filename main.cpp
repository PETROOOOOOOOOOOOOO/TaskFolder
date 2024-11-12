#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

class Task {
public:
    static int nextId;
    int id;
    std::string title;
    std::string description;
    std::string dueData;
    bool completed;

    Task() : id(nextId++), completed(false) {}

    void saveToFile(std::ofstream &out) const {
        out << id << '\n'
            << title << '\n'
            << description << '\n'
            << dueData << '\n'
            << completed << '\n';

    }

    bool loadFromFile(std::ifstream &in) {
        if (!(in >> id)) return false;
        in.ignore();
        std::getline(in, title);
        std::getline(in, description);
        std::getline(in, dueData);
        in >> completed;
        in.ignore();
        if (id >= nextId)nextId = id + 1;
        return true;
    }
};

int Task::nextId = 1;

class TaskManager {
public:
    void addTask() {
        Task newTask;
        std::cout << "Enter Task " << std::endl;
        std::cin.ignore();
        std::getline(std::cin, newTask.title);
        std::cout << "Enter description " << std::endl;
        std::getline(std::cin, newTask.description);
        std::cout << "Enter Date " << std::endl;
        std::getline(std::cin, newTask.dueData);
        tasks.push_back(newTask);
        std::cout << "Task added!" << std::endl;
        saveTaskToFile();
    }

    void displayTasks() const {
        for (const auto &task: tasks) {
            std::cout << "ID: " << task.id
                      << "| Task: " << task.title
                      << ", Description: " << task.description
                      << ", Data: " << task.dueData
                      << ", Status: " << (task.completed ? "Done" : "Not Done") << std::endl;
        }
    }

    void markTaskAsCompleted(int taskId) {
        for (auto &task: tasks) {
            if (task.id == taskId) {
                task.completed = true;
                std::cout << "Task marked as done." << std::endl;
                saveTaskToFile();
                return;
            }
        }
        std::cout << "Task not found" << std::endl;
    }

    void deleteTask(int taskId) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [taskId](const Task &task) {
            return task.id == taskId;
        });
        if (it != tasks.end()) {
            tasks.erase(it);

            for (int i = 0; i < tasks.size() ; ++i) {
                tasks[i].id = i + 1;
            }

            Task::nextId = tasks.size() + 1;

            std::cout << "Task deleted." << std::endl;
            saveTaskToFile();
        } else {
            std::cout << "Task not found." << std::endl;
        }
    }

    void loadTasksFromFile() {
        tasks.clear();

        std::ifstream infile(filename);
        if (!infile) {
            return;
        }
        Task task;
        while (task.loadFromFile(infile)) {
            tasks.push_back(task);
        }
    }

    void saveTaskToFile() const {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Error of opening file";
            return;
        }
        for (const auto &task: tasks) {
            task.saveToFile(outfile);
        }
        std::cout << "Tasks saved to file" << std::endl;
    }

private:
    std::vector<Task> tasks;
    const std::string filename = "Tasks.txt";
};

void showMenu() {
    std::cout << "Chose action" << std::endl;
    std::cout << "1.Add Task" << std::endl;
    std::cout << "2.Show all Tasks" << std::endl;
    std::cout << "3.Mark Task as done." << std::endl;
    std::cout << "4.Delete Task." << std::endl;
    std::cout << "5.Exit" << std::endl;
    std::cout << "Your choice: ";
}

int main() {
    TaskManager manager;
    manager.loadTasksFromFile();

    int choice;

    refresher:
    showMenu();
    std::cin >> choice;
    switch (choice) {
        case 1: {
            manager.addTask();
            goto refresher;
        }
        case 2: {
            manager.displayTasks();
            goto refresher;
        }
        case 3: {
            int id;
            std::cout << "Enter Task ID: ";
            std::cin >> id;
            manager.markTaskAsCompleted(id);
            goto refresher;
        }
        case 4: {
            int ID;
            std::cout << "Enter Task ID: ";
            std::cin >> ID;
            manager.deleteTask(ID);
            manager.saveTaskToFile();
            goto refresher;
        }
        case 5: {
            std::cout << "Program Locked." << std::endl;
            return 1;
        }
        default:
            std::cout << "Fail" << std::endl;
    }
    return 1;
}