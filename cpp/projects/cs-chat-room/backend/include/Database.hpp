#pragma once

#include <sqlite3.h>

#include <string>
#include <string_view>

class Database
{
  public:
    explicit Database(std::string databasePath);
    ~Database();

    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    sqlite3 *raw();

    void execute(std::string_view sql);
    void initializeSchema();

  private:
    sqlite3 *database_{nullptr};
};
