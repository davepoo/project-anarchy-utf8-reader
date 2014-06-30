Project Anarchy UTF-8 Reader
===========================

The `cBufferedReaderUTF8` class allows line by line reading of a UTF-8 text file created with [Notepad++](http://notepad-plus-plus.org/ "Notepad++") inside the [Project Anarchy game engine](https://www.projectanarchy.com/ "Project Anarchy game engine").

# Usage

* Create a file in Notepad++
* Enter a few lines of text
* Save it as UTF-8 encoded without BOM
* The following example code shows how read the file inside your game using the `readLine()` method

```C++

	#include "BufferedReaderUTF8.h"
	
	// Reads 10 lines of text from the file
	void loadText(VString file) {
		//open the stream
		IVFileInStream* fin = Vision::File.Open(file);
		VASSERT_MSG(fin != NULL, "The text file could not be opened" );
		cBufferedReaderUTF8 reader = cBufferedReaderUTF8( fin );
	
		//read the number of strings in the file
		const int numStrings = 10;
		for (int i = 0; i < numStrings; i++) {
			VString line = reader.readLine();

			// do something with the line that was read here....
		}
		
		
		//close the stream
		fin->Close();
	}
```

# Author

[https://www.projectanarchy.com/users/davepoo](https://www.projectanarchy.com/users/davepoo)

# License

This source code is free do anything you want with
