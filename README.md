# How to Play

*   **Movement:** Use the **W, A, S, and D** keys to move your character.
*   **Attack:** Press the **F** key to use your equipped magic or spell.
*   **Change Skills:** Press the **1, 2, 3, or 4** keys to switch between different skills.

# Project Setup Instructions

## 1. Prerequisites

Ensure you have the following software installed before you begin:

*   **Visual Studio 2022** (with the "Desktop development with C++" workload)
*   **CMake** (version 3.22 or newer)
*   **Git**
*   **Dependencies and assets** (https://github.com/nthday-jpg/EntropysGrasp.git)

## 2. Clone the Repository

First, open your command line or terminal and run the following commands to download the project files and navigate into the project directory:

```bash
git clone https://github.com/nthday-jpg/EntropysGrasp.git
cd EntropysGrasp
```

## 3. Open the Project

Launch Visual Studio 2022 and open the cloned project folder.

## 4. Configure Project Properties

You will need to configure the project's properties to locate the necessary files. Right-click on the project in the Solution Explorer and select **Properties**.

### A. Set C++ Standard

1.  Navigate to **Configuration Properties** -> **C/C++** -> **Language**.
2.  Set the **C++ Language Standard** to **C++17** or a higher version.

### B. Configure VC++ Directories

1.  Go to **Configuration Properties** -> **VC++ Directories**.
2.  Edit the **Include Directories** and add the following paths:
    ```
    $(ProjectDir)dependencies\nlohmann
    $(ProjectDir)dependencies\tinyxml2\include
    $(ProjectDir)dependencies\SFML\include
    $(ProjectDir)dependencies\EnTT\include
    ```
3.  Edit the **Library Directories** and add the following paths:
    ```
    $(ProjectDir)dependencies\tinyxml2\lib
    $(ProjectDir)dependencies\SFML\lib
    ```

### C. Configure Linker Dependencies

1.  Go to **Configuration Properties** -> **Linker** -> **Input**.
2.  Edit **Additional Dependencies** and add the following libraries:
    ```
    sfml-graphics.lib
    sfml-window.lib
    sfml-system.lib
    sfml-audio.lib
    tinyxml2.lib
    ```

## 5. Set Up Runtime DLLs

Copy all the required DLL files from the following folders:

*   `dependencies/SFML/bin`
*   `dependencies/tinyxml2/bin`

Paste these DLLs into your project's output folder (e.g., `Debug/` or `Release/`).

## 6. Build the Project

1.  Select your desired build configuration (**Debug** or **Release**) from the dropdown menu in the toolbar.
2.  In the main menu, go to **Build** -> **Build Solution**.

If the configuration is correct, the executable file (`.exe`) will be generated in your selected output folder.

## 7. Run the Game

You can run the game in two ways:

*   **Directly from Visual Studio:** Press the "Start" button.
*   **From the output folder:** Navigate to the `Debug/` or `Release/` folder and double-click the `.exe` file.

**Note:** Ensure that the SFML DLL files you copied earlier are in the same folder as the game's executable.