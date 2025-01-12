# Geolocalization of IP Address using IPStack

This application is designed to show the geolocation of an IP address based on the provided IP. You can search by either a **hostname** or an **IP address** (both IPv4 and IPv6 are supported).

The UI consists of two main parts:
1. A list of added addresses along with their corresponding geolocation.
2. An OpenStreetMap view that shows the geolocation on the map.

The application is built using the **QT Framework** and includes an **SQLite** database to store data between app sessions.

Unit tests are written using **QT Test** and **Google Test**.

---

## Target OS

> **Note:** The default operating system for this project is **Windows** due to the use of the `ws2_32` library. Non-Windows users should adjust the project configuration to suit their operating system.


## Prerequisites

### 1. Google Test
- Download **Google Test** from the official repository:  
  [Google Test Repository](https://github.com/google/googletest)
  
- Add the unpacked directory to your system's environment variables.  
  Example for Windows:
  ```bash
  GOOGLE_TEST : C:\Users\Paweł\Documents\googletest-main\
  ```

### 2. SQLite
- This application requires the **SQLite** database driver.
- Installation instructions can be found on the official SQLite website:  
  [SQLite Installation](https://www.sqlite.org/index.html)

### 3. QT
Ensure that you have the default **QT** installation with the following libraries:
- **Widgets**
- **Quick**
- **QML**
- **QuickWidgets**
- **Test**
- **SQL**

### 4. CMake
Make sure you have **CMake** installed on your system.  
You can download it from: [CMake Downloads](https://cmake.org/download/).

### 5. API Key
An **API key** is required to use the **IPStack API**. It must be set as an environment variable with the name `IPSTACK_API_KEY`.

To retrieve the API key:
Make sure to configure the API key in your environment variables before running the application.

---

## Basic Architecture

The application is split into a few major modules:

- **IIPResolver**: Validates user input and translates hostnames to IP addresses.
- **IGeolocationProvider**: Provides geolocation data based on the IP address. Implemented by `IPStackApiClient`.
- **IDataStorage**: Interface for data storage. The implementation is `SqlLiteDataStorage`, which uses SQLite.
- **MainWindow**: Manages the UI and schedules tasks based on user interaction.

### Libraries Used
This application uses the following third-party libraries, which are included in the repository:
- **RapidJSON**: Used for **JSON parsing** and **schema validation**.
- **httplib**: Used for making HTTP requests to the **IPStack API**.

---

## Quick Setup

### 1. Install Prerequisites

Follow the prerequisites listed above to install **Google Test**, **SQLite**, **Qt**, **RapidJSON**, **httplib**, and set up the **API key**.

### 2. Download the Repository

Clone the repository using the following command:

```bash
git clone <repository-url>
cd <project-folder>
```

### 3. Open the Project

You have two options to open and run the project:

- **Option 1 (Recommended):** Open the project in **Qt Creator** and simply click the **Run** button. This will automatically handle the build and execution process for you.
  
- **Option 2:** Alternatively, if you prefer to manually set up the build, navigate to the project directory using the console:

  ```bash
  cd <project-folder>
  ```

  Then follow steps 4-7 to configure and build the project manually.

### 4. Create a Build Directory

If you're building manually, create a directory where CMake will generate the build files:

```bash
mkdir build
cd build
```

### 5. Run CMake

Run CMake to configure the project. Make sure to specify the correct Qt installation directory if it's not already set in your environment variables:

```bash
cmake ..
```

If you need to specify a Qt installation path manually, use:

```bash
cmake .. -DCMAKE_PREFIX_PATH=<path_to_your_qt_installation>
```

### 6. Build the Project

Once CMake has configured the project, build it using:

```bash
cmake --build . --config Debug
```

### 7. Run the Application

After the build is complete, run the application:

```bash
.\IPStack.exe
```

---

### Key Notes:

- **Qt Creator (Option 1)**: If you open the project in **Qt Creator** and click **Run**, **you do not need to follow steps 4-7**. Everything will be automatically handled for you.
  
- **Manual Setup (Option 2)**: If you're building manually, follow steps 4-7 for configuration and build.

---
