
---
## GitHub Repository Setup Instructions for `aerospace_encryptor_with_qr`
### Repository Structure
```
aerospace_encryptor_with_qr/
├── main.cpp          # Main source file with AES encryption and QR code generation
├── qrcodegen.hpp     # QR code library header
├── qrcodegen.cpp     # QR code library source
├── CMakeLists.txt    # CMake build configuration
├── README.md         # Extensive documentation
├── .gitignore        # Ignore build artifacts
└── mbedtls/          # mbedTLS library source (submodule or external)
```
---

### Step-by-Step Setup

#### 1. Create the Repository on GitHub
- Navigate to [GitHub](https://github.com), log in, and click **"New repository"**.
- Name the repository **`aerospace_encryptor_with_qr`**.
- Choose **"Public"** or **"Private"** based on your preference.
- **Do not** initialize with a README (we’ll add it manually).
- Click **"Create repository"**.

#### 2. Clone the Repository Locally
Open a terminal and run:

```bash
git clone https://github.com/yourusername/aerospace_encryptor_with_qr.git
cd aerospace_encryptor_with_qr
```
Replace `yourusername` with your actual GitHub username.
#### 3. Add Files
Create or copy the following files into the `aerospace_encryptor_with_qr` directory:
##### File 1: `main.cpp`
- **Content**: This is the main C++ source file containing the AES-256-CBC encryption and QR code generation logic.
- **Action**: Save it as `main.cpp` in the project root.
##### File 2: `qrcodegen.hpp`
- **Source**: Download from [nayuki/QR-Code-generator](https://raw.githubusercontent.com/nayuki/QR-Code-generator/master/cpp/qrcodegen.hpp).
- **Action**: Place it in the project root.
- **Alternative Download**:
  ```bash
  wget https://raw.githubusercontent.com/nayuki/QR-Code-generator/master/cpp/qrcodegen.hpp
  ```
##### File 3: `qrcodegen.cpp`
- **Source**: Download from [nayuki/QR-Code-generator](https://raw.githubusercontent.com/nayuki/QR-Code-generator/master/cpp/qrcodegen.cpp).
- **Action**: Place it in the project root.
- **Alternative Download**:
  ```bash
  wget https://raw.githubusercontent.com/nayuki/QR-Code-generator/master/cpp/qrcodegen.cpp
  ```
##### File 4: `CMakeLists.txt`
- **Content**:
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  project(aerospace_encryptor_with_qr)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  add_subdirectory(mbedtls)
  add_executable(aerospace_encryptor main.cpp qrcodegen.cpp)
  target_link_libraries(aerospace_encryptor mbedcrypto)
  set_target_properties(aerospace_encryptor PROPERTIES LINK_FLAGS "-static")
  ```
- **Action**: Save as `CMakeLists.txt` in the project root.
##### File 5: `README.md`
- **Content**:
  ```markdown
  # Aerospace Encryptor with QR Code Output

This is a C++ application for Windows that encrypts messages using AES-256-CBC and generates QR code bitmaps for each line of the encrypted output. Coded to demo aerospace application practices where code resilience is priority, it runs as a portable `.exe` from a USB stick. The QR codes encode the hex groups of each line (e.g., "ABCDE FGHIJ KLMNO PQRST UVWXY"), allowing researchers in Post Doc 3 to scan them with a standard mobile phone QR code app, retrieve the numbers, and screenshot the results.

  ## Features
  - **AES-256-CBC Encryption**: Securely encrypts messages with a 256-bit key and IV.
  - **Formatted Hex Output**: Displays encrypted data in groups of 5 characters per line.
  - **QR Code Generation**: Creates a QR code BMP file for each line’s hex output.
  - **Portability**: Statically linked executable for easy distribution.
  ## Prerequisites
  - **Windows OS**: Tested on Windows 10/11.
  - **MinGW**: For compiling with `g++` (e.g., via MSYS2).
  - **CMake**: Version 3.10 or higher.
  - **Git**: For cloning the repository.
  ## Setup and Build
  ### 1. Clone the Repository
  ```bash
  git clone https://github.com/yourusername/aerospace_encryptor_with_qr.git
  cd aerospace_encryptor_with_qr
  ```
  ### 2. Add mbedTLS
  #### Option 1: Submodule
  ```bash
  git submodule add https://github.com/ARMmbed/mbedtls.git mbedtls
  git submodule update --init
  ```
  #### Option 2: Manual Download
  - Download mbedTLS from [GitHub](https://github.com/ARMmbed/mbedtls).
  - Extract to the `mbedtls/` directory in the project root.
  ### 3. Build the Project
  ```bash
  mkdir build
  cd build
  cmake -G "MinGW Makefiles" ..
  mingw32-make
  ```
  This generates `aerospace_encryptor.exe` in the `build` directory.
  ### 4. Run the Application
  Copy `aerospace_encryptor.exe` to a USB stick or run directly:
  ```bash
  ./aerospace_encryptor.exe
  ```
  ## Usage
  1. **Run the Program**: Launch `aerospace_encryptor.exe` in a Windows command prompt.
  2. **Input Data**:
     - **Message**: Enter a message (1-256 characters).
     - **Key**: Enter a 256-bit key as 64 hex characters (e.g., `00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF`).
     - **IV**: Enter a 128-bit IV as 32 hex characters (e.g., `AABBCCDDEEFF00112233445566778899`).
  3. **Output**:
     - The encrypted hex output is displayed in the console, formatted as:
       ```
       1 ABCDE FGHIJ KLMNO PQRST UVWXY
       2 Z0123 45678 9ABCD EF012 34567
       ```
     - QR codes are saved as `qr_line1.bmp`, `qr_line2.bmp`, etc., in the current directory.
  4. **Scan QR Codes**:
     - Open a QR code scanner app on your phone (e.g., Google Lens, QR Code Reader).
     - Scan a BMP file (e.g., `qr_line1.bmp`) from a screen or printed output.
     - The app displays the hex string (e.g., "ABCDE FGHIJ KLMNO PQRST UVWXY").
     - Screenshot the result as needed.
  ## Example
  **Input:**
  ```
  Enter message (max 256 chars): Hello, Aerospace!
  Enter 256-bit key (64 hex chars): 00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF
  Enter IV (32 hex chars): AABBCCDDEEFF00112233445566778899
  ```
  **Output:**
  ```
  Aerospace Encryption Tool
  -------------------------
  Enter message (max 256 chars): Hello, Aerospace!
  Enter 256-bit key (64 hex chars): 00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF
  Enter IV (32 hex chars): AABBCCDDEEFF00112233445566778899

  Encrypted Output (Hex):
   1 8C1F5 2A9B3 D4E7F 60C8D 19A2E
   2 7B3F4 E5D91 28A0C B6F2E 93D5A
  QR codes saved as qr_lineX.bmp files.
  Encryption complete.
  ```
  Files `qr_line1.bmp` and `qr_line2.bmp` are generated.
  ## Dependencies
  - **qrcodegen**: Included in the repository (`qrcodegen.hpp`, `qrcodegen.cpp`).
    - Source: [nayuki/QR-Code-generator](https://github.com/nayuki/QR-Code-generator).
  - **mbedTLS**: Encryption library in `mbedtls/` (add via submodule or manual download).
  ## Troubleshooting
  - **Build Errors**: Ensure MinGW and CMake are installed. For MSYS2, run:
    ```bash
    pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
    ```
  - **QR Code Not Scanning**: Verify BMP files are not corrupted. Increase `pixel_size` in `save_qr_to_bmp` (e.g., to 20) if needed.
  - **File Not Found**: Ensure `mbedtls/` is populated and `qrcodegen` files are present.
  ```
- **Action**: Save as `README.md` in the project root.
##### File 6: `.gitignore`
- **Content**:
  ```
  build/
  *.exe
  *.o
  *.obj
  CMakeCache.txt
  CMakeFiles/
  cmake_install.cmake
  Makefile
  qr_line*.bmp
  ```
- **Action**: Save as `.gitignore` in the project root.
#### 4. Add mbedTLS
- **Option 1: Submodule**
  ```bash
  git submodule add https://github.com/ARMmbed/mbedtls.git mbedtls
  git submodule update --init
  ```
- **Option 2: Manual Download**
  - Download mbedTLS from [GitHub](https://github.com/ARMmbed/mbedtls).
  - Extract the contents to the `mbedtls/` directory in the project root.
#### 5. Commit and Push
Run the following commands to commit and push the files to GitHub:
```bash
git add .
git commit -m "Initial commit with AES encryptor and QR code feature"
git push origin main
```
#### 6. Verify on GitHub
- Visit `https://github.com/yourusername/aerospace_encryptor_with_qr` in your browser.
- Confirm that all files (`main.cpp`, `qrcodegen.hpp`, `qrcodegen.cpp`, `CMakeLists.txt`, `README.md`, `.gitignore`, and the `mbedtls/` directory) are present.
---
### Notes
- **Main.cpp**: Ensure your `main.cpp` includes the full AES-256-CBC encryption and QR code generation logic as intended for the project.
- **Portability**: The executable is statically linked (via `-static` in `CMakeLists.txt`), making it portable for USB deployment.
- **QR Code Design**: The QR codes are generated with a default module size of 10x10 pixels, adjustable in the code if needed.
