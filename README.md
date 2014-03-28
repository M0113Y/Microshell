Microshell
==========


This program mimics the bash shell by allowing the user to do basic commands and pipeing.


To pipe:  command || command   	


DO NOT USE |  


To exit shell type   quit or  q



Example:
========
	 myshell> ls -al 

	 myshell> cat myfile || sort

	 myshell> more myfile || grep USA
 
	 myshell> quit 





myfile is text file containing random strings






Install: 
========	
	 1. Download the tar file to any linux directory 

	 2. Go to the command line and type in the downloaded directory: tar -xvf microshell.tar

	 3. While still in the current directory type: cd microshell_dir

	 4. Run make command 

	 5. Type ./microshell or microshell to execute 




