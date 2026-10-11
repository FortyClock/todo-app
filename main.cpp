
#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <limits>
#include <cstdlib>



int main()
{
    try
    {
        pqxx::connection db{
            "host=" + std::string(
                std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "localhost"
            )
            + " port=5432 "
            "dbname=todo_db "
            "user=todo_user "
            "password=0000"
        };

        pqxx::work transaction{db};

        transaction.exec(R"(
            CREATE TABLE IF NOT EXISTS tasks (
                id SERIAL PRIMARY KEY,
                title TEXT NOT NULL,
                done BOOLEAN NOT NULL DEFAULT FALSE
            )
        )");

        transaction.commit();

        while (true)
        {
            std::cout << "\n1. Add task\n";
            std::cout << "2. List tasks\n";
            std::cout << "3. Exit\n";
            std::cout << "> ";

            int command{};
            if (!(std::cin >> command))
            {
                break;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');

            if (command == 1)
            {
                std::string title;

                std::cout << "Task title: ";
                std::getline(std::cin, title);

                if (title.empty())
                {
                    std::cout << "Title cannot be empty!\n";
                    continue;
                }

                pqxx::work tx{db};

                tx.exec_params(
                    "INSERT INTO tasks (title) VALUES ($1)",
                    title
                );

                tx.commit();

                std::cout << "Task added!\n";
            }
            else if (command == 2)
            {
                pqxx::read_transaction tx{db};

                const auto result = tx.exec(
                    "SELECT id, title, done FROM tasks ORDER BY id"
                );

                for (const auto& row : result)
                {
                    std::cout << row["id"].as<int>() << ". "
                              << row["title"].as<std::string>()
                              << " ["
                              << (row["done"].as<bool>()
                                      ? "done" : "not done")
                              << "]\n";
                }

                if (result.empty())
                {
                    std::cout << "No tasks yet.\n";
                }
            }
            else if (command == 3)
            {
                break;
            }
            else
            {
                std::cout << "Unknown command.\n";
            }
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
