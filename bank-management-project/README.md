# Bank Management System

This project is a simple Bank Management System implemented in C++. It allows users to manage bank accounts, perform transactions, and view account details. The system supports two types of accounts: Savings and Current accounts.

## Project Structure

```
bank-management-project
├── src
│   └── project2.cpp          # Main implementation of the bank management system
├── include                    # Directory for header files (if any)
├── tests                      # Directory for unit test files
├── .gitignore                 # Specifies files and directories to be ignored by Git
├── CMakeLists.txt            # Configuration file for CMake
└── README.md                  # Documentation for the project
```

## Features

- Create and manage Savings and Current accounts
- Deposit and withdraw funds
- View account details
- Record transactions in a text file

## Requirements

- C++11 or later
- CMake (for building the project)

## Building the Project

1. Clone the repository:
   ```
   git clone <repository-url>
   cd bank-management-project
   ```

2. Create a build directory and navigate into it:
   ```
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```
   cmake ..
   ```

4. Build the project:
   ```
   make
   ```

## Running the Application

After building the project, you can run the application using the following command:
```
./bank-management-project
```

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any suggestions or improvements.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.