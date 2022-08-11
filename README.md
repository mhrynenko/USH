<h1 align='center'> USH </h1>

### Description
This program is a basic command-line interpreter with features without which there can be no
shell. As a reference, i've taken **zsh**. During developing was implemented:  
- default prompt looks like u$h> , followed by a space character
- builtin commands without ﬂags:
  |Command|Description|
  |:-----:|:---------:|
  |export [name[=value] ...] | Set an environment variable with specified name |           
  |unset  name ... | Delete given shell variable. |
  |exit [n]  | Exit the shell with the exit status specified by n expression, if none is specified, exit with 1 status. |
- builtin commands with ﬂags:
  |Command|Description|
  |:-----:|:---------:|
  |env | Set each NAME to VALUE in the environment and run COMMAND <br> -i - start with an empty environment <br> -P altpath - Search the set of directories as specified by altpath to locate the specified utility program, instead of using the value of the PATH environment variable. <br> -u name - remove variable from the environment |           
  |cd | Change current directory. <br> -s - cd refuses to change the current directory if the given pathname contains symlinks <br> -P - symbolic links are resolved to their true values <br> -`-` - change to the previous directory |
  |pwd [-LP] | Print the absolute pathname of the current working directory <br> -L - he default one <br> -P - if -L flag is not given, the printed path will not contain symbolic |
  |which [-as] name| For each name, indicate how it would be interpreted if used as a command name. If name is not a built-in command or a reserved word, the exit status shall be non-zero, and a message will be written to standard error.<br> -a - do a search for all occurrences of name throughout the command path. Normally only the first occurrence is printed. <br> -s - if a pathname contains symlinks, print the symlink-free pathname as well. |
  |echo [-neE] [arg ...] | Write each arg on the standard output, with a space separating each one. <br> -n - do not output the trailing newline <br> -e - enable interpretation of backslash escapes <br> -E - disable interpretation of backslash escapes (default) |
  
- calling the builtin command instead of the binary program if there is a name match among them
- correct managing errors
- correct managing user environment
- running programs located in the directories listed in the PATH variable
- the command separator ;
- managing these expansions correctly:   
  - the tilde expansion ~ with the following tilde-preﬁxes: ~ , ~/dir_name , ~username/dir_name , ~+/dir_name , ~-/dir_name
  - the basic form of parameter expansion ${parameter} 
  - the two-level-nested command substitution $(command

#### Some examples of working    
![Снимок экрана 2022-06-25 в 06 24 04](https://user-images.githubusercontent.com/108219165/184127681-5715ade8-593e-44d1-8211-cee51aa82c1d.png)


### How to start
*This program was developed on the MacOS, so it works correctly there.*   

In folder with cloned project use `make` to compile project. Start program with `./ush`.

To delete the program - `make uninstall`   
To delete all 'obj' files - `make clean`   
To reinstall the program - `make reinstall`

