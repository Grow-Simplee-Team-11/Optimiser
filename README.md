# OR Tools Setup 
## Binary Installation
First, download the binary version corresponding to your distro, commands for that are as follows :
```bash
curl -LJO "<or-tools package link for your distro>"
tar -xf <name of tar file downloaded>;
```
Link to the binary versions can be found [here](https://github.com/google/or-tools/releases/) , look for the binary corresponding to your distro and use that link to run the above command
For Ubuntu 22.04 it looks like the follwing:
```bash
curl -LJO "https://github.com/google/or-tools/releases/download/v9.5/or-tools_amd64_ubuntu-22.04_cpp_v9.5.2237.tar.gz"
tar -xf or-tools_amd64_ubuntu-22.04_cpp_v9.5.2237.tar.gz;
```
## Folder Structure
After downloading the binary version on your system do the following -
1. Copy the entire `lib` folder present inside the downloaded package and place it in the root directory of this folder
2. Copy all the files present in `include` directory of ortools package and place it inside a folder called `include/ortools`
The Folder Structure would finally look as follows
![Folder Structure](folder_structure.png)
3. To make sure g++ looks for linking libraries in the created lib folder, run this command once at the root of this folder - 
```bash
export LD_LIBRARY_PATH=./lib:/usr/lib:/usr/local/lib
```
## Execution
Run the following commands at the root of this folder to make and run the executable - 
```bash
make
./main
```
Additionally, to remove unnecessary files - 
```bash
make clean
```