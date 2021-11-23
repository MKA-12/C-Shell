This is to guide you through the usage of the attached shell programmed through C.

File Structure:

	src-
        -Makefile
        -main.c
        -display.c
        -input.c
        -ls.c
        -pwd.c
        -cd.c
        -echo.c
        -pinfo.c
        -background.c
        -foreground.c
        -cron.c
     -README.txt

Invoke Terminal:
Use the following command in the src directory to obtain "shell" binary file.
$ make
If readline.h is not available, On Debian based platforms, like Ubuntu, you can run:
sudo apt-get install libreadline-dev to install it and then run make. 
Use the following command to start a terminal session
$ ./shell

-main.c

-> Prompt displayed before each input using function display_user() in display.c.
-> Input read using parse_input() in input.c.
-> Commands separated with respect to ';' and stored in a 2D array named "commands". This is done using the function store_input() in main.c .
-> All tabs and spaces are stripped for every command in "commands" using the function store_command() in main.c .
-> Redirection of control to various files/functions done according as the the input given by the user. "Command not found" displayed appropriately.

-display.c

-> Contains display_user() function explicitly to display the prompt. 
-> Support for using redirection commands like 
$ diff file1.txt file2.txt > output.txt
$ sort < lines.txt
$ sort < lines.txt > sortedlines.txt
-> '>>' also supported.
-> Piping commands can be given
$ more file.txt | wc
$ grep "new" temp.txt | cat somefile.txt | wc
$ cat < in.txt | wc -l > lines.txt
->​ setenv ​var [value] : To set environment variables to shell , requires one or two arguments. Use 
$ echo $var
to print the value of the environment variable. 
-> unsetenv var : Destroys the environment variable var.

-input.c

-> Has the function parse_input() which is used to take input from the user and return to main.c.

-ls.c

-> Contains ls() function which when invoked lists files in given directory according to the flags given.
-> Multiple flags can be given such as "-a","-l","-al","-la" or no flag or given flags separated by spaces. Entering any other flag will display error message.
-> Multiple directories can be given and flags are applied to all directories given. Error displayed when invalid directory name given. 

-pwd.c

-> Has the function pwd() which prints the current working directory of the shell. 

-cd.c

-> Changes directory to given directory. Error thrown when given directory is not found. 

-echo.c

-> echo bash command implemented using echo() function. Only the case "echo <string1>  <string2> " is handled. 

-pinfo.c

-> Current Process info is printed when "pinfo" command used. For a particular process information use "pinfo <process_id>". pinfo() function in this file implements this functionality.
-> Error thrown when given process id does not exist.

-background.c/foreground.c

-> Non built-in commands are executed
-> execvp() executes all the other commands
-> Handles foreground and background processes seperately.
-> Prompts the user on exiting a background process.
-> Foreground processes stop the terminal.
-> A background process is created whenever "&" is given as second argument to commands like "vi","emacs","firefox" etc.
-> Exit code printed to stderr when the process is killed.
-> exit_child() function used for printing exit status.
-> getProcessStatus() calculates the status of the jobs running in background. 
-> "jobs" command prints current list of jobs and statuses along with their pids. 
$ jobs
->"kjob" takes the job id of a running job and sends a signal value to that process
$ kjob <job-number> <signal-number> 
-> fg <jobNumber> ​ : brings a running or a stopped background job with given job number to foreground.
$ fg 3
​-> bg <jobNumber> ​ : changes a stopped background job to a running background job.
$ bg 1
-> overkill : kills all background process at once.
-> quit ​ : exits the shell. ignores SIGINT, SIGCHLD
-> CTRL-Z ​ :changes the status of currently running job to stop, and push it in background process.
-. CTRL-C ​ : causes a SIGINT signal to be sent to the current foreground job of the shell​ . If there is no foreground job, then the signal won't have any effect.
