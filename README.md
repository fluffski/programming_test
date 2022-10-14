# Simudyne Backend Programming Test
## How to run program

Program is run in two steps:
1. Run build and run cpp file with the required arguments - use main.cpp.
2. Run jupyter notebook script - use ModelOutput.ipynb or copy code provided further in your own jupyter notebook.

### Simulating model output
There are three input arguments required to run cpp file:
1. Brand factor with values ranging from (0.1 - > 2.9).
2. Input data file name.
3. Output data file name.

To make it easier, open terminal in directory with cpp and ipynb file. 
In terminal run these lines:
$ g++ main.cpp -o main
$ ./main arg1 arg2 arg3

If files exist or must be generated in different location, provide paths along filenames as appropriate.

Example:
$ g++ main.cpp -o main
$ ./main 2.1 PseudoData.csv OutputData.csv

The program will parse input csv file in (e.g. PseudoData.csv), process data and save the output into output csv file (e.g. OutputData.csv).

The program is configured to process csv with carriage return as an end of line character. If there is a problem with processing data, try changing delimited in getline(input, line,'\r') to '\n' (getline(input, line,'\n')) on line 139.

### Displaying output
Open ModelOutput.ipynb and run. There is only one cell. Alternatively, run the code below in your favourite place to process it:
```
import pandas as pd

# get file name from the user
print("Provide file name containing model data:")
outputFilename = input()

# reading the database
data = pd.read_csv(outputFilename)

# printing the results
display(data)
```

This will prompt user to provide file name of the output data and then display the results.


## Task Description:
You are being asked to create a simple agent based model of a set of agents (with values as defined in PsuedoData) that will execute the basic logic (as defined in PsuedoCode). You should fork this repo and once finished with your submission please email the Github link to john.andrews@simudyne.com

Overall you'll need to do the following things:

1. Use C++ for the code execution of agents and parsing of data
2. Please use either a script/command line argument/Juptyer notebook entry for the single input of Brand_Factor (but it should be somehow variable to a user)
3. Use a Juptyer notebook to either interop with the C++ code/program directly or indirectly (such as reading outputted data) and use whatever method or library you like to display the results of the program execution as defined by outputs in the psuedo code.
4. If you have questions please email john.andrews@simudyne.com

In terms of what we are looking for while the test can be “solved” in under 1 hour - we recommend being creative with your submission. We are looking for submissions that show a good understanding of:

* Optimized code in terms of performance and speed
* Program setup & design
* Management of data inputs and outputs
* Good documentation and instructions to run your code
* Code being robust and extendable potentially beyond what you write for the assignment
* Test-driven development focused code
* Analysis of output and key findings (and/or code to analyze data)

Your solution doesn’t need to be perfect in all the above areas. It’s better in the time frame to focus on one aspect to show off your strengths. Following the test, there may be a code review that asks why you made the choices or tradeoffs you did, and what you think the next hypothetical steps would be.
