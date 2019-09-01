### Bash Scripts

###### What is **Bash Scripts** ?
**Bash Scripting** is an useful for system administration and development.

**Bash** is a UNIX Shell, which is a command line interface (CLI) for interacting with an operating system (OS). Scripts are used to run a **series of commands**.

##### Why we need it ? (Collect from Quora)
* You can write a script to initialize something at boot time of the system. so you don't need to do manually.
* You can write a script which installs per-requisite and build the code with user input to enable/disable some features.
* To kill or start multiple applications together.
* To observe large database of files and find some patterns out of it.
* So in general to automate the process and the list goes on....

### Common Commands
```console
cd ~                    # to return to home directory
mkdir folderName        # to create new folder with folderName
chmod +x namefile.sh    # to add execute permission to the file
                        # +x is telling to add (+) the executable bit (x) to the owner, group and others.
uname -a                # show all system information
zip nameFile fileNeedtoZip.extend
lscpu                   # show cpu information
cat /proc/meminfo       # show ram bla bla


```

### Some Special Commands
```console
install apt install htop
htop                     # like task manager in window
```

### Variables
A Variables is declared without a **$**, but has a **$** when invoked.
```console
#!/bin/bash
myName="ubuntu"
echo Hello, $myName!
```
Note :
```cosole
#!/bin/bash
myName = "ubuntu"
echo Hello, $myName!
```
**Invalid-** We can not push space between variable and value.

### Reading
Using the **read** command

```console
#!/bin/bash
echo "Enter your name: "
read yourName
echo "You are, $yourName"
```

### Conditionals
**Keywords:**
```console
if [condition]
then
.....
else
....
fi
```
Example :
```console
#!/bin/bash

echo "Which year were you born in ?"
read year
if [ "$year" -gt 1998 ]
then
    echo "Me too."
else
    echo "Wow."
fi
```
**Note :**
 * Invalid when you are missing space example : if ["$year" -gt 1998]
 
| **Bash Operator** | **Operator** | **Description**             |  
|-------------------|------------- | ----------------------------|
| -eq               |       ==     | Equal                       |
| -ne               |       !=     | Not Equal                   |
| -gt               |       >      | Greater than                |
| -ge               |       >=     | Greater than or equal       |
| -lt               |       <      | Less than                   |
| -le               |       <=     | Less than or equal          |
| -z                |       == null| Is null                     |

### Looping
Using **for**, **while**, **loops**. In example: **for...in**
```console
#!/bin/bash

FILES = /Users/.....
for file in $FILES
do
    echo $(basename $file)
done
```

### Git Example Script
```console
#!/bin/bash

read -r -p 'Commit message: ' desc  # prompt user for commit message
git add .                           # track all files
git add -u                          # track deletes
git commit -m "$desc"               # commit with message
git push origin master              # push to origin

```

### Vim mode
* press **i** ,  --INSERT-- mode
* press **Esc** , go to command mode
    * press **h**, go left
    * press **j**, go down
    * pres **k**, go up
    * press **l**, go right
    * press **x**, delete character
    * press **u**, undo
    * press **Ctrl + r**, redo
    * press **:q!**, "quit and throw things away"
    * press **:x**, save file



### Makefile
