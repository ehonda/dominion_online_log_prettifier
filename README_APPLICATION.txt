The purpose of this application is to make copy/pasted dominion online logs readable. Some users get malformatted logs
when copy/pasting from the gamelog in e.g. firefox that are filled with unnecessary newline characters in the wrong places.
The logprettifier is a commandline application and can repair those malformatted logs. See the bottom of this readme for
usage examples.


USING THE LOGPRETTIFIER

0.	(Optional) Add the folder containing logprettifier.exe to your PATH environment variable. That way, you can invoke the
	application in a shell by simply typing "logprettifier" without having to specify the full path to the executable.
	
1.	Open a command prompt (e.g. cmd.exe on windows).

2.	Type "logprettifier path/to/dominion_log.txt" to prettify the specified log.


CONTROLLING THE BEHAVIOUR OF THE APPLICATION
You can control the logprettifier application by passing one or several of the following commandline arguments:

path/to/file.txt		This represents a path to a file containing either a full dominion online log or only parts of it.
						At least one filepath has to be passed to the application, this is the log to be worked on. If two
						filepaths are passed the second is interpreted as the destination to save the prettified log to. Only
						filepaths ending in .txt are accepted. Two is the maximum number of filepaths that can be specified.
						If the path you want to specify contains spaces make sure to surround it by ""
						
-o						This option tells the application to overwrite the file that the prettified log should be saved to,
						if it is present. If you specify only the path to the source file, this option will lead to the
						unprettified log being overwritten by its prettified version. If you specify a source and destination
						file, the destination file will be overwritten if it is present. If this option is not activated, the
						the application will make sure no files (except those ending in _prettified.txt) are overwritten by
						appending _prettified to the filename of the prettified log in case saving it to that destination would
						cause a file to be overwritten.
						
-h						Specifying this option shows the helptext. It can not be specified in combination with any other arguments.


USAGE EXAMPLES
In the following examples we are gonna assume we added the path to logprettifier.exe to our PATH and have a folder "c:/logs" containing
a malformatted dominion log called "bad.txt"

> logprettifier c:/logs/bad.txt
	This will result in a file called "bad_prettified.txt" being created in "c:/logs" which contains the prettified log. The original
	will stay unmodified.
	
> logprettifier c:/logs/bad.txt -o
	This will result in the original file being overwritten by the prettified version.
	
> logprettifier c:/logs/bad.txt c:/logs/good.txt
	This will result in a file called "good.txt" being created in "c:/logs" which contains the prettified log if there doesn't already
	exist a file called "good.txt" at that destination. If "good.txt" already exists, the prettified log will be saved to the file
	"good_prettified.txt". The original will stay unmodified.
	
> logprettifier c:/logs/bad.txt c:/logs/good.txt -o
	This will result in a file called "good.txt" being created in "c:/logs" which contains the prettified log. If "good.txt" already
	exists at that destination it will be overwritten. The original will stay unmodified.