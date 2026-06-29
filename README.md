# encrypty-add-multithreading

A C++ file encryption and decryption tool with **multiprocessing** and **multithreading** support for high-performance parallel file processing.

## Features

- AES-based file encryption and decryption
- Multiprocessing architecture for parallel workloads
- Multithreading support for concurrent I/O operations
- Environment-based configuration via `.env`
- Batch processing of multiple files

## Project Structure

```
encrypty_multiprocessing/
├── main.cpp                          # Entry point
├── Makefile                          # Build configuration
├── makeDirs.py                       # Directory setup script
├── src/
│   └── app/
│       ├── encryptDecrypt/
│       │   ├── Cryption.hpp          # Encryption class header
│       │   ├── Cryption.cpp          # Encryption implementation
│       │   └── CryptionMain.cpp      # Encryption entry logic
│       ├── fileHandling/
│       │   ├── IO.hpp                # File I/O header
│       │   ├── IO.cpp                # File I/O implementation
│       │   └── ReadEnv.cpp           # .env reader
│       └── processes/
│           ├── ProcessManagement.hpp # Process manager header
│           ├── ProcessManagement.cpp # Process manager implementation
│           └── Task.hpp              # Task abstraction
└── test/                             # Test files
```

## Requirements

- g++ (C++17 or later)
- Make
- Python 3 (for `makeDirs.py`)
- Windows OS

## Build

```bash
python makeDirs.py
make
```

## Usage

```bash
./cryption.exe encrypt <input_file> <output_file>
./encrypt_decrypt.exe decrypt <input_file> <output_file>
```

## Configuration

Create a `.env` file in the project directory:

```
KEY=your_encryption_key_here
THREADS=4
```

## License

MIT License
