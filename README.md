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
