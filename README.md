# FileSend

A fast, lightweight terminal-based file transfer tool for local networks with automatic device discovery.

FileSend allows you to send files between devices on the same network without manually entering IP addresses. Devices are discovered automatically and identified by user-friendly names.

---

## ✨ Features

- 🔍 Automatic device discovery (no IP required)
- 🏷️ Human-readable device names
- 📁 Simple file selection from terminal
- ⚡ Fast TCP-based file transfer
- 🖥️ Terminal UI using ncurses
- ⚙️ Persistent configuration (`~/.config/filesend/`)
- 🌐 Cross-platform support (Linux / Windows)

---

## 📦 Installation (Linux)

### Option 1: Install via `.deb` (Recommended)

Download the latest release from GitHub.

Then run:

```bash
sudo apt install ./filesend_1.0_amd64.deb
```

---

### Option 2: Build from source

```bash
git clone https://github.com/Reyonce-Aswin/filesend.git
cd filesend
make
sudo make install
```

---

## 🚀 Usage

Start the application:

```bash
filesend
```

---

### 📤 Send a file

1. Select `Send`
2. Choose a device from discovered list
3. Select file to transfer
4. File is sent automatically

---

### 📥 Receive a file

1. Select `Receive`
2. Wait for incoming transfer
3. File will be saved in current directory

---

### 🏷️ Rename device

1. Select `Rename`
2. Enter new device name
3. Name will be saved for future sessions

---

## ⚙️ Configuration

FileSend stores configuration at:

```
~/.config/filesend/config
```

Example:

```
initialized=1
name=My-PC
```

---

## 🧹 Uninstall

```bash
sudo dpkg -r filesend
```

---

## 🛠️ Build Requirements

- GCC
- ncurses (wide version)
- pthread

Install dependencies:

```bash
sudo apt install build-essential libncursesw5-dev
```

---

## 📁 Project Structure

```
filesend/
├── src/
├── include/
├── build/
├── dist/
├── packaging/
├── Makefile
```

---

## 🧠 How it works

- Uses UDP broadcast for device discovery
- Uses TCP for reliable file transfer
- Runs a background listener for discovery requests
- Maps device names to IP addresses internally

---

## 🚧 Future Improvements

- File transfer progress indicator
- Device caching
- Encryption support
- Multi-file transfer
- Improved UI navigation

---

## 🤝 Contributing

Contributions are welcome. Feel free to open issues or submit pull requests.

---

## 📄 License

This project is licensed under the Apache 2.0 License.

---

## 👨‍💻 Author

Developed by Reyonce Aswin
