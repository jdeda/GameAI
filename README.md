# PathPlanning

### Building the Program
To build the program, please run "builder.sh" via ./builder.sh. You may need to enable permissions, which may be done via "chmod u+x builder.sh". If you change a header file and wish to call make, you must "make clean" before calling "make" as it will not pickup the dependency change!

### Running the Program
To run the program, simply run "./main". Make sure the program was built first, especially if you changed any files! The program takes user input, as described below.

You will be prompted with several visualizations to chose from. 
1. Small Graph
2. Large Graph
3. Huge Graph
4. Character Graph
5. Testing

Here is some important information you need to know before running them:
1. Small Graph -- takes a couple seconds to run and render
2. Large Graph -- takes a couple minutes to run and rendr
3. Huge Graph -- takes over half an hour to run and render
4. Character Graph -- takes a couple seconds to run and render
5. Testing -- takes about an hour to run
Thus, Huge Graph is terribly slow (search algorithm takes almost the entire time) and Testing is slow (testing 4 algorithms 10 times which each take around two and a half minutes).

Additionally you will be prompted for some algorithms in some cases. Choose to your liking.
