# Using VSCode to debug the Examples

## Creating a Task Configuration File

- Open the Command Palette in VS Code by pressing Ctrl+Shift+P (Windows, Linux) or Cmd+Shift+P (macOS).
- Type "Tasks: Configure Task" in the Command Palette and select it.
- In the dropdown menu, select "Create tasks.json file from template".
- In the next dropdown menu, select "Others" to create a custom task.
- VS Code will open the tasks.json file for editing. Replace the contents of the file with the following:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build with custom script",
      "type": "shell",
      "command": "./build_linking_example.sh",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "presentation": {
        "reveal": "silent"
      },
      "problemMatcher": {
        "owner": "cpp",
        "fileLocation": ["relative", "${workspaceFolder}"],
        "pattern": {
          "regexp": "^(.*):(\\d+):(\\d+):\\s+(warning|error):\\s+(.*)$",
          "file": 1,
          "line": 2,
          "column": 3,
          "severity": 4,
          "message": 5
        }
      }
    }
  ]
}


```

## Creating a launch.json File

- Click on the "Debug" icon in the left-hand menu (or press Ctrl+Shift+D on Windows/Linux, or Cmd+Shift+D on Mac).
- Click on the gear icon (⚙️) to the right of the "Run" and "Debug" buttons, and select "Add Configuration..." from the dropdown menu.
- In the popup that appears, select the environment you want to configure. For example, if you're using C++, you might select "C++ (GDB/LLDB)".
- This should generate a new launch.json file and open it in the editor. You can modify this file to customize your debugging configuration.

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug C Program",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/linked_shared_library_example",
      "args": [],
      "stopAtEntry": true,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "build with custom script"
    }
  ]
}

```
