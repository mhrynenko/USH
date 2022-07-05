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
  |env | descr <br> -i - about <br> -P - about <br> -u - about |           
  |cd | descr <br> -s - about <br> -P - about <br> -`-` - about |
  |pwd [-LP] | descr <br> -L - about <br> -P - about |
  |which [-as] name| descr <br> -a - about <br> -s - about |
  |echo [-neE] [arg ...]| descr <br> -n - about <br> -e - about <br> -E - about |
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


### How to start
*This program was developed on the MacOS, so it works correctly there.*   

In folder with cloned project use `make` to compile project. Start program with `./ush`.

To delete the program - `make uninstall`   
To delete all 'obj' files - `make clean`   
To reinstall the program - `make reinstall`

