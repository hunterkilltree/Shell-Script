### Bash Scripts

###### What is **Bash Scripts** ?
**Bash Scripting** is an useful for system administration and development.

**Bash** is a UNIX Shell, which is a command line interface (CLI) for interacting with an operating system (OS). Scripts are used to run a **series of commands**.

##### Why we need it ? (Collect from Quora)
*
*
*
*

### Common Commands
```console
cd ~                    # to return to home directory
mkdir folderName        # to create new folder with folderName
chmod +x namefile.sh    # to add execute permission to the file
                        # +x is telling to add (+) the executable bit (x) to the owner, group and others.
uname -a                # show all system information
zip nameFile fileNeedtoZip.extend

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
