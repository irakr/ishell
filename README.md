# ishell
'ishell' is a shell program similar to a linux shell, like bash,sh,csh,ksh etc.
It contains almost any feature that a shell contains. Its more like the bash shell.
It can be used just like any shell via a terminal.

# How to install and use
- To install, just run the install.sh script in the main directory using sudo privilages. Run the command mentioned below:
	$ sudo ./install.sh
- To use it, simply go to the terminal anytime and run the command 'ishell' to start using ISHELL.
	$ ishell

# How to manually compile and install(If you are more curious) 
- Currently the process of building the software is not so clean.
- After you download the package, go to the src/ directory.
- Then run the 'make' command from your terminal.
- Then the executable file will be available in the bin/ directory with the name 'ishell'.
- You can either run it from there. But a better way is to copy the executable file 'ishell' to the /usr/bin or /usr/sbin directory.
- Performing the second option will let you run the command directly from your terminal by simply entering the executable filename 'ishell' as
	$ ishell

# OUTPUT
- Currently you can run any simple command without using pipes. A single background command might still work.
	Example: $ gedit &
- The correct and complete output will be soon produced.
